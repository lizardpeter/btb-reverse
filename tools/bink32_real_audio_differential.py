#!/usr/bin/env python3
"""Compare the native Bink audio decoder with FFmpeg on original SJPi BIKs.

The source files are never modified. FFmpeg does not recognize this game's
"SJPi" signature, so the script makes a temporary demuxer copy with only the
first four signature bytes changed to "BIKf". The native audit always reads
the untouched original file.

Usage:
  python tools/bink32_real_audio_differential.py \
      --native build/reconstruction/btb_bink32_native_audio_audit \
      --asset-dir "C:/path/to/BTB-BBP"
"""
from __future__ import annotations

import argparse
import math
import shutil
import struct
import subprocess
import sys
import tempfile
from pathlib import Path


MAX_FILE_BYTES = 100_000_000
MAX_TRACKS = 64


def read_source(path: Path) -> tuple[bytes, int]:
    data = path.read_bytes()
    if len(data) < 44 or len(data) > MAX_FILE_BYTES:
        raise ValueError(f"file size outside bounds: {len(data)} bytes")
    if data[:4] != b"SJPi":
        raise ValueError("not a supported original SJPi movie")
    declared_size = struct.unpack_from("<I", data, 4)[0]
    if declared_size != len(data) - 8:
        raise ValueError("file-size field does not match original bytes")
    track_count = struct.unpack_from("<I", data, 40)[0]
    if track_count > MAX_TRACKS or 44 + track_count * 12 > len(data):
        raise ValueError(f"invalid source audio track count: {track_count}")
    return data, track_count


def run_checked(command: list[str], label: str) -> None:
    result = subprocess.run(command, capture_output=True, text=True)
    if result.returncode:
        detail = (result.stderr or result.stdout).strip()
        raise RuntimeError(f"{label} failed: {detail[-1200:]}")


def pcm16(path: Path) -> tuple[int, ...]:
    data = path.read_bytes()
    if not data or len(data) % 2:
        raise ValueError(f"invalid or empty PCM16 output ({len(data)} bytes)")
    return struct.unpack(f"<{len(data) // 2}h", data)


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--native", required=True, type=Path,
                        help="built btb_bink32_native_audio_audit executable")
    parser.add_argument("--asset-dir", required=True, type=Path,
                        help="folder containing original .bik files")
    parser.add_argument("--max-lsb-difference", type=int, default=1,
                        help="maximum allowed absolute PCM16 sample difference")
    parser.add_argument("--ffmpeg", default=None,
                        help="FFmpeg executable (defaults to PATH)")
    args = parser.parse_args()

    native = args.native.resolve()
    asset_dir = args.asset_dir.resolve()
    ffmpeg = args.ffmpeg or shutil.which("ffmpeg")
    if not native.is_file():
        parser.error(f"native audit executable not found: {native}")
    if not asset_dir.is_dir():
        parser.error(f"asset directory not found: {asset_dir}")
    if ffmpeg is None:
        parser.error("ffmpeg is required as an independent reference decoder")
    if args.max_lsb_difference < 0:
        parser.error("--max-lsb-difference must be nonnegative")

    files = sorted(
        p for p in asset_dir.rglob("*")
        if p.is_file() and (
            p.name.lower().endswith(".bik") or
            p.name.lower().endswith(".bik.bin")
        )
    )
    if not files:
        parser.error(f"no .bik or .bik.bin files found under {asset_dir}")

    passed = failed = skipped = tracks_checked = 0
    with tempfile.TemporaryDirectory(prefix="bink-real-audio-parity-") as temp:
        temp_root = Path(temp)
        for file_index, source in enumerate(files):
            try:
                data, track_count = read_source(source)
            except (OSError, ValueError) as exc:
                print(f"SKIP {source.name}: {exc}", flush=True)
                skipped += 1
                continue
            if track_count == 0:
                skipped += 1
                continue

            # The compatibility signature exists only in this temporary copy.
            patched = bytearray(data)
            patched[:4] = b"BIKf"
            ffmpeg_source = temp_root / f"{file_index:04d}_{source.stem}.bik"
            ffmpeg_source.write_bytes(patched)

            for track_index in range(track_count):
                label = f"{source.name} track {track_index}"
                reference_path = temp_root / f"{file_index:04d}_{track_index}.ffmpeg.pcm"
                native_path = temp_root / f"{file_index:04d}_{track_index}.native.pcm"
                try:
                    run_checked([
                        ffmpeg, "-nostdin", "-hide_banner", "-loglevel", "error",
                        "-f", "bink", "-i", str(ffmpeg_source),
                        "-map", f"0:a:{track_index}", "-f", "s16le",
                        "-y", str(reference_path),
                    ], f"FFmpeg reference for {label}")
                    run_checked([
                        str(native), str(source), str(track_index),
                        "--output-s16le", str(native_path),
                    ], f"native decoder for {label}")
                    reference = pcm16(reference_path)
                    candidate = pcm16(native_path)
                except (OSError, ValueError, RuntimeError) as exc:
                    print(f"FAIL {label}: {exc}", flush=True)
                    failed += 1
                    tracks_checked += 1
                    continue

                tracks_checked += 1
                if len(reference) != len(candidate):
                    print(
                        f"FAIL {label}: sample-count mismatch "
                        f"FFmpeg={len(reference)} native={len(candidate)}",
                        flush=True,
                    )
                    failed += 1
                    continue
                max_diff = max(
                    (abs(a - b) for a, b in zip(reference, candidate)),
                    default=0,
                )
                rmse = math.sqrt(
                    sum((a - b) * (a - b)
                        for a, b in zip(reference, candidate)) /
                    max(1, len(reference))
                )
                mismatches = sum(a != b for a, b in zip(reference, candidate))
                status = "PASS" if max_diff <= args.max_lsb_difference else "FAIL"
                print(
                    f"{status} {label}: {len(reference)} PCM16 values; "
                    f"max LSB diff={max_diff}; differing={mismatches}; "
                    f"RMSE={rmse:.8g}",
                    flush=True,
                )
                if status == "PASS":
                    passed += 1
                else:
                    failed += 1

    print(
        f"Original-file Bink PCM16 comparison: {passed} passed, "
        f"{failed} failed, {skipped} video-only/unsupported files skipped, "
        f"{tracks_checked} audio tracks checked."
    )
    return 0 if tracks_checked and failed == 0 else 1


if __name__ == "__main__":
    sys.exit(main())

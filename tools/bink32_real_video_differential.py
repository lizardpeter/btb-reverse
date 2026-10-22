#!/usr/bin/env python3
"""Compare the native SJPi video decoder against FFmpeg on original BIK files.

The source files are never modified. FFmpeg does not recognize the game's
"SJPi" signature, so only a temporary reference copy has its first four bytes
changed to "BIKf". The native video audit always reads the untouched original.

Examples:
  python tools/bink32_real_video_differential.py \
      --native build/reconstruction/btb_bink32_retail_video_audit \
      --asset-dir "C:/path/to/BTB-BBP"
  python tools/bink32_real_video_differential.py \
      --native build/reconstruction/btb_bink32_retail_video_audit \
      --asset-dir "C:/path/to/BTB-BBP" --max-frames 20
"""
from __future__ import annotations

import argparse
import shutil
import struct
import subprocess
import sys
import tempfile
from pathlib import Path

MAX_FILE_BYTES = 100_000_000


def read_source(path: Path) -> tuple[bytes, int, int, int, bool]:
    data = path.read_bytes()
    if len(data) < 44 or len(data) > MAX_FILE_BYTES:
        raise ValueError(f"file size outside bounds: {len(data)} bytes")
    if data[:4] != b"SJPi":
        raise ValueError("not a supported original SJPi movie")
    if struct.unpack_from("<I", data, 4)[0] != len(data) - 8:
        raise ValueError("file-size field does not match original bytes")
    frame_count = struct.unpack_from("<I", data, 8)[0]
    width, height = struct.unpack_from("<II", data, 20)
    flags = struct.unpack_from("<I", data, 36)[0]
    if not frame_count or not width or not height or width > 640 or height > 480:
        raise ValueError(f"unsupported source geometry/timing: {width}x{height}, {frame_count} frames")
    return data, frame_count, width, height, bool(flags & 0x00100000)


def run(command: list[str]) -> subprocess.CompletedProcess[str]:
    return subprocess.run(command, capture_output=True, text=True)


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--native", required=True, type=Path,
                        help="built btb_bink32_retail_video_audit executable")
    parser.add_argument("--asset-dir", required=True, type=Path,
                        help="folder containing original .bik files")
    parser.add_argument("--max-frames", type=int, default=None,
                        help="check only the first N frames of each movie")
    parser.add_argument("--max-files", type=int, default=None,
                        help="check only the first N sorted movies")
    parser.add_argument("--ffmpeg", default=None,
                        help="FFmpeg executable (defaults to PATH)")
    args = parser.parse_args()

    native = args.native.resolve()
    asset_dir = args.asset_dir.resolve()
    ffmpeg = args.ffmpeg or shutil.which("ffmpeg")
    if not native.is_file():
        parser.error(f"native video audit executable not found: {native}")
    if not asset_dir.is_dir():
        parser.error(f"asset directory not found: {asset_dir}")
    if ffmpeg is None:
        parser.error("ffmpeg is required as an independent reference decoder")
    if args.max_frames is not None and args.max_frames < 1:
        parser.error("--max-frames must be positive")
    if args.max_files is not None and args.max_files < 1:
        parser.error("--max-files must be positive")

    files = sorted(
        p for p in asset_dir.rglob("*")
        if p.is_file() and (
            p.name.lower().endswith(".bik") or
            p.name.lower().endswith(".bik.bin")
        )
    )
    if args.max_files is not None:
        files = files[:args.max_files]
    if not files:
        parser.error(f"no .bik or .bik.bin files found under {asset_dir}")

    passed = failed = skipped = 0
    with tempfile.TemporaryDirectory(prefix="bink-real-video-parity-") as temp:
        temp_root = Path(temp)
        for file_index, source in enumerate(files):
            try:
                data, frame_count, width, height, alpha = read_source(source)
            except (OSError, ValueError) as exc:
                print(f"SKIP {source.name}: {exc}", flush=True)
                skipped += 1
                continue

            checked_frames = min(frame_count, args.max_frames or frame_count)
            patched = bytearray(data)
            patched[:4] = b"BIKf"
            ffmpeg_source = temp_root / f"{file_index:04d}_{source.stem}.bik"
            reference = temp_root / f"{file_index:04d}.reference.yuva"
            ffmpeg_source.write_bytes(patched)
            pixel_format = "yuva420p" if alpha else "yuv420p"
            ffmpeg_command = [
                ffmpeg, "-nostdin", "-hide_banner", "-loglevel", "error",
                "-f", "bink", "-i", str(ffmpeg_source),
                "-map", "0:v:0", "-pix_fmt", pixel_format,
                "-fps_mode", "passthrough",
            ]
            if args.max_frames is not None:
                ffmpeg_command.extend(["-frames:v", str(checked_frames)])
            ffmpeg_command.extend(["-f", "rawvideo", "-y", str(reference)])
            result = run(ffmpeg_command)
            if result.returncode:
                detail = (result.stderr or result.stdout).strip()
                print(f"FAIL {source.name}: FFmpeg reference failed: {detail[-1200:]}",
                      flush=True)
                failed += 1
                continue

            native_command = [str(native), str(source), "--reference-yuva", str(reference)]
            if args.max_frames is not None:
                native_command.extend(["--max-frames", str(checked_frames)])
            result = run(native_command)
            if result.returncode:
                lines = (result.stdout + "\n" + result.stderr).strip().splitlines()
                detail = "\n".join(lines[-12:])
                print(
                    f"FAIL {source.name} ({width}x{height}, "
                    f"{'alpha' if alpha else 'opaque'}, {checked_frames} frames):\n{detail}",
                    flush=True,
                )
                failed += 1
                continue

            summary = next(
                (line for line in result.stdout.splitlines()
                 if line.startswith("SUMMARY ")),
                "native audit returned success",
            )
            print(
                f"PASS {source.name}: {width}x{height}, "
                f"{'alpha' if alpha else 'opaque'}, {checked_frames} frames; {summary}",
                flush=True,
            )
            passed += 1

    print(
        f"Original-file video pixel comparison: {passed} passed, {failed} failed, "
        f"{skipped} unsupported files skipped."
    )
    return 0 if passed and failed == 0 else 1


if __name__ == "__main__":
    sys.exit(main())

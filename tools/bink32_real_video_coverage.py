#!/usr/bin/env python3
"""Run native SJPi decode coverage on original BIKs without modifying them.

This is a coverage check only. A successful run means every requested frame
was accepted by the native decoder; it does NOT itself perform pixel parity.
Use bink32_real_video_differential.py for the independent FFmpeg comparison;
that tool uses a temporary SJPi-to-BIKi signature normalization.
"""
from __future__ import annotations

import argparse
import struct
import subprocess
import sys
from pathlib import Path

MAX_FILE_BYTES = 100_000_000


def header(path: Path) -> tuple[bytes, int, int, int, bool]:
    size = path.stat().st_size
    if size < 44 or size > MAX_FILE_BYTES:
        raise ValueError(f"file size outside bounds: {size} bytes")
    with path.open("rb") as stream:
        data = stream.read(44)
    if data[:4] != b"SJPi":
        raise ValueError("unsupported container signature (expected SJPi)")
    if struct.unpack_from("<I", data, 4)[0] != size - 8:
        raise ValueError("header file-size field does not match source bytes")
    frames = struct.unpack_from("<I", data, 8)[0]
    width, height = struct.unpack_from("<II", data, 20)
    flags = struct.unpack_from("<I", data, 36)[0]
    if not frames or not width or not height or width > 640 or height > 480:
        raise ValueError(f"unsupported geometry/frame count: {width}x{height}, {frames}")
    return data, frames, width, height, bool(flags & 0x00100000)


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
    args = parser.parse_args()

    native = args.native.resolve()
    asset_dir = args.asset_dir.resolve()
    if not native.is_file():
        parser.error(f"native video audit executable not found: {native}")
    if not asset_dir.is_dir():
        parser.error(f"asset directory not found: {asset_dir}")
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

    passed = failed = skipped = total_frames = 0
    for source in files:
        try:
            _, frame_count, width, height, alpha = header(source)
        except (OSError, ValueError) as exc:
            print(f"SKIP {source.name}: {exc}", flush=True)
            skipped += 1
            continue

        count = min(frame_count, args.max_frames or frame_count)
        command = [str(native), str(source)]
        if args.max_frames is not None:
            command.extend(["--max-frames", str(count)])
        result = subprocess.run(command, capture_output=True, text=True)
        if result.returncode:
            lines = (result.stdout + "\n" + result.stderr).strip().splitlines()
            detail = "\n".join(lines[-12:])
            print(
                f"FAIL {source.name} ({width}x{height}, "
                f"{'alpha' if alpha else 'opaque'}, {count} frames):\n{detail}",
                flush=True,
            )
            failed += 1
            continue

        total_frames += count
        passed += 1
        print(
            f"PASS {source.name}: {width}x{height}, "
            f"{'alpha' if alpha else 'opaque'}, decoded {count}/{frame_count} frames; "
            "pixel parity not checked",
            flush=True,
        )

    print(
        f"Native SJPi frame coverage: {passed} movies passed, {failed} failed, "
        f"{skipped} unsupported files skipped; {total_frames} frames decoded. "
        "This is not a retail pixel-parity certification."
    )
    return 0 if passed and failed == 0 else 1


if __name__ == "__main__":
    sys.exit(main())

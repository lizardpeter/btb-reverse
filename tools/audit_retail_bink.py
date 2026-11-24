#!/usr/bin/env python3
"""Read-only whole-installation Bink inventory and native decoding audit.

No original media is copied or distributed. The native audit executables are
optional for inventory-only mode and mandatory for a complete decode report.
A successful decode is not a pixel/PCM equivalence claim.
"""
from __future__ import annotations

import argparse
import hashlib
import json
from pathlib import Path
import re
import struct
import subprocess
import sys
from typing import Any

MAGICS = {b"BIKf", b"BIKg", b"BIKh", b"SJPi"}
MAX_FRAMES = 1_000_000
MAX_TRACKS = 64
MAX_REPORT_OUTPUT = 8192


def inventory_header(path: Path) -> dict[str, Any]:
    """Validate the same prefix/index invariants as bink32_container.cpp.

    Reads at most the metadata and frame index, not the movie payload.
    """
    size = path.stat().st_size
    if size < 44 or size > 0xFFFFFFFF:
        raise ValueError("invalid file length")
    with path.open("rb") as f:
        prefix = f.read(44)
        if len(prefix) != 44 or prefix[:4] not in MAGICS:
            raise ValueError("unrecognized original Bink/SJPi magic")
        length, frames, max_frame, reserved, w, h, rate, divisor, flags, tracks = struct.unpack(
            "<10I", prefix[4:]
        )
        if length != size - 8 or not (0 < frames <= MAX_FRAMES) or not all((w, h, rate, divisor)):
            raise ValueError("invalid prefix dimensions, frame count, rate, or declared length")
        if tracks > MAX_TRACKS:
            raise ValueError("too many declared tracks")
        index_start = 44 + 12 * tracks
        data_start = index_start + 4 * (frames + 1)
        if data_start > size:
            raise ValueError("truncated frame index")
        track_data = f.read(12 * tracks)
        if len(track_data) != 12 * tracks:
            raise ValueError("truncated track table")
        audio = [
            {"index": i, "max_decoded_bytes": maximum, "codec_flags": codec,
             "track_id": track_id}
            for i, (maximum, codec, track_id) in enumerate(struct.iter_unpack("<III", track_data))
        ]
        offsets = f.read(4 * (frames + 1))
        if len(offsets) != 4 * (frames + 1):
            raise ValueError("truncated frame index")
        prev = 0
        keys = 0
        for i, (entry,) in enumerate(struct.iter_unpack("<I", offsets)):
            offset = entry & ~3
            if entry & 2 or offset > size or (i == 0 and offset < data_start) or offset < prev:
                raise ValueError(f"invalid frame-index entry {i}")
            if i == frames:
                if entry != size:
                    raise ValueError("terminal frame offset differs from file length")
            else:
                keys += entry & 1
            prev = offset
    return {
        "magic": prefix[:4].decode("ascii"), "file_bytes": size,
        "frames": frames, "maximum_frame_bytes": max_frame,
        "width": w, "height": h, "frame_rate": rate, "divisor": divisor,
        "video_flags": flags, "keyframes": keys, "audio_tracks": audio,
    }


def digest(path: Path) -> str:
    sha = hashlib.sha256()
    with path.open("rb") as f:
        for block in iter(lambda: f.read(1024 * 1024), b""):
            sha.update(block)
    return sha.hexdigest()


def run_tool(command: list[str], timeout: float) -> dict[str, Any]:
    try:
        result = subprocess.run(command, check=False, stdout=subprocess.PIPE,
                                stderr=subprocess.STDOUT, timeout=timeout)
        output = result.stdout.decode("utf-8", errors="replace")
        return {"status": "passed" if result.returncode == 0 else "failed",
                "exit_code": result.returncode, "output_tail": output[-MAX_REPORT_OUTPUT:]}
    except subprocess.TimeoutExpired:
        return {"status": "timeout", "timeout_seconds": timeout}
    except OSError as exc:
        return {"status": "failed", "error": str(exc)}


def audit(root: Path, video_tool: Path | None, audio_tool: Path | None,
          timeout: float, max_frames: int | None) -> dict[str, Any]:
    if not root.is_dir():
        raise ValueError("root is not an installed-game directory")
    movies = sorted((p for p in root.rglob("*") if p.is_file() and p.suffix.casefold() == ".bik"),
                    key=lambda p: (p.relative_to(root).as_posix().casefold(), p.as_posix()))
    results = []
    seen: dict[str, str] = {}
    failed = 0
    for path in movies:
        relative = path.relative_to(root).as_posix()
        item: dict[str, Any] = {"path": relative}
        try:
            item["header"] = header = inventory_header(path)
            item["sha256"] = sha = digest(path)
            if sha in seen:
                item["identical_to"] = seen[sha]
            else:
                seen[sha] = relative
            # Decode EVERY path; report aliases without silently assuming
            # that a successful copy proves another path was accessible.
            if video_tool:
                command = [str(video_tool), str(path)]
                if max_frames is not None:
                    command += ["--max-frames", str(max_frames)]
                item["video"] = run_tool(command, timeout)
                if item["video"]["status"] == "passed":
                    summary = re.search(r"SUMMARY decoded=(\d+)/(\d+) frames", item["video"]["output_tail"])
                    expected = min(max_frames, header["frames"]) if max_frames is not None else header["frames"]
                    if not summary or tuple(map(int, summary.groups())) != (expected, expected):
                        item["video"]["status"] = "failed"
                        item["video"]["error"] = "native tool omitted the expected complete decode count"
            else:
                item["video"] = {"status": "not_run"}
            item["audio"] = [
                {"index": t["index"], **(run_tool([str(audio_tool), str(path), str(t["index"])], timeout)
                                        if audio_tool else {"status": "not_run"})}
                for t in header["audio_tracks"]
            ]
            for track in item["audio"]:
                if track["status"] == "passed":
                    summary = re.search(r"Native Bink1 audio structural audit: (\d+)/(\d+) packets",
                                        track["output_tail"])
                    pcm = re.search(r"PCM16 validated bytes=(\d+)", track["output_tail"])
                    if not summary or tuple(map(int, summary.groups())) != (header["frames"], header["frames"]) or not pcm:
                        track["status"] = "failed"
                        track["error"] = "native tool omitted complete packet/PCM16 validation"
                    else:
                        track["pcm16_bytes"] = int(pcm.group(1))
            item["status"] = ("failed" if item["video"]["status"] in {"failed", "timeout"} or
                              any(a["status"] in {"failed", "timeout"} for a in item["audio"])
                              else "passed" if video_tool and (audio_tool or not item["audio"]) and
                              (max_frames is None or max_frames >= header["frames"])
                              else "partial")
        except (OSError, ValueError, struct.error) as exc:
            item["status"] = "failed"
            item["error"] = str(exc)
        failed += item["status"] == "failed"
        results.append(item)
    return {
        "schema": 1,
        "scope": "native decode diagnostics; not retail pixel, PCM or playback equivalence",
        "root": str(root),
        "movie_count": len(results), "unique_valid_movies": len(seen),
        "fully_decoded_paths": sum(m["status"] == "passed" for m in results),
        "partial_paths": sum(m["status"] == "partial" for m in results),
        "failed_paths": failed,
        "movies": results,
    }


def main(argv: list[str] | None = None) -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("root", type=Path)
    parser.add_argument("--video-tool", type=Path)
    parser.add_argument("--audio-tool", type=Path)
    parser.add_argument("--max-frames", type=int)
    parser.add_argument("--timeout", type=float, default=1800)
    parser.add_argument("--require-complete", action="store_true",
                        help="fail if any discovered movie is not fully decoded")
    parser.add_argument("--expected-count", type=int,
                        help="fail unless this many movie paths were discovered")
    parser.add_argument("--out", type=Path, required=True,
                        help="JSON report outside the original installation")
    args = parser.parse_args(argv)
    if (args.max_frames is not None and args.max_frames < 1) or args.timeout <= 0:
        parser.error("positive frame limit and timeout required")
    if args.expected_count is not None and args.expected_count < 1:
        parser.error("expected count must be positive")
    if args.require_complete and not args.video_tool:
        parser.error("--require-complete requires --video-tool")
    if args.out.resolve().is_relative_to(args.root.resolve()):
        parser.error("write reports outside the original game installation")
    for tool in (args.video_tool, args.audio_tool):
        if tool is not None and not tool.is_file():
            parser.error(f"native tool not found: {tool}")
    report = audit(args.root.resolve(),
                   args.video_tool.resolve() if args.video_tool else None,
                   args.audio_tool.resolve() if args.audio_tool else None,
                   args.timeout, args.max_frames)
    report["complete_decode"] = (report["movie_count"] > 0 and
                                 report["fully_decoded_paths"] == report["movie_count"])
    report["expected_count"] = args.expected_count
    report["inventory_count_matches"] = (None if args.expected_count is None else
                                         report["movie_count"] == args.expected_count)
    report["require_complete"] = args.require_complete
    report["video_frames_validated"] = sum(
        m["header"]["frames"] for m in report["movies"]
        if m["status"] == "passed")
    report["audio_tracks_validated"] = sum(
        a["status"] == "passed" for m in report["movies"] for a in m.get("audio", []))
    report["native_tools_sha256"] = {
        name: digest(tool.resolve()) for name, tool in
        (("video", args.video_tool), ("audio", args.audio_tool)) if tool is not None}
    args.out.parent.mkdir(parents=True, exist_ok=True)
    args.out.write_text(json.dumps(report, indent=2) + "\n", encoding="utf-8")
    print(f'{report["movie_count"]} paths, {report["fully_decoded_paths"]} fully decoded, '
          f'{report["partial_paths"]} partial, {report["failed_paths"]} failed')
    if report["inventory_count_matches"] is False:
        print("Discovered movie count differs from expected inventory.", file=sys.stderr)
    if args.require_complete and not report["complete_decode"]:
        print("Complete native decode gate failed.", file=sys.stderr)
    return int(not report["movie_count"] or bool(report["failed_paths"]) or
               report["inventory_count_matches"] is False or
               (args.require_complete and not report["complete_decode"]))


if __name__ == "__main__":
    sys.exit(main())

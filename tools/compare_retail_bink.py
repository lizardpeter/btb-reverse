#!/usr/bin/env python3
"""Compare installed original Bink streams to an independent FFmpeg decoder.

The original tree is read-only. Raw reference YUV(A) and PCM are temporary,
never shipped. FFmpeg is a diagnostic dependency, not a native game dependency.
"""
from __future__ import annotations

import argparse
from array import array
import json
from pathlib import Path
import re
import shutil
import subprocess
import sys
import tempfile

from audit_retail_bink import digest, inventory_header


def compare_pcm(reference: Path, native: Path) -> dict:
    sizes = reference.stat().st_size, native.stat().st_size
    if sizes[0] % 2 or sizes[1] % 2 or sizes[0] != sizes[1]:
        raise ValueError(f"PCM16 byte count mismatch or unaligned output: {sizes}")
    changed = 0
    maximum = 0
    square_error = 0
    with reference.open('rb') as ref, native.open('rb') as actual:
        while (block := ref.read(128 * 1024)):
            a, b = array('h'), array('h')
            a.frombytes(block)
            b.frombytes(actual.read(len(block)))
            if sys.byteorder != 'little':
                a.byteswap()
                b.byteswap()
            for x, y in zip(a, b):
                delta = abs(x - y)
                maximum = max(maximum, delta)
                changed += delta != 0
                square_error += delta * delta
    return {'samples': sizes[0] // 2, 'changed_samples': changed,
            'max_abs_lsb': maximum, 'sum_squared_error': square_error}


def execute(command: list[str], timeout: float) -> str:
    result = subprocess.run(command, stdout=subprocess.PIPE, stderr=subprocess.STDOUT,
                            timeout=timeout, text=True, errors='replace')
    if result.returncode:
        raise ValueError(f"{Path(command[0]).name} exited {result.returncode}: {result.stdout[-2048:]}")
    return result.stdout


def compare_movie(path: Path, header: dict, video: Path, audio: Path,
                  ffmpeg: str, timeout: float, temporary: Path) -> dict:
    result = {'video_frames': 0, 'audio': []}
    raw = temporary / 'reference.yuv'
    ref = temporary / 'reference.pcm'
    candidate = temporary / 'native.pcm'
    # SJPi is accepted by FFmpeg's Bink demuxer. Do not patch the retail file.
    command = [ffmpeg, '-nostdin', '-v', 'error', '-xerror', '-f', 'bink', '-i', str(path)]
    alpha = bool(header['video_flags'] & 0x100000)
    execute(command + ['-map', '0:v:0', '-pix_fmt', 'yuva420p' if alpha else 'yuv420p',
                       '-f', 'rawvideo', '-y', str(raw)], timeout)
    output = execute([str(video), str(path), '--reference-yuva', str(raw)], timeout)
    match = re.search(r'SUMMARY decoded=(\d+)/(\d+) frames, (\d+) reference-mismatched frames '
                      r'Y_mismatches=(\d+) U_mismatches=(\d+) V_mismatches=(\d+) A_mismatches=(\d+)', output)
    if not match or tuple(map(int, match.groups())) != (header['frames'], header['frames'], 0, 0, 0, 0, 0):
        raise ValueError('native video omitted complete exact reference comparison')
    result['video_frames'] = header['frames']
    raw.unlink()
    for track in header['audio_tracks']:
        i = track['index']
        execute(command + ['-map', f'0:a:{i}', '-f', 's16le', '-y', str(ref)], timeout)
        execute([str(audio), str(path), str(i), '--output-s16le', str(candidate)], timeout)
        comparison = compare_pcm(ref, candidate)
        if comparison['max_abs_lsb'] > 1:
            raise ValueError(f'PCM16 track {i}: more than one LSB difference')
        result['audio'].append({'track': i, **comparison})
        ref.unlink()
        candidate.unlink()
    return result


def main(argv: list[str] | None = None) -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('root', type=Path)
    parser.add_argument('--video-tool', required=True, type=Path)
    parser.add_argument('--audio-tool', required=True, type=Path)
    parser.add_argument('--ffmpeg', default='ffmpeg')
    parser.add_argument('--timeout', type=float, default=1800)
    parser.add_argument('--expected-count', type=int)
    parser.add_argument('--out', required=True, type=Path)
    args = parser.parse_args(argv)
    root = args.root.resolve()
    if not root.is_dir() or args.out.resolve().is_relative_to(root):
        parser.error('valid original directory and report path outside it required')
    if args.timeout <= 0 or (args.expected_count is not None and args.expected_count < 1):
        parser.error('positive timeout and expected count required')
    ffmpeg = shutil.which(args.ffmpeg)
    if ffmpeg is None or not args.video_tool.is_file() or not args.audio_tool.is_file():
        parser.error('both native audit tools and independent FFmpeg are required')
    movies = sorted((p for p in root.rglob('*') if p.is_file() and p.suffix.casefold() == '.bik'),
                    key=lambda p: p.relative_to(root).as_posix().casefold())
    rows = []
    for path in movies:
        row = {'path': path.relative_to(root).as_posix()}
        try:
            row['sha256'] = digest(path)
            header = inventory_header(path)
            with tempfile.TemporaryDirectory(prefix='btb-bink-compare-') as directory:
                row.update(compare_movie(path, header, args.video_tool.resolve(), args.audio_tool.resolve(),
                                         ffmpeg, args.timeout, Path(directory)))
            row['status'] = 'passed'
        except (OSError, ValueError, subprocess.TimeoutExpired) as exc:
            row['status'] = 'failed'
            row['error'] = str(exc)
        rows.append(row)
        print(f"{row['status']}: {row['path']}", flush=True)
    passed = [row for row in rows if row['status'] == 'passed']
    tracks = [a for row in passed for a in row['audio']]
    report = {'schema': 1, 'scope': 'independent FFmpeg decoded pixels and PCM16; not physical playback',
              'ffmpeg': execute([ffmpeg, '-version'], args.timeout).splitlines()[0],
              'native_tools_sha256': {'video': digest(args.video_tool), 'audio': digest(args.audio_tool)},
              'movie_count': len(rows), 'passed': len(passed), 'failed': len(rows)-len(passed),
              'expected_count': args.expected_count, 'video_frames': sum(r['video_frames'] for r in passed),
              'audio_tracks': len(tracks), 'pcm16_samples': sum(t['samples'] for t in tracks),
              'pcm16_changed_samples': sum(t['changed_samples'] for t in tracks),
              'max_abs_lsb': max((t['max_abs_lsb'] for t in tracks), default=0), 'movies': rows}
    args.out.parent.mkdir(parents=True, exist_ok=True)
    args.out.write_text(json.dumps(report, indent=2) + '\n', encoding='utf-8')
    print({k: v for k, v in report.items() if k != 'movies'})
    return int(not rows or len(passed) != len(rows) or
               (args.expected_count is not None and len(rows) != args.expected_count))


if __name__ == '__main__':
    raise SystemExit(main())

#!/usr/bin/env python3
"""Independent, asset-free Bink1 candidate audio PCM differential tests.

Builds original SJPi-format BIKs from synthetic audio bitstreams only, then
compares decoded native candidate float samples against the separately
installed FFmpeg Bink1 reference. No original game assets, RAD libraries
or FFmpeg dependency are added to the native decoder.
"""
from __future__ import annotations

import argparse
import math
from pathlib import Path
import shutil
import struct
import subprocess
import tempfile

CRITICAL_HZ = (
    100, 200, 300, 400, 510, 630, 770, 920, 1080, 1270, 1480, 1720,
    2000, 2320, 2700, 3150, 3700, 4400, 5300, 6400, 7700, 9500,
    12000, 15500, 24500,
)


class BitWriter:
    def __init__(self) -> None:
        self.bits: list[int] = []

    def put(self, value: int, count: int) -> None:
        self.bits.extend((value >> i) & 1 for i in range(count))

    def align32(self) -> None:
        while len(self.bits) % 32:
            self.put(0, 1)

    def data(self) -> bytes:
        return bytes(
            sum(self.bits[i + j] << j
                for j in range(min(8, len(self.bits) - i)))
            for i in range(0, len(self.bits), 8)
        )


def movie(path: Path, rate: int, channels: int, dct: bool, blocks: int, loud: bool) -> None:
    bits = 9 if rate < 22050 else 10 if rate < 44100 else 11
    if not dct and channels == 2:
        bits += 1
    n = 1 << bits
    rate_half = (rate * (1 if dct else channels) + 1) // 2
    bands = next((i for i, hz in enumerate(CRITICAL_HZ, 1)
                  if rate_half <= hz), 25)
    w = BitWriter()
    for block in range(blocks):
        if dct:
            w.put(0, 2)
        for channel in range(channels if dct else 1):
            # DC and AC use different quantizations. Include signed values,
            # nonzero widths, a zero-run, two blocks and stereo variants.
            w.put(18 if loud else 23, 5)
            w.put((32768 + block * 4096 + channel * 2048)
                  if loud else (1 + block + channel), 23)
            w.put((block + channel) % 2, 1)
            w.put(16 if loud else 22, 5)
            w.put(8192 if loud else 2, 23)
            w.put(0, 1)
            for band in range(bands):
                w.put(60 if (loud and band == 0) else
                      (10 if band == 0 else 0), 8)
            for start in range(2, n, 8):
                w.put(0, 1)
                width = 3 if start == 2 else 0
                w.put(width, 4)
                if width:
                    for j in range(min(8, n - start)):
                        magnitude = j % 5
                        w.put(magnitude, width)
                        if magnitude:
                            w.put(j % 2, 1)
        w.align32()
    bitstream = w.data()
    declared_pcm_bytes = (n - n // 16) * channels * 2 * blocks
    audio = struct.pack("<I", declared_pcm_bytes) + bitstream
    # One little-endian audio payload length precedes each track. The video
    # image is intentionally dummy data; tests map ONLY stream 0:a:0.
    frame = struct.pack("<I", len(audio)) + audio + bytes(4)
    frame += bytes((-len(frame)) % 4)
    header = bytearray(64)
    header[:4] = b"SJPi"
    for where, val in (
        (8, 1), (12, len(frame)), (16, 1),
        (20, 640), (24, 480), (28, 15), (32, 1),
        (36, 0x100000), (40, 1), (44, 65536),
        (52, 71), (56, 65), (60, len(header) + len(frame)),
    ):
        struct.pack_into("<I", header, where, val)
    flags = (0x5000 if dct else 0x4000) | (0x2000 if channels == 2 else 0)
    struct.pack_into("<I", header, 48, (flags << 16) | rate)
    bik = header + frame
    struct.pack_into("<I", bik, 4, len(bik) - 8)
    path.write_bytes(bik)


def signed16_samples(path: Path) -> tuple[int, ...]:
    data = path.read_bytes()
    if not data or len(data) % 2:
        raise AssertionError(f"Invalid raw s16 output: {path}")
    return struct.unpack(f"<{len(data) // 2}h", data)


def float_samples(path: Path) -> tuple[float, ...]:
    data = path.read_bytes()
    if not data or len(data) % 4:
        raise AssertionError(f"Invalid raw f32 output: {path}")
    result = struct.unpack(f"<{len(data) // 4}f", data)
    if not all(math.isfinite(v) for v in result):
        raise AssertionError(f"Nonfinite audio data: {path}")
    return result


def run(native: Path, ffmpeg: str) -> None:
    cases = [
        ("bink_dct_mono", 11025, 1, True, 2, False),
        ("bink_rdft_mono", 11025, 1, False, 2, False),
        ("bink_dct_stereo", 44100, 2, True, 1, False),
        ("bink_rdft_stereo", 44100, 2, False, 1, False),
        ("bink_dct_mono_loud", 11025, 1, True, 2, True),
        ("bink_rdft_mono_loud", 11025, 1, False, 2, True),
        ("bink_dct_stereo_loud", 44100, 2, True, 1, True),
        ("bink_rdft_stereo_loud", 44100, 2, False, 1, True),
    ]
    with tempfile.TemporaryDirectory(prefix="bink1-audio-parity-") as temp:
        root = Path(temp)
        for name, rate, channels, dct, blocks, loud in cases:
            source = root / (name + ".bik")
            expected = root / (name + ".ffmpeg.f32")
            actual = root / (name + ".native.f32")
            expected_s16 = root / (name + ".ffmpeg.s16")
            actual_s16 = root / (name + ".native.s16")
            movie(source, rate, channels, dct, blocks, loud)
            subprocess.run(
                [ffmpeg, "-nostdin", "-hide_banner", "-loglevel", "error",
                 "-f", "bink", "-i", str(source), "-map", "0:a:0",
                 "-f", "f32le", "-y", str(expected)],
                check=True,
            )
            subprocess.run(
                [str(native), str(source), "0", "--output-f32", str(actual)],
                check=True,
            )
            reference, candidate = float_samples(expected), float_samples(actual)
            if len(reference) != len(candidate):
                raise AssertionError(
                    f"{name}: sample count mismatch: FFmpeg={len(reference)}, "
                    f"native={len(candidate)}"
                )
            max_difference = max(abs(a - b) for a, b in zip(reference, candidate))
            rmse = math.sqrt(sum((a - b) ** 2
                                 for a, b in zip(reference, candidate))
                             / len(reference))
            print(
                f"{name}: {len(reference)} float samples, "
                f"max abs diff={max_difference:.10g}, RMSE={rmse:.10g}",
                flush=True,
            )
            # Float FFT backends need not agree bit-for-bit. Nevertheless
            # this is <0.2% of the strongest synthetic signals, sufficient
            # to catch missing/incorrect normalization or coefficient signs.
            if max_difference > (1.0e-5 if loud else 2.0e-7):
                raise AssertionError(
                    f"{name}: native float audio differs from FFmpeg "
                    f"beyond bounded transform tolerance"
                )
            subprocess.run(
                [ffmpeg, "-nostdin", "-hide_banner", "-loglevel", "error",
                 "-f", "bink", "-i", str(source), "-map", "0:a:0",
                 "-f", "s16le", "-y", str(expected_s16)],
                check=True,
            )
            subprocess.run(
                [str(native), str(source), "0", "--output-s16le",
                 str(actual_s16)],
                check=True,
            )
            ref_pcm, native_pcm = signed16_samples(expected_s16), signed16_samples(actual_s16)
            if len(ref_pcm) != len(native_pcm):
                raise AssertionError(f"{name}: signed PCM count mismatch")
            pcm_max = max(abs(a - b) for a, b in zip(ref_pcm, native_pcm))
            print(f"{name}: signed PCM16 max LSB difference={pcm_max}", flush=True)
            if pcm_max > 1:
                raise AssertionError(
                    f"{name}: native signed PCM16 differs from FFmpeg by >1 LSB")
            if loud and max(abs(x) for x in ref_pcm) < 100:
                raise AssertionError(f"{name}: loud signal not actually audible")
    print("All eight synthetic Bink1 native-vs-FFmpeg F32 and PCM16 comparisons passed.")


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--native", required=True, type=Path,
                        help="Path to btb_bink32_native_audio_audit executable")
    arguments = parser.parse_args()
    native = arguments.native.resolve()
    ffmpeg = shutil.which("ffmpeg")
    if not native.is_file():
        parser.error(f"Missing built native audio audit: {native}")
    if ffmpeg is None:
        parser.error("ffmpeg required only for independent CI comparison")
    run(native, ffmpeg)


if __name__ == "__main__":
    main()

#!/usr/bin/env python3
"""Test the installed-data diagnostic with metadata only, never original art.

The BMP dimensions were read from headers in the matching retail installation.
These generated 26-byte files are deliberately not usable bitmap images.
"""
import pathlib
import struct
import subprocess
import sys
import tempfile

GOLF = {
    'Clown.bmp': (1648, 129), 'Flag.bmp': (1485, 160),
    'Windmill.bmp': (1935, 164), 'flag01.bmp': (3762, 165),
    'golfballstop.bmp': (18, 18), 'golfbg.bmp': (640, 480),
    'golfnumbers.bmp': (150, 32), 'golfsprite_8bit.bmp': (2128, 7830),
    'marker.bmp': (44, 44), 'powerbar.bmp': (257, 53), 'shadow.bmp': (10, 10),
    'spud.bmp': (9700, 100), 'toolbar ball.bmp': (18, 18), 'wendy.bmp': (8800, 130),
}
SQUIRREL = {
    'all.bmp': (3293, 140), 'allconveyor.bmp': (3293, 140),
    'body_01.bmp': (294, 156), 'bottom.bmp': (600, 80),
    'conveyor.bmp': (9600, 44), 'left.bmp': (20, 460),
    'level1.bmp': (620, 480), 'level2.bmp': (1240, 480),
    'level3.bmp': (1860, 480), 'nutpile.bmp': (89, 21),
    'right.bmp': (20, 460), 'top.bmp': (640, 20), 'eyes.bmp': (3968, 35),
    'hook.bmp': (48, 446), 'loftyarmswing_8bit.bmp': (6789, 192),
    'mouth.bmp': (360, 47), 'squriel_1_86.bmp': (8772, 92),
}
GOLF_DATA = r"""98 195
data\\subgamegolf\\golfsprite_8bit.bmp
45
159 184
16
91 140
330 178
data\\subgamegolf\\flag.bmp
99 160
22 143
15
3
230 15
data\\subgamegolf\\windmill.bmp
129 164
62 151
15
3
429 31
data\\subgamegolf\\clown.bmp
103 129
33 117
15
3
data\\subgamegolf\\wendy.bmp
20 105
88 130
100
data\\subgamegolf\\spud.bmp
92 20
97 100
100
data\\subgamegolf\\wendy.bmp
53 54
85 118
100
5 4 3
2 4 6
10 10
1
105 247
304 155
//Comments
"""
SQUIRREL_DATA = """0 3 10 27 8 -4 -4
0 0 1 -1 0 0 0
0 0 0 0 0 0 0
0 0 1 -17 0 0 0
0 0 1 -33 0 0 0
0 0 -1 17 0 0 0
0 0 19 13 0 0 0
0 0 0 -32 0 0 0
0 0 0 32 0 0 0
7 105 79
1 2 2 3
70
110 0 234 146 384 177 362 203 172 0
"""


def write_header(path, width, height):
    header = bytearray(26)
    header[:2] = b"BM"
    struct.pack_into("<III", header, 2, 26, 0, 54)
    struct.pack_into("<Iii", header, 14, 40, width, height)
    path.write_bytes(header)


def run(exe, root, activity):
    return subprocess.run([str(exe), str(root), activity, "1"], text=True,
                          capture_output=True, timeout=180)


def main():
    exe = pathlib.Path(sys.argv[1]).resolve()
    with tempfile.TemporaryDirectory(prefix="btb-metadata-only-") as temp:
        root = pathlib.Path(temp)
        golf = root / "data" / "subgamegolf"
        squirrel = root / "data" / "subgamesquirrel"
        music = root / "data" / "music"
        for directory in (golf, squirrel, music):
            directory.mkdir(parents=True)
        for folder, dimensions in ((golf, GOLF), (squirrel, SQUIRREL)):
            for name, (width, height) in dimensions.items():
                write_header(folder / name, width, height)
        (golf / "golfdata.txt").write_text(GOLF_DATA)
        (squirrel / "sqdata.txt").write_text(SQUIRREL_DATA)
        # Only path resolution is checked; these are not PCM WAV files.
        for name in ("golf.wav", "squirrelrun.wav"):
            (music / name).touch()
        result = run(exe, root, "both")
        print("METADATA-ONLY FIXTURE, not real artwork or physical playback")
        print(result.stdout, end="")
        if result.returncode or result.stdout.count("completion=1") != 12:
            raise RuntimeError(result.stderr or "incomplete diagnostic")
        # Verify failures cannot accidentally be counted as successful play.
        (golf / "marker.bmp").unlink()
        result = run(exe, root, "golf")
        if result.returncode == 0 or "marker.bmp" not in result.stderr:
            raise RuntimeError("missing bitmap was not rejected")
        write_header(golf / "marker.bmp", 44, 44)
        write_header(squirrel / "level1.bmp", 619, 480)
        result = run(exe, root, "squirrel")
        if result.returncode == 0 or "invalid DirectDraw source crop" not in result.stderr:
            raise RuntimeError("undersized source bitmap was not rejected")


if __name__ == "__main__":
    main()

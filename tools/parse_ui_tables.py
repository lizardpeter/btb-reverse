#!/usr/bin/env python3
"""Parse Bob Builds a Park's plaintext front-end UI tables into JSON.

The original game data is supplied separately and is never written to the repository.
This tool preserves raw records while exposing the ordered screen names, hot-area
counts, bitmap names, replacement sections, and help-screen names.
"""

from __future__ import annotations

import argparse
import json
import re
from pathlib import Path


INT_LINE = re.compile(r"^\s*-?\d+(?:\s+-?\d+)*\s*$")
SCREEN_PREFIX = re.compile(r"^\s*-1\s+(.+?)\s*$")


def read_lines(path: Path) -> list[str]:
    return [line.rstrip("\r\n") for line in path.read_text(encoding="latin-1").splitlines()]


def parse_bitmap_names(path: Path) -> list[str]:
    out: list[str] = []
    for line in read_lines(path):
        s = line.strip()
        if not s:
            continue
        if s.upper() == "END.BMP":
            break
        out.append(s)
    return out


def parse_counts(path: Path) -> list[int]:
    out: list[int] = []
    for line in read_lines(path):
        s = line.strip()
        if not s:
            continue
        value = int(s)
        if value == -1:
            break
        out.append(value)
    return out


def parse_hot_area_sections(path: Path, expected_counts: list[int]) -> list[dict]:
    lines = [x.strip() for x in read_lines(path) if x.strip()]
    sections: list[dict] = []
    i = 0
    screen_index = 0

    while i < len(lines) and screen_index < len(expected_counts):
        header = lines[i]
        if header.startswith("-99"):
            break
        if INT_LINE.match(header):
            i += 1
            continue

        first_action = None
        screen_name = header
        if "---" in header:
            screen_name, first_action = header.split("---", 1)
            screen_name = screen_name.strip("- ")
            first_action = first_action.strip("- ") or None
        i += 1

        expected = expected_counts[screen_index]
        actions: list[str | None] = []
        records: list[list[str]] = []
        if first_action:
            actions.append(first_action)

        current: list[str] = []
        while i < len(lines) and len(records) < expected:
            line = lines[i]
            if line.startswith("-99"):
                break
            if not INT_LINE.match(line) and not line.startswith("-1 -1"):
                break
            current.append(line)
            if line.startswith("-1 -1"):
                parts = line.split(maxsplit=2)
                action = parts[2] if len(parts) > 2 else None
                records.append(current)
                current = []
                if action:
                    actions.append(action)
            i += 1

        sections.append({
            "index": screen_index,
            "name": screen_name,
            "expected_hot_area_count": expected,
            "parsed_record_count": len(records),
            "actions": actions,
            "records": records,
        })
        screen_index += 1

    return sections


def parse_replace_sections(path: Path) -> list[dict]:
    sections: list[dict] = []
    current: dict | None = None
    for raw in read_lines(path):
        line = raw.strip()
        if not line:
            continue
        if line == "-1 End":
            break
        match = SCREEN_PREFIX.match(line)
        if match:
            current = {"name": match.group(1), "records": []}
            sections.append(current)
            continue
        if current is None and not INT_LINE.match(line):
            current = {"name": line, "records": []}
            sections.append(current)
            continue
        if current is not None:
            current["records"].append(line)
    return sections


def parse_help_sections(path: Path) -> list[dict]:
    sections: list[dict] = []
    current: dict | None = None
    for raw in read_lines(path):
        line = raw.strip()
        if not line:
            continue
        if line.upper() == "END":
            break
        if not INT_LINE.match(line):
            current = {"name": line, "records": []}
            sections.append(current)
            continue
        if current is not None:
            current["records"].append([int(x) for x in line.split()])
    return sections


def main() -> int:
    ap = argparse.ArgumentParser()
    ap.add_argument("game_root", type=Path, help="Root containing the original loaddata directory")
    ap.add_argument("--out", type=Path, required=True)
    args = ap.parse_args()

    load = args.game_root / "loaddata"
    required = {
        "bitmap_names": load / "uiBitmapName.txt",
        "counts": load / "NumUiHotArea.txt",
        "hot_areas": load / "uiHotArea.txt",
        "replacements": load / "uiHotAreaReplace.txt",
        "help": load / "helpinfo.txt",
    }
    missing = [str(path) for path in required.values() if not path.is_file()]
    if missing:
        raise SystemExit("Missing required UI table(s):\n  " + "\n  ".join(missing))

    counts = parse_counts(required["counts"])
    result = {
        "bitmap_names": parse_bitmap_names(required["bitmap_names"]),
        "hot_area_counts": counts,
        "hot_area_screens": parse_hot_area_sections(required["hot_areas"], counts),
        "replacement_screens": parse_replace_sections(required["replacements"]),
        "help_screens": parse_help_sections(required["help"]),
    }

    args.out.parent.mkdir(parents=True, exist_ok=True)
    args.out.write_text(json.dumps(result, indent=2), encoding="utf-8")
    print(f"Wrote {len(result['hot_area_screens'])} UI screens to {args.out}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())

#!/usr/bin/env python3
"""Extract useful strings and path references from the game executable.

This is deliberately dependency-free so it can run before Ghidra or IDA is installed.
"""

from __future__ import annotations

import argparse
import json
import re
from pathlib import Path


ASCII = re.compile(rb"[\x20-\x7e]{5,}")
PATHISH = re.compile(
    r"(?i)(?:data|loaddata)[\\/][^\r\n\x00]+|[^\s\x00]+\.(?:txt|bmp|wav|bik|dll|ico)"
)


def ascii_strings(blob: bytes):
    for m in ASCII.finditer(blob):
        yield m.start(), m.group().decode("ascii", errors="replace")


def main() -> int:
    ap = argparse.ArgumentParser()
    ap.add_argument("exe", type=Path)
    ap.add_argument("--out", type=Path, required=True)
    args = ap.parse_args()

    blob = args.exe.read_bytes()
    strings = [{"file_offset": off, "text": text} for off, text in ascii_strings(blob)]

    paths = []
    seen = set()
    for item in strings:
        for match in PATHISH.finditer(item["text"]):
            value = match.group(0)
            key = value.lower().replace("/", "\\")
            if key not in seen:
                seen.add(key)
                paths.append({
                    "file_offset": item["file_offset"] + match.start(),
                    "text": value,
                })

    result = {
        "file": args.exe.name,
        "size": len(blob),
        "ascii_string_count": len(strings),
        "path_reference_count": len(paths),
        "path_references": paths,
        "strings": strings,
    }

    args.out.parent.mkdir(parents=True, exist_ok=True)
    args.out.write_text(json.dumps(result, indent=2), encoding="utf-8")
    print(f"Extracted {len(strings)} strings and {len(paths)} path-like references")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())

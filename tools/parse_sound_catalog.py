#!/usr/bin/env python3
"""Parse Data/sound/binklist.txt into a sound-ID catalog.

Despite its filename, this file is a WAV filename -> numeric sound-ID table.
"""

from __future__ import annotations

import argparse
import json
from pathlib import Path


def parse_catalog(text: str) -> list[dict]:
    result = []
    for line_number, raw in enumerate(text.splitlines(), 1):
        line = raw.strip()
        if not line:
            continue
        parts = line.split()
        if len(parts) < 2:
            continue
        try:
            sound_id = int(parts[-1])
        except ValueError:
            continue
        filename = " ".join(parts[:-1])
        result.append({
            "id": sound_id,
            "filename": filename,
            "line": line_number,
        })
    return result


def main() -> int:
    ap = argparse.ArgumentParser()
    ap.add_argument("catalog", type=Path)
    ap.add_argument("--out", type=Path)
    ap.add_argument("--min-id", type=int)
    ap.add_argument("--max-id", type=int)
    args = ap.parse_args()

    entries = parse_catalog(args.catalog.read_text(encoding="latin-1"))
    if args.min_id is not None:
        entries = [e for e in entries if e["id"] >= args.min_id]
    if args.max_id is not None:
        entries = [e for e in entries if e["id"] <= args.max_id]

    encoded = json.dumps(entries, indent=2)
    if args.out:
        args.out.parent.mkdir(parents=True, exist_ok=True)
        args.out.write_text(encoded + "\n", encoding="utf-8")
        print(f"Wrote {len(entries)} sound entries to {args.out}")
    else:
        print(encoded)
    return 0


if __name__ == "__main__":
    raise SystemExit(main())

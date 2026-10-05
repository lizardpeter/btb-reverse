#!/usr/bin/env python3
"""Parse Bob Builds a Park Fireworks editor geometry and optional firedata grid."""

from __future__ import annotations

import argparse
import json
import struct
from pathlib import Path


def ints_from_file(path: Path) -> list[int]:
    return [int(token, 10) for token in path.read_text(encoding="latin-1").split()]


def parse_rect_table(path: Path, count: int) -> list[dict]:
    values = ints_from_file(path)
    expected = count * 8
    if len(values) != expected:
        raise ValueError(
            f"{path}: expected {expected} integers ({count} records), got {len(values)}"
        )

    records = []
    for i in range(count):
        v = values[i * 8:(i + 1) * 8]
        records.append({
            "source_vertices": [
                [v[0], v[1]],
                [v[2], v[3]],
                [v[4], v[5]],
                [v[6], v[7]],
            ],
            # Retail fscanf passes the unused corners to scratch storage and
            # retains only integer pairs 1/2 and 5/6.
            "retained_rect": [v[0], v[1], v[4], v[5]],
        })
    return records


def parse_firedata(path: Path) -> list[int]:
    data = path.read_bytes()
    if len(data) != 18 * 4:
        raise ValueError(f"{path}: expected exactly 72 bytes, got {len(data)}")
    return list(struct.unpack("<18i", data))


def main() -> int:
    ap = argparse.ArgumentParser()
    ap.add_argument("game_root", type=Path)
    ap.add_argument("--firedata", type=Path)
    ap.add_argument("--out", type=Path, required=True)
    args = ap.parse_args()

    graph = args.game_root / "Data" / "SubGameFirework" / "graph"
    placement = parse_rect_table(graph / "fireworks.txt", 18)
    palette = parse_rect_table(graph / "fireworkboxes.txt", 12)

    interactive = []
    for i, record in enumerate(placement):
        l, t, r, b = record["retained_rect"]
        runtime_rect = [l - 20, t, r - 20, b]
        record["runtime_rect"] = runtime_rect
        interactive.append({"rect": runtime_rect, "action": 12, "placement_index": i})

    for type_id, record in enumerate(palette):
        interactive.append({
            "rect": record["retained_rect"],
            "action": type_id,
            "firework_type": type_id,
        })

    interactive.extend([
        {"rect": [292, 416, 346, 472], "action": 27, "role": "play"},
        {"rect": [102, 416, 156, 472], "action": 25, "role": "delete_all"},
        {"rect": [483, 416, 537, 472], "action": 26, "role": "delete_selected"},
    ])

    result = {
        "placement_records": placement,
        "palette_records": palette,
        "interactive_regions": interactive,
        "grid": {
            "rows": 3,
            "columns": 6,
            "slots": 18,
            "empty_value": -1,
            "retail_type_span": [1] * 12,
        },
    }

    if args.firedata:
        result["firedata"] = {
            "path": str(args.firedata),
            "values": parse_firedata(args.firedata),
        }

    args.out.parent.mkdir(parents=True, exist_ok=True)
    args.out.write_text(json.dumps(result, indent=2) + "\n", encoding="utf-8")
    print(f"Wrote Fireworks geometry to {args.out}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())

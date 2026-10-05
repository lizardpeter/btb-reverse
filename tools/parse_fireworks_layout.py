#!/usr/bin/env python3
from __future__ import annotations

import argparse
import json
from pathlib import Path


def read_ints(path: Path) -> list[int]:
    return [int(token, 10) for token in path.read_text(encoding="latin-1").split()]


def parse_rectangles(path: Path, count: int) -> list[dict]:
    values = read_ints(path)
    if len(values) != count * 8:
        raise ValueError(f"{path}: expected {count * 8} integers, got {len(values)}")

    out = []
    for i in range(count):
        p = values[i * 8:(i + 1) * 8]
        out.append({
            "left": p[0],
            "top": p[1],
            "right": p[4],
            "bottom": p[5],
            "vertices": [[p[0], p[1]], [p[2], p[3]], [p[4], p[5]], [p[6], p[7]]],
        })
    return out


def main() -> int:
    ap = argparse.ArgumentParser()
    ap.add_argument("game_root", type=Path)
    ap.add_argument("--out", type=Path, required=True)
    args = ap.parse_args()

    graph = args.game_root / "Data" / "SubGameFirework" / "graph"
    placement = parse_rectangles(graph / "fireworks.txt", 18)
    palette = parse_rectangles(graph / "fireworkboxes.txt", 12)

    runtime_placement = [
        {
            "left": r["left"] - 20,
            "top": r["top"],
            "right": r["right"] - 20,
            "bottom": r["bottom"],
        }
        for r in placement
    ]

    result = {
        "placement_source": placement,
        "placement_runtime": runtime_placement,
        "palette": palette,
    }
    args.out.parent.mkdir(parents=True, exist_ok=True)
    args.out.write_text(json.dumps(result, indent=2) + "\n", encoding="utf-8")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())

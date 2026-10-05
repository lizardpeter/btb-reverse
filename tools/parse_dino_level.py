#!/usr/bin/env python3
"""Parse a Bob Builds a Park Dinosaur dino.txt level file."""

from __future__ import annotations

import argparse
import json
from pathlib import Path


def parse_dino_text(text: str) -> dict:
    lines = [line.strip() for line in text.splitlines() if line.strip()]
    if not lines:
        raise ValueError("empty dino.txt")

    piece_count = int(lines[0])
    coordinates: list[list[int]] = []
    i = 1

    while i < len(lines):
        parts = lines[i].split()
        if len(parts) != 2:
            break
        x, y = map(int, parts)
        i += 1
        if (x, y) == (-1, -1):
            break
        coordinates.append([x, y])
    else:
        raise ValueError("missing -1 -1 coordinate terminator")

    permutation_tokens: list[int] = []
    for line in lines[i:]:
        permutation_tokens.extend(int(value) for value in line.split())

    required_coordinates = 2 * piece_count
    if len(coordinates) < required_coordinates:
        raise ValueError(
            f"expected at least {required_coordinates} coordinate pairs for "
            f"{piece_count} pieces, got {len(coordinates)}"
        )
    if len(permutation_tokens) != piece_count:
        raise ValueError(
            f"expected {piece_count} permutation entries, got {len(permutation_tokens)}"
        )

    expected_ids = set(range(piece_count))
    actual_ids = set(permutation_tokens)
    if actual_ids != expected_ids:
        raise ValueError(
            "piece permutation is not a permutation of 0..N-1: "
            f"expected {sorted(expected_ids)}, got {sorted(actual_ids)}"
        )

    extras = coordinates[required_coordinates:]

    return {
        "piece_count": piece_count,
        "target_positions": coordinates[:piece_count],
        "start_positions": coordinates[piece_count:required_coordinates],
        "extra_anchor_positions": extras,
        "primary_anchor": extras[0] if len(extras) >= 1 else None,
        "secondary_anchor": extras[1] if len(extras) >= 2 else None,
        "piece_permutation": permutation_tokens,
        "coordinate_pair_count_before_sentinel": len(coordinates),
    }


def main() -> int:
    ap = argparse.ArgumentParser()
    ap.add_argument("dino_txt", type=Path)
    ap.add_argument("--out", type=Path)
    args = ap.parse_args()

    result = parse_dino_text(args.dino_txt.read_text(encoding="latin-1"))
    encoded = json.dumps(result, indent=2)

    if args.out:
        args.out.parent.mkdir(parents=True, exist_ok=True)
        args.out.write_text(encoded + "\n", encoding="utf-8")
        print(f"Wrote {args.out}")
    else:
        print(encoded)

    return 0


if __name__ == "__main__":
    raise SystemExit(main())

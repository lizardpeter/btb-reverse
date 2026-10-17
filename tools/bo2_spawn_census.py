#!/usr/bin/env python3
"""Independent, read-only audit of T6 MapEnts exported by OpenAssetTools.

This script is intentionally standalone so a public GitHub-hosted runner can
verify retail entity counts without publishing the private Rust-test repo.
It does NOT execute retail GSC or claim that every spawn is active.
"""
from __future__ import annotations

import argparse
import collections
import json
import pathlib
import re
import sys

KV = re.compile(r'^\s*"([^"]+)"\s+"([^"]*)"\s*$')


def parse_entities(source: str) -> list[dict[str, str]]:
    rows = []
    entity = None
    for line in source.splitlines():
        line = line.strip()
        if line == "{":
            entity = {}
        elif line == "}":
            if entity is not None:
                rows.append(entity)
            entity = None
        elif entity is not None:
            match = KV.match(line)
            if match:
                entity[match.group(1)] = match.group(2)
    return rows


def classify(row: dict[str, str]) -> str | None:
    name = row.get("classname", "").lower()
    target = row.get("targetname", "").lower()
    if name == "mp_dm_spawn":
        return "ffa"
    if name == "mp_tdm_spawn":
        return "team"
    if target == "player_respawn_point":
        return "zombies_player"
    if (name.startswith("mp_") and "spawn" in name
            or name.startswith("info_player_")
            or "player" in target and "spawn" in target
            or name == "script_struct" and "spawn" in target):
        return "unclassified"
    return None


def valid_vec3(value: str | None) -> bool:
    if value is None:
        return False
    try:
        vals = [float(part) for part in value.split()]
    except ValueError:
        return False
    import math
    return len(vals) == 3 and all(map(math.isfinite, vals))


def main() -> None:
    parser = argparse.ArgumentParser()
    parser.add_argument("map_id")
    parser.add_argument("oat_output", type=pathlib.Path)
    parser.add_argument("json_output", type=pathlib.Path)
    opts = parser.parse_args()
    root = opts.oat_output
    files = sorted(
        path for path in root.rglob("*")
        if path.is_file()
        and path.suffix.lower() in {".map", ".ents"}
        and "map" in str(path.parent).lower()
        and path.stat().st_size <= 50_000_000
    )
    if not files:
        inventory = [str(path.relative_to(root))
                     for path in sorted(root.rglob("*")) if path.is_file()][:60]
        raise RuntimeError(f"No MapEnts output found for {opts.map_id}. Extracted files: {inventory}")

    report = {"map": opts.map_id, "sources": [], "totals": collections.Counter(),
              "coverage": "Authored MapEnt player starts only; dynamic GSC and spawn eligibility not evaluated"}
    for path in files:
        raw = path.read_text("utf-8", errors="replace")
        entities = parse_entities(raw)
        if not entities:
            continue
        found, invalid = [], []
        for index, entity in enumerate(entities):
            group = classify(entity)
            if group is None:
                continue
            record = {"entity_index": index, "group": group,
                      "classname": entity.get("classname", ""),
                      "targetname": entity.get("targetname", ""),
                      "origin": entity.get("origin"),
                      "angles": entity.get("angles", entity.get("angle")),
                      "source_fields": entity}
            if not valid_vec3(entity.get("origin")):
                invalid.append(record)
            else:
                found.append(record)
        counts = collections.Counter(row["group"] for row in found)
        unknown_classes = collections.Counter(row["classname"] for row in found
                                              if row["group"] == "unclassified")
        report["sources"].append({
            "export": str(path.relative_to(root)),
            "mapent_count": len(entities),
            "classifications": dict(sorted(counts.items())),
            "unclassified_classnames": dict(sorted(unknown_classes.items())),
            "located_spawn_candidates": found,
            "unlocated_spawn_candidates": invalid,
        })
        report["totals"]["entities"] += len(entities)
        report["totals"]["located"] += len(found)
        report["totals"]["unlocated"] += len(invalid)
        report["totals"].update(counts)
    if not report["sources"]:
        raise RuntimeError(f"{opts.map_id} has no parseable exported entity data from {files}")
    report["totals"] = dict(sorted(report["totals"].items()))
    opts.json_output.parent.mkdir(parents=True, exist_ok=True)
    opts.json_output.write_text(json.dumps(report, indent=2, sort_keys=True), encoding="utf-8")
    all_unknown = collections.Counter()
    for source in report["sources"]:
        all_unknown.update(source["unclassified_classnames"])
    print(json.dumps({"map": opts.map_id, **report["totals"],
                      "unclassified_by_class": dict(all_unknown.most_common(12))},
                     sort_keys=True))


if __name__ == "__main__":
    try:
        main()
    except Exception as exc:
        print(f"bo2-spawn-census: {type(exc).__name__}: {exc}", file=sys.stderr)
        sys.exit(1)

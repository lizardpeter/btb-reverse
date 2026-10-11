#!/usr/bin/env python3
"""Build an auditable, dependency-ranked C++26 reconstruction registry.

Input is ONLY the verified Ghidra corpus + explicitly reviewed status overlay.
Generated function metadata is not a substitute for reconstructed functions.
"""
from __future__ import annotations

import argparse
import csv
import io
import json
import re
from collections import Counter, defaultdict
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
GHIDRA = ROOT / "analysis/ghidra_main_exe"
REVIEWED = ROOT / "analysis/reconstruction/reviewed_functions.csv"
GENERATED = ROOT / "analysis/reconstruction"
HEADER = ROOT / "reconstruction/include/btb/original_function_registry.hpp"
EXPECTED_SHA = "c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05"

# Tentative ranges from docs/module-map.md; boundaries are not proof of ownership.
RANGES = (
    ("shared_platform", 0x401000, 0x409DFF),
    ("dinosaur", 0x409E00, 0x40AC8F),
    ("park_designer", 0x40AC90, 0x410FCF),
    ("fireworks", 0x410FD0, 0x41440F),
    ("golf", 0x4145F0, 0x417FFF),
    ("herding", 0x418000, 0x41A72F),
    ("maze", 0x41A730, 0x41DB1F),
    ("bobs_band", 0x41DB20, 0x420324),
    ("spud_maze", 0x420360, 0x4240FF),
    ("spud_skate", 0x424100, 0x424E1F),
    ("squirrel", 0x424E20, 0x42800F),
    ("shared_activity", 0x428010, 0x42A2BF),
    ("game_flow", 0x42A2C0, 0x42CD6B),
    ("profiles_and_ui", 0x42CD6C, 0x42EFFF),
    ("static_runtime", 0x42F000, 0x440000),
)
SUBSYSTEMS = ("unclassified",) + tuple(x[0] for x in RANGES)
STAGES = ("unreviewed", "under_analysis", "source_written", "compiles", "behavior_verified")
ADDRESS = re.compile(r"^[0-9a-fA-F]{8}$")
FUNC_FILE = re.compile(r"^[0-9a-fA-F]{8}\.c$")
STATUS_FIELDS = ["address", "stage", "cpp_source", "validation_test", "evidence", "notes"]


def read_csv(path: Path) -> list[dict[str, str]]:
    with path.open("r", encoding="utf-8", newline="") as handle:
        return list(csv.DictReader(handle))


def csv_bytes(fieldnames: list[str], rows: list[dict]) -> str:
    buffer = io.StringIO(newline="")
    writer = csv.DictWriter(buffer, fieldnames=fieldnames, lineterminator="\n")
    writer.writeheader()
    writer.writerows(rows)
    return buffer.getvalue()


def subsystem(address: int) -> str:
    for name, low, high in RANGES:
        if low <= address <= high:
            return name
    return "unclassified"


def normalized_address(value: str) -> str:
    value = value.strip().lower().removeprefix("0x")
    if not ADDRESS.fullmatch(value):
        raise ValueError("Invalid original function address: " + value)
    return value


def relative_source_exists(path: str) -> bool:
    if not path or Path(path).is_absolute() or ".." in Path(path).parts:
        return False
    return (ROOT / path).is_file()


def components(vertices: list[str], edges: dict[str, set[str]]) -> list[list[str]]:
    """Tarjan SCCs, stable reindexing by lowest address."""
    index = 0
    stack: list[str] = []
    active: set[str] = set()
    ids: dict[str, int] = {}
    low: dict[str, int] = {}
    result: list[list[str]] = []

    def visit(node: str) -> None:
        nonlocal index
        ids[node] = index
        low[node] = index
        index += 1
        stack.append(node)
        active.add(node)
        for target in sorted(edges[node]):
            if target not in ids:
                visit(target)
                low[node] = min(low[node], low[target])
            elif target in active:
                low[node] = min(low[node], ids[target])
        if low[node] == ids[node]:
            component = []
            while True:
                item = stack.pop()
                active.remove(item)
                component.append(item)
                if item == node:
                    break
            result.append(sorted(component))

    for vertex in vertices:
        if vertex not in ids:
            visit(vertex)
    return sorted(result, key=lambda x: x[0])


def run(check: bool) -> None:
    manifest = json.loads((GHIDRA / "manifest.json").read_text(encoding="utf-8"))
    if (manifest["expected_exe_sha256"] != EXPECTED_SHA or
            manifest["expected_exe_size"] != 311296):
        raise ValueError("Executable identity mismatch")
    raw = read_csv(GHIDRA / "functions.csv")
    if len(raw) != manifest["function_count"] or not raw:
        raise ValueError("Functions do not match Ghidra manifest")
    by_address: dict[str, dict] = {}
    for record in raw:
        address = normalized_address(record["address"])
        filename = record["file"].strip()
        if address in by_address or not FUNC_FILE.fullmatch(filename):
            raise ValueError("Duplicate function or unsafe source filename at " + address)
        path = GHIDRA / "functions" / filename
        if not path.is_file() or not path.read_bytes().startswith(b"/* Ghidra generated pseudocode."):
            raise ValueError("Missing Ghidra source: " + str(path))
        if record["decompiled"] != "1" or record["error"]:
            raise ValueError("Incomplete decompilation: " + address)
        by_address[address] = {
            "address": address,
            "name": record["name"],
            "body_bytes": int(record["body_bytes"]),
            "ghidra_callers": int(record["callers"]),
            "ghidra_callees": int(record["callees"]),
            "pseudocode": "analysis/ghidra_main_exe/functions/" + filename,
            "subsystem": subsystem(int(address, 16)),
        }

    reviewed = {}
    for record in read_csv(REVIEWED):
        address = normalized_address(record["address"])
        if address not in by_address or address in reviewed:
            raise ValueError("Invalid or duplicate reviewed function: " + address)
        stage = record["stage"]
        if stage not in STAGES:
            raise ValueError("Invalid stage at " + address + ": " + stage)
        if stage in ("source_written", "compiles", "behavior_verified"):
            if not relative_source_exists(record["cpp_source"]) or not record["evidence"].strip():
                raise ValueError("Source/evidence required for reviewed function " + address)
        if stage in ("compiles", "behavior_verified"):
            if not relative_source_exists(record["validation_test"]):
                raise ValueError("Validation test required for " + address)
        reviewed[address] = record

    addresses = sorted(by_address)
    internal: dict[str, set[str]] = {key: set() for key in addresses}
    external: dict[str, set[str]] = {key: set() for key in addresses}
    external_symbols = {row["address"].lower() for row in read_csv(GHIDRA / "external_symbols.csv")}
    edges_seen: set[tuple[str, str]] = set()
    for record in read_csv(GHIDRA / "callgraph.csv"):
        caller = normalized_address(record["caller"])
        target = record["callee"].strip().lower()
        if caller not in by_address or (caller, target) in edges_seen:
            if caller not in by_address:
                raise ValueError("Unknown caller " + caller)
            continue
        edges_seen.add((caller, target))
        if target.startswith("external:"):
            if target not in external_symbols:
                raise ValueError("Missing external symbol " + target)
            external[caller].add(target)
        else:
            target = normalized_address(target)
            if target not in by_address:
                raise ValueError("Unknown internal target " + target)
            internal[caller].add(target)
    if len(edges_seen) != manifest["callgraph_edges"]:
        raise ValueError("Callgraph edge count disagrees with Ghidra manifest")

    incoming = Counter(target for outputs in internal.values() for target in outputs)
    clusters = components(addresses, internal)
    group_for = {a: i for i, cluster in enumerate(clusters) for a in cluster}
    unresolved: dict[str, int] = {}
    for address in addresses:
        unresolved[address] = sum(
            reviewed.get(target, {}).get("stage", "unreviewed") not in
            ("source_written", "compiles", "behavior_verified")
            for target in internal[address] if target != address
        )

    status_fields = [
        "address", "symbol", "subsystem_tentative", "stage", "cpp_source",
        "validation_test", "evidence", "notes", "pseudocode",
        "direct_callers", "internal_callees", "external_callees",
        "unresolved_callees", "scc_group",
    ]
    statuses = []
    for address in addresses:
        data = by_address[address]
        override = reviewed.get(address, {})
        statuses.append({
            "address": "0x" + address.upper(),
            "symbol": data["name"],
            "subsystem_tentative": data["subsystem"],
            "stage": override.get("stage", "unreviewed"),
            "cpp_source": override.get("cpp_source", ""),
            "validation_test": override.get("validation_test", ""),
            "evidence": override.get("evidence", ""),
            "notes": override.get("notes", ""),
            "pseudocode": data["pseudocode"],
            "direct_callers": incoming[address],
            "internal_callees": len(internal[address]),
            "external_callees": len(external[address]),
            "unresolved_callees": unresolved[address],
            "scc_group": group_for[address],
        })

    # Dependency-first within each tentative subsystem; high fan-in helpers
    # have higher priority, but priority is not a fidelity/completeness claim.
    def score(row: dict) -> int:
        address = row["address"][2:].lower()
        return 10 * row["direct_callers"] + 2 * row["internal_callees"] + (
            5 if row["symbol"] and not row["symbol"].startswith(("FUN_", "Unwind@")) else 0
        ) - 3 * row["unresolved_callees"] - (len(clusters[group_for[address]]) - 1)

    queue_fields = ["rank", "score"] + status_fields
    ordered = sorted(statuses, key=lambda x: (
        x["stage"] in ("behavior_verified", "compiles", "source_written"),
        x["unresolved_callees"] != 0,
        -score(x), x["address"]))
    queue = [{"rank": i + 1, "score": score(item), **item} for i, item in enumerate(ordered)]
    stage_counts = Counter(r["stage"] for r in statuses)
    sub_counts = Counter(r["subsystem_tentative"] for r in statuses)
    scc_info = [{
        "id": index,
        "addresses": ["0x" + x.upper() for x in group],
        "recursive": len(group) > 1 or (len(group) == 1 and group[0] in internal[group[0]]),
        "subsystems": sorted({by_address[a]["subsystem"] for a in group}),
        "external_dependency_groups": sorted({
            group_for[target] for address in group for target in internal[address]
            if group_for[target] != index
        }),
    } for index, group in enumerate(clusters)]
    summary = {
        "schema_version": 1,
        "source_executable_sha256": EXPECTED_SHA,
        "meaning": "Inventory of Ghidra-discovered functions; not source reconstruction coverage.",
        "ghidra_function_count": len(addresses),
        "ghidra_decompiled_count": manifest["decompiled_count"],
        "reviewed_overlay_rows": len(reviewed),
        "stage_counts": {key: stage_counts[key] for key in STAGES},
        "tentative_subsystem_counts": dict(sorted(sub_counts.items())),
        "direct_internal_edges": sum(map(len, internal.values())),
        "direct_external_edges": sum(map(len, external.values())),
        "strongly_connected_groups": len(clusters),
        "recursive_groups": sum(x["recursive"] for x in scc_info),
        "scc_method": "Tarjan over verified direct calls; group IDs sorted by address.",
    }

    enum_for = {name: "".join(part.capitalize() for part in name.split("_")) for name in SUBSYSTEMS}
    cpp = [
        "// Generated by tools/build_reconstruction_index.py; DO NOT EDIT.",
        "// Ghidra identities are NOT C++ function implementations.",
        "#pragma once",
        "#include <array>",
        "#include <cstddef>",
        "#include <cstdint>",
        "#include <string_view>",
        "namespace btb::reconstruction {",
        "enum class OriginalSubsystem : std::uint8_t {",
        "    " + ", ".join(enum_for.values()),
        "};",
        "enum class ReconstructionStage : std::uint8_t {",
        "    " + ", ".join("".join(x.capitalize() for x in s.split("_")) for s in STAGES),
        "};",
        "struct OriginalFunction {",
        "    std::uint32_t address;",
        "    std::string_view name;",
        "    OriginalSubsystem subsystem;",
        "    ReconstructionStage stage;",
        "    std::uint32_t body_bytes;",
        "    std::uint32_t callers;",
        "    std::uint32_t callees;",
        "};",
        "inline constexpr std::array<OriginalFunction, %d> kOriginalFunctions{{" % len(statuses),
    ]
    for row in statuses:
        src = by_address[row["address"][2:].lower()]
        name = json.dumps(row["symbol"], ensure_ascii=True)
        enum = enum_for[src["subsystem"]]
        stage = "".join(x.capitalize() for x in row["stage"].split("_"))
        cpp.append("    {0x%sU, %s, OriginalSubsystem::%s, ReconstructionStage::%s, %dU, %dU, %dU}," %
                   (src["address"].upper(), name, enum, stage,
                    src["body_bytes"], src["ghidra_callers"], src["ghidra_callees"]))
    cpp.extend([
        "}};",
        "[[nodiscard]] constexpr const OriginalFunction* find_original_function(",
        "    std::uint32_t address) noexcept {",
        "    std::size_t lo = 0, hi = kOriginalFunctions.size();",
        "    while (lo < hi) {",
        "        const auto mid = lo + (hi - lo) / 2;",
        "        if (kOriginalFunctions[mid].address < address) lo = mid + 1;",
        "        else hi = mid;",
        "    }",
        "    return lo < kOriginalFunctions.size() &&",
        "        kOriginalFunctions[lo].address == address ? &kOriginalFunctions[lo] : nullptr;",
        "}",
        "} // namespace btb::reconstruction",
        "",
    ])
    outputs = {
        GENERATED / "function_status.csv": csv_bytes(status_fields, statuses),
        GENERATED / "work_queue.csv": csv_bytes(queue_fields, queue),
        GENERATED / "dependency_groups.json": json.dumps(scc_info, indent=2) + "\n",
        GENERATED / "summary.json": json.dumps(summary, indent=2) + "\n",
        HEADER: "\n".join(cpp),
    }
    for path, content in outputs.items():
        if check:
            if not path.exists() or path.read_text(encoding="utf-8") != content:
                raise ValueError("Generated index is stale: " + str(path.relative_to(ROOT)))
        else:
            path.parent.mkdir(parents=True, exist_ok=True)
            path.write_text(content, encoding="utf-8", newline="\n")
    print("Ghidra identities=%d, internal calls=%d, external calls=%d, SCC groups=%d; reviewed=%d; %s" %
          (len(addresses), summary["direct_internal_edges"], summary["direct_external_edges"],
           len(clusters), len(reviewed), "verified" if check else "generated"))


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("--check", action="store_true", help="fail if checked-in outputs are stale")
    args = parser.parse_args()
    run(check=args.check)

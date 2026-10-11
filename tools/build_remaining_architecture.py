#!/usr/bin/env python3
"""Map the final 224 original BTB functions and reconcile all 615 addresses.

Source provenance: verified original Win32 PE, Ghidra C-like pseudocode.
This tool produces *architecture evidence*, not reconstructed C++ methods.
"""
from __future__ import annotations

import argparse
import csv
import hashlib
import io
import json
import re
from collections import Counter, defaultdict
from pathlib import Path

import build_architecture_map as common

ROOT = common.ROOT
ARCH = common.ARCH
GHIDRA = common.GHIDRA
SHA = common.SOURCE_SHA
FINAL = ARCH / "architecture_224_selection.csv"
FIRST = ARCH / "architecture_200_selection.csv"
REVIEWED = ARCH / "reviewed_functions.csv"

BASE_COLS = common.COLS
EXTRA_COLS = (
    "mapping_cohort", "pseudocode_sha256", "runtime_function_kind",
    "explicit_cpp_source", "cpp_validation_test",
    "direct_cross_module_callees", "indirect_call_coverage",
)
COLS = [*BASE_COLS, *EXTRA_COLS]


def file_selection(path: Path) -> list[str]:
    result = [common.normalize(r["address"]) for r in common.read_csv(path)]
    if len(result) != len(set(result)):
        raise ValueError("Duplicate original function address in " + str(path))
    return result


def read_one_file(original: dict) -> tuple[str, str]:
    path = GHIDRA / "functions" / original["file"]
    raw = path.read_bytes()
    data = raw.decode("utf-8")
    address = common.normalize(original["address"])
    if SHA not in data or "address: " + address not in data.lower():
        raise ValueError("Ghidra provenance failure for " + address)
    return data, hashlib.sha256(raw).hexdigest()


def classify_runtime(name: str, module: str) -> str:
    if module != "static_runtime":
        return "game_or_platform_candidate"
    lower = name.lower()
    if lower.startswith(("unwind@", "catch@", "eh_", "__eh")):
        return "compiler_exception_cleanup_candidate"
    if lower.startswith(("crt_", "msvc_", "__", "_", "operator")):
        return "crt_or_compiler_helper_candidate"
    if lower.startswith(("thunk_", "import_")):
        return "import_forwarder_candidate"
    return "statically_linked_runtime_unknown"


def mapping_inputs() -> tuple[list[dict], list[str], list[str], dict]:
    manifest = json.loads((GHIDRA / "manifest.json").read_text(encoding="utf-8"))
    if manifest["expected_exe_sha256"] != SHA or manifest["function_count"] != 615:
        raise ValueError("Incorrect original executable identity")
    functions = common.read_csv(GHIDRA / "functions.csv")
    if len(functions) != 615:
        raise ValueError("Incomplete Ghidra source inventory")
    by_address = {common.normalize(f["address"]): f for f in functions}
    if len(by_address) != 615:
        raise ValueError("Duplicate Ghidra original address")
    previous = file_selection(FIRST)
    final = file_selection(FINAL)
    reviewed_rows = common.read_csv(REVIEWED)
    reviewed = {common.normalize(r["address"]): r for r in reviewed_rows}
    # Frozen architecture cohorts are independent of changing C++ coverage.
    initial_source = set(by_address) - set(previous) - set(final)
    if len(previous) != 200 or len(final) != 224 or len(initial_source) != 191:
        raise ValueError("Expected frozen 191/200/224 mapping cohorts")
    if set(previous) & set(final):
        raise ValueError("Architecture mapping cohorts overlap")
    if len(reviewed) != len(reviewed_rows):
        raise ValueError("Duplicate reviewed function address")
    if not initial_source.issubset(reviewed) or not set(reviewed).issubset(by_address):
        raise ValueError("Reviewed source inventory omitted baseline or added unknown address")
    return functions, previous, final, reviewed


def build():
    functions, previous, final, reviewed = mapping_inputs()
    previous_set, final_set = set(previous), set(final)
    symbols = {r["address"].lower(): r["namespace"] + "!" + r["name"]
               for r in common.read_csv(GHIDRA / "external_symbols.csv")}
    incoming, outgoing, external = defaultdict(set), defaultdict(set), defaultdict(set)
    for edge in common.read_csv(GHIDRA / "callgraph.csv"):
        caller = edge["caller"].lower()
        callee = edge["callee"].lower()
        if callee.startswith("external:"):
            external[caller].add(symbols.get(callee, callee))
        else:
            incoming[callee].add(caller)
            outgoing[caller].add(callee)

    rows = []
    xrefs = defaultdict(set)
    original_module = {}
    for original in functions:
        addr = common.normalize(original["address"])
        source, digest = read_one_file(original)
        module = common.module(addr)
        owner, basis, confidence = common.owner(
            original["name"], module, common.prototype(source))
        globs, assets, warnings, uncertain, arrays = common.evidence(source)
        prototype = common.prototype(source)
        cohort = ("architecture_batch_200" if addr in previous_set else
                  "architecture_batch_224" if addr in final_set else
                  "previous_source_written")
        source_record = reviewed.get(addr, {})
        for global_address in globs:
            xrefs[global_address].add(addr)
        original_module[addr] = module
        row = dict(
            address="0x" + addr, symbol=original["name"],
            prototype=prototype, module_candidate=module,
            cpp_owner_candidate=owner, owner_basis=basis,
            owner_confidence=confidence,
            receiver_evidence=(
                "explicit_thiscall" if "__thiscall" in prototype else "not_established"),
            source_stage=source_record.get("stage", "unreviewed"),
            body_bytes=original["body_bytes"],
            caller_count=str(len(incoming[addr])),
            internal_callee_count=str(len(outgoing[addr])),
            external_callee_count=str(len(external[addr])),
            observed_globals=";".join("0x" + g for g in globs),
            global_count=str(len(globs)),
            shared_global_count="0",
            assets=";".join(assets),
            array_access_evidence=";".join(arrays),
            decompiler_warnings=str(warnings),
            uncertainties=";".join(uncertain),
            caller_addresses=";".join("0x" + x for x in sorted(incoming[addr])),
            callee_addresses=";".join("0x" + x for x in sorted(outgoing[addr])),
            external_symbols=";".join(sorted(external[addr])),
            pseudocode="analysis/ghidra_main_exe/functions/" + original["file"],
            disclaimer="evidence_mapping_not_C++_implementation",
            mapping_cohort=cohort, pseudocode_sha256=digest,
            runtime_function_kind=classify_runtime(original["name"], module),
            explicit_cpp_source=source_record.get("cpp_source", ""),
            cpp_validation_test=source_record.get("validation_test", ""),
            direct_cross_module_callees="0",
            indirect_call_coverage="not_claimed",
        )
        rows.append(row)

    for r in rows:
        addr = common.normalize(r["address"])
        globs = [x.removeprefix("0x") for x in r["observed_globals"].split(";") if x]
        r["shared_global_count"] = str(sum(len(xrefs[g]) > 1 for g in globs))
        r["direct_cross_module_callees"] = str(sum(
            common.module(dep) != original_module[addr]
            for dep in outgoing[addr] if dep in original_module))

    xrefs_report = []
    for global_addr, fn_addresses in xrefs.items():
        grouped = sorted({original_module[a] for a in fn_addresses})
        xrefs_report.append(dict(
            original_global="0x" + global_addr,
            selected_functions=["0x" + a for a in sorted(fn_addresses)],
            function_count=len(fn_addresses), candidate_modules=grouped,
            crosses_modules=len(grouped) > 1, data_type="unknown",
            storage_owner="unverified", read_write_direction="not_inferred",
        ))
    xrefs_report.sort(key=lambda r: (-r["function_count"], r["original_global"]))

    final_rows = [r for r in rows if r["mapping_cohort"] == "architecture_batch_224"]
    if len(final_rows) != 224 or len(rows) != 615:
        raise ValueError("Mapping cohort violation")

    runtime = Counter(r["runtime_function_kind"] for r in final_rows)
    summary224 = dict(
        original_exe_sha256=SHA, architecture_mapped_this_batch=224,
        original_function_count=615, previous_source_written=191,
        previous_architecture_mapped=200,
        final_addresses_all_in_statically_linked_runtime=all(
            r["module_candidate"] == "static_runtime" for r in final_rows),
        runtime_function_kinds=dict(sorted(runtime.items())),
        direct_original_call_edges=sum(
            int(r["internal_callee_count"]) + int(r["external_callee_count"])
            for r in final_rows),
        entries_with_decompiler_warnings=sum(
            int(r["decompiler_warnings"]) > 0 for r in final_rows),
        original_thiscall_prototypes=sum(
            r["receiver_evidence"] == "explicit_thiscall" for r in final_rows),
        cpp_bodies_reconstructed_this_batch=0,
        limitation="Original-code architecture evidence; C++ owners and types unverified",
    )
    summary615 = dict(
        original_exe_sha256=SHA, ghidra_original_function_count=615,
        architecture_mapped=615, cohort_counts=dict(sorted(Counter(
            r["mapping_cohort"] for r in rows).items())),
        source_written=sum(r["source_stage"] == "source_written" for r in rows),
        architecture_only=sum(r["source_stage"] == "unreviewed" for r in rows),
        module_counts=dict(sorted(Counter(r["module_candidate"] for r in rows).items())),
        original_global_addresses=len(xrefs_report),
        original_function_global_reference_edges=sum(int(r["global_count"]) for r in rows),
        globals_used_by_multiple_functions=sum(r["function_count"] > 1 for r in xrefs_report),
        globals_referenced_across_modules=sum(r["crosses_modules"] for r in xrefs_report),
        observed_original_direct_call_edges=sum(
            int(r["internal_callee_count"]) + int(r["external_callee_count"])
            for r in rows),
        source_fingerprint_sha256_validated=615,
        cpp_bodies_reconstructed_this_batch=0,
        prototype_notes="Ghidra prototype, not original C++ signature",
        owner_notes="Tentative module and class candidates, not proven layouts",
        indirect_call_edges="not_measured",
    )
    header = (
        "#pragma once\n"
        "// Architecture EVIDENCE for 615 Ghidra original functions. "
        "Not 615 source implementations.\n"
        "#include <array>\n#include <cstdint>\n#include <string_view>\n"
        "namespace btb::evidence {\n"
        "struct OriginalArchitecture615 {\n"
        " std::uint32_t address;\n"
        " std::string_view original_name;\n"
        " std::string_view module_candidate;\n"
        " std::string_view owner_candidate;\n"
        " std::uint16_t caller_count;\n"
        " std::uint16_t global_references;\n"
        " bool has_source_mapping;\n"
        " bool original_thiscall;\n"
        "};\n"
        "inline constexpr std::array<OriginalArchitecture615,615> kOriginalArchitecture615{{\n"
    )
    for r in rows:
        header += ("  {" + r["address"] + "U," +
                   json.dumps(r["symbol"]) + "," +
                   json.dumps(r["module_candidate"]) + "," +
                   json.dumps(r["cpp_owner_candidate"]) + "," +
                   r["caller_count"] + "," + r["global_count"] + "," +
                   ("true" if r["source_stage"] == "source_written" else "false") + "," +
                   ("true" if r["receiver_evidence"] == "explicit_thiscall" else "false") +
                   "},\n")
    header += "}};\n} // namespace btb::evidence\n"

    docs = [
        "# Final 224 original-function architecture mappings",
        "",
        "**All 224 are original functions in the statically linked runtime region**.",
        "These map source evidence and dependency relationships only.",
        "No new C++ gameplay implementations are claimed by this batch.",
        "",
        "## Scope and provenance",
        "",
        "Original executable SHA-256: " + SHA,
        "",
        "- Original Ghidra pseudocode text is referenced for every address.",
        "- Original direct caller and callee edges include imported names.",
        "- Original DAT global references and their cross-module sharing are indexed.",
        "- Candidate class/module owners and the original call prototype are labeled tentative.",
        "- Original pseudocode SHA-256 hashes are recorded to detect corpus drift.",
        "- The original 191 source mappings remain preserved; newly reconstructed C++ models are tracked separately.",
        "",
        "## Runtime categories (name-derived, not guaranteed compiler provenance)",
        "",
        "| Candidate kind | Functions |",
        "|---|---:|",
    ]
    docs.extend("| " + name + " | " + str(count) + " |"
                for name, count in sorted(runtime.items()))
    docs.extend([
        "",
        "## Reproducible artifacts",
        "",
        "- analysis/reconstruction/architecture_224_selection.csv — frozen final 224 addresses.",
        "- analysis/reconstruction/architecture_224.csv — detailed 224-function report.",
        "- analysis/reconstruction/architecture_224_summary.json — 224-function audit.",
        "- analysis/reconstruction/architecture_615.csv — all 615 original functions reconciled.",
        "- analysis/reconstruction/architecture_615_summary.json — disjoint cohort / fidelity totals.",
        "- analysis/reconstruction/architecture_615_globals.json — full original global-reference census.",
        "- reconstruction/include/btb/original_architecture_615.hpp — constexpr metadata only.",
        "",
        "## Reconstruction boundary",
        "",
        "615/615 architecture-mapped does not mean 615/615 rewritten in C++.",
        f"Current source-written models: {summary615['source_written']}; remaining: {summary615['architecture_only']}.",
        "Frozen architecture cohorts are independent of reconstruction status.",
        "Recover runtime layouts, identify import thunks,",
        "and integrate original gameplay functions before claiming fidelity.",
        "",
    ])
    docs615 = [
        "# Unified architecture inventory: all 615 original functions",
        "",
        "Original executable SHA-256: " + SHA,
        "",
        "Each original function belongs to exactly one evidence cohort:",
        "",
        "- 191 functions formed the initial source-written baseline.",
        "- 200 functions were originally architecture-only in the first pass.",
        "- 224 functions were originally architecture-only in the final pass.",
        "",
        f"Current status: {summary615['source_written']} source-written models and "
        f"{summary615['architecture_only']} architecture-only functions.",
        "",
        "The full inventory keeps these statuses separate; no source-written",
        "function was promoted to behavior-verified by architecture analysis.",
        "",
        "Important global evidence: the original Ghidra DAT-address tokens are",
        "cross-linked without assuming they are class members, types, or writes.",
        "Original callgraph relationships include only direct observed targets.",
        "",
        "See analysis/reconstruction/architecture_615.csv and",
        "analysis/reconstruction/architecture_615_globals.json for all records.",
        "Next step: actual C++ class and field-layout recovery with cross-game",
        "behavior integration and original-executable differential validation.",
        "",
    ]
    return {
        ARCH / "architecture_224.csv": common.write_csv(COLS, final_rows),
        ARCH / "architecture_615.csv": common.write_csv(COLS, rows),
        ARCH / "architecture_224_summary.json": json.dumps(summary224, indent=2, sort_keys=True) + "\n",
        ARCH / "architecture_615_summary.json": json.dumps(summary615, indent=2, sort_keys=True) + "\n",
        ARCH / "architecture_615_globals.json": json.dumps(xrefs_report, indent=2) + "\n",
        ROOT / "docs/architecture-final-224.md": "\n".join(docs),
        ROOT / "docs/architecture-615-functions.md": "\n".join(docs615),
        ROOT / "reconstruction/include/btb/original_architecture_615.hpp": header,
    }


def run(check=False):
    outputs = build()
    for path, content in outputs.items():
        if check:
            if not path.is_file() or path.read_text(encoding="utf-8") != content:
                raise RuntimeError("Missing or stale architecture artifact: " + str(path))
        else:
            path.parent.mkdir(parents=True, exist_ok=True)
            path.write_text(content, encoding="utf-8")
    print("Validated" if check else "Generated", "224 final + 615 unified source-evidence architecture records")


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("--check", action="store_true")
    args = parser.parse_args()
    run(check=args.check)

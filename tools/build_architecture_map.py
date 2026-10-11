#!/usr/bin/env python3
"""Produce reproducible, evidence-only architecture mapping for 200 original functions.

This mapping does NOT mark C++ functions as source_written, compiled or verified.
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
ARCH = ROOT / "analysis/reconstruction"
SELECT = ARCH / "architecture_200_selection.csv"
SOURCE_SHA = "c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05"
RANGES = [
    ("shared_platform", 0x401000, 0x409dff), ("dinosaur", 0x409e00, 0x40ac8f),
    ("park_designer", 0x40ac90, 0x410fcf), ("fireworks", 0x410fd0, 0x41440f),
    ("golf", 0x414410, 0x417fff), ("herding", 0x418000, 0x41a72f),
    ("maze", 0x41a730, 0x41db1f), ("bobs_band", 0x41db20, 0x420324),
    ("spud_maze", 0x420360, 0x4240ff), ("spud_skate", 0x424100, 0x424e1f),
    ("squirrel", 0x424e20, 0x42800f), ("shared_activity", 0x428010, 0x42a2bf),
    ("game_flow", 0x42a2c0, 0x42cd6b), ("profiles_and_ui", 0x42cd6c, 0x42efff),
    ("static_runtime", 0x42f000, 0x440000),
]
COLS = [
    "address", "symbol", "prototype", "module_candidate", "cpp_owner_candidate",
    "owner_basis", "owner_confidence", "receiver_evidence", "source_stage",
    "body_bytes", "caller_count", "internal_callee_count", "external_callee_count",
    "observed_globals", "global_count", "shared_global_count", "assets",
    "array_access_evidence", "decompiler_warnings", "uncertainties",
    "caller_addresses", "callee_addresses", "external_symbols", "pseudocode",
    "disclaimer",
]

def read_csv(path):
    with Path(path).open(encoding="utf-8", newline="") as f:
        return list(csv.DictReader(f))

def write_csv(names, rows):
    buffer = io.StringIO(newline="")
    out = csv.DictWriter(buffer, fieldnames=names, lineterminator="\n")
    out.writeheader()
    out.writerows(rows)
    return buffer.getvalue()

def normalize(value):
    addr = value.lower().removeprefix("0x")
    if not re.fullmatch(r"[0-9a-f]{8}", addr):
        raise ValueError("Invalid original address: " + value)
    return addr

def module(addr):
    val = int(addr, 16)
    return next((name for name, lo, hi in RANGES if lo <= val <= hi), "unclassified")

def owner(name, mod, prototype):
    name = name.lower()
    explicit = [("csoundmanager_", "CSoundManager"), ("csound_", "CSound"),
                ("cwavefile_", "CWaveFile"), ("bitmapprinter", "BitmapPrinter"),
                ("displaymanager", "DisplayManager")]
    for prefix, candidate in explicit:
        if name.startswith(prefix):
            return candidate, "original_symbol_prefix", "medium"
    if mod in ("shared_platform", "shared_activity", "profiles_and_ui"):
        for token, candidate in [
            ("sound", "SoundSubsystem"), ("audio", "SoundSubsystem"),
            ("bitmap", "BitmapSurfaceSubsystem"), ("directdraw", "DisplaySubsystem"),
            ("surface", "DisplaySubsystem"), ("bink", "MovieSubsystem"),
            ("movie", "MovieSubsystem"), ("input", "InputSubsystem"),
            ("cursor", "InputSubsystem"), ("print", "PrintingSubsystem"),
            ("profile", "ProfilesSubsystem"), ("msvc", "MsvcStringSubsystem"),
            ("string", "MsvcStringSubsystem"),
        ]:
            if token in name:
                return candidate, "symbol_and_code_neighborhood", "low"
    default = {
        "dinosaur": "DinosaurActivityModule",
        "park_designer": "ParkDesignerModule",
        "fireworks": "FireworksModule",
        "golf": "GolfActivityModule",
        "herding": "HerdingActivityModule",
        "maze": "MazeActivityModule",
        "bobs_band": "BobsBandModule",
        "spud_maze": "SpudMazeActivityModule",
        "spud_skate": "SpudSkateActivityModule",
        "squirrel": "SquirrelActivityModule",
        "shared_activity": "SharedActivityModule",
        "game_flow": "GameRootModule",
        "profiles_and_ui": "ProfilesAndUIModule",
        "static_runtime": "MsvcRuntimeModule",
        "shared_platform": "SharedPlatformModule",
    }
    return default.get(mod, "UnresolvedModule"), "tentative_address_range", "low"

def evidence(source):
    globals_seen = sorted(set(x.lower() for x in
        re.findall(r"\b_?DAT_([a-fA-F0-9]{8})\b", source)))
    assets = sorted(set(re.findall(r"\bs_[A-Za-z0-9_]+_[0-9a-fA-F]{8}\b", source)))
    warnings = source.count("WARNING:")
    uncertain = sorted(p for p in (
        "UNRECOVERED_JUMPTABLE", "unaff_", "extraout_",
        "float10", "undefined4", "in_EAX", "in_ECX"
    ) if p in source)
    arrays = sorted(set(x for x in
        re.findall(r"\[[^\]\n]{1,80}\]", source) if
        re.search(r"0x[0-9a-fA-F]+|[0-9]{2,}", x)))[:12]
    return globals_seen, assets, warnings, uncertain, arrays

def prototype(source):
    match = re.search(
        r"(?m)^[ \t]*([^\n;{}]{1,240}\([^\n;{}]*\))[ \t]*(?:\n[ \t]*){1,3}\{",
        source, re.M)
    return " ".join(match.group(1).split()) if match else "UNRESOLVED"

def generate():
    manifest = json.loads((GHIDRA / "manifest.json").read_text(encoding="utf-8"))
    if manifest["expected_exe_sha256"] != SOURCE_SHA or manifest["function_count"] != 615:
        raise ValueError("Original EXE / Ghidra provenance mismatch")
    funcs = read_csv(GHIDRA / "functions.csv")
    by_addr = {normalize(f["address"]): f for f in funcs}
    if len(by_addr) != 615:
        raise ValueError("Unexpected function inventory size")
    selection = [normalize(f["address"]) for f in read_csv(SELECT)]
    if len(selection) != 200 or len(set(selection)) != 200 or any(a not in by_addr for a in selection):
        raise ValueError("Must select precisely 200 distinct original functions")
    stages = {normalize(f["address"]): f["stage"]
              for f in read_csv(ARCH / "reviewed_functions.csv")}
    external_labels = {f["address"].lower(): f["namespace"] + "!" + f["name"]
                       for f in read_csv(GHIDRA / "external_symbols.csv")}
    incoming, outgoing, external = defaultdict(set), defaultdict(set), defaultdict(set)
    for edge in read_csv(GHIDRA / "callgraph.csv"):
        a, b = edge["caller"].lower(), edge["callee"].lower()
        if b.startswith("external:"):
            external[a].add(external_labels.get(b, b))
        else:
            outgoing[a].add(b)
            incoming[b].add(a)

    interim = []
    global_xrefs = defaultdict(set)
    for addr in selection:
        f = by_addr[addr]
        source = (GHIDRA / "functions" / f["file"]).read_text(encoding="utf-8")
        if SOURCE_SHA not in source or ("address: " + addr) not in source.lower():
            raise ValueError("Source text provenance failure at " + addr)
        globals_seen, assets, warnings, uncertain, arrays = evidence(source)
        mod = module(addr)
        guessed, basis, confidence = owner(f["name"], mod, prototype(source))
        ptype = prototype(source)
        entry = dict(
            address="0x" + addr, symbol=f["name"], prototype=ptype,
            module_candidate=mod, cpp_owner_candidate=guessed, owner_basis=basis,
            owner_confidence=confidence,
            receiver_evidence="explicit_thiscall" if "__thiscall" in ptype else "not_established",
            source_stage=stages.get(addr, "unreviewed"),
            body_bytes=f["body_bytes"],
            caller_count=str(len(incoming[addr])),
            internal_callee_count=str(len(outgoing[addr])),
            external_callee_count=str(len(external[addr])),
            observed_globals=";".join("0x" + g for g in globals_seen),
            global_count=str(len(globals_seen)),
            shared_global_count="0",
            assets=";".join(assets),
            array_access_evidence=";".join(arrays),
            decompiler_warnings=str(warnings),
            uncertainties=";".join(uncertain),
            caller_addresses=";".join("0x" + a for a in sorted(incoming[addr])),
            callee_addresses=";".join("0x" + a for a in sorted(outgoing[addr])),
            external_symbols=";".join(sorted(external[addr])),
            pseudocode="analysis/ghidra_main_exe/functions/" + f["file"],
            disclaimer="evidence_mapping_not_C++_implementation",
        )
        interim.append(entry)
        for g in globals_seen:
            global_xrefs[g].add(addr)

    for item in interim:
        gs = [g.removeprefix("0x") for g in item["observed_globals"].split(";") if g]
        item["shared_global_count"] = str(sum(len(global_xrefs[g]) > 1 for g in gs))
    globals_report = [
        dict(original_global="0x" + g, selected_functions=["0x" + a for a in sorted(addrs)],
             selected_function_count=len(addrs),
             modules=sorted({module(a) for a in addrs}),
             cross_module=len({module(a) for a in addrs}) > 1,
             type_status="unresolved", read_write_status="not_inferred")
        for g, addrs in global_xrefs.items()
    ]
    globals_report.sort(key=lambda x: (-x["selected_function_count"], x["original_global"]))
    modules = Counter(f["module_candidate"] for f in interim)
    summary = dict(
        schema="btb_architecture_evidence_200_v1",
        original_sha256=SOURCE_SHA, architecture_mapped=200,
        total_ghidra_functions=615, existing_source_written=sum(s=="source_written" for s in stages.values()),
        game_platform_mapped=sum(f["module_candidate"]!="static_runtime" for f in interim),
        static_runtime_mapped=sum(f["module_candidate"]=="static_runtime" for f in interim),
        modules=dict(sorted(modules.items())),
        observed_distinct_globals=len(globals_report),
        globals_shared_within_selected=sum(x["selected_function_count"]>1 for x in globals_report),
        globals_crossing_modules=sum(x["cross_module"] for x in globals_report),
        functions_with_ghidra_warnings=sum(int(f["decompiler_warnings"])>0 for f in interim),
        direct_call_edges=sum(int(f["internal_callee_count"])+int(f["external_callee_count"]) for f in interim),
        source_recompiled_this_batch=0,
        confidence="hypothesis_and_original_evidence_only",
    )
    markdown = [
        "# Architecture map: 200 additional original BTB functions",
        "",
        "This is **architecture mapping**, NOT 200 recompiled C++ functions.",
        "All 181 previously unmapped game/platform functions and 19 referenced CRT",
        "functions were selected. Their original Ghidra text and direct-call",
        "relationships are indexed, but class names are hypotheses.",
        "",
        "Original EXE SHA-256: " + SOURCE_SHA,
        "",
        "## Files",
        "",
        "- analysis/reconstruction/architecture_200_selection.csv: frozen 200 addresses",
        "- analysis/reconstruction/architecture_200.csv: prototype, module, tentative C++ owner, original global/asset references, callers, callees and uncertainty",
        "- analysis/reconstruction/architecture_global_xrefs.json: original DAT-address sharing across functions, untyped",
        "- analysis/reconstruction/architecture_200_summary.json: audited totals",
        "- reconstruction/include/btb/original_architecture_map.hpp: C++26 METADATA ONLY",
        "",
        "## Candidate module distribution",
        "",
        "| Candidate original module | Count |",
        "|---|---:|",
    ]
    markdown.extend("| " + k + " | " + str(v) + " |"
                    for k, v in sorted(modules.items(), key=lambda x: (-x[1], x[0])))
    markdown.extend([
        "",
        "## Highest-call-impact original functions still requiring C++ reconstruction",
        "",
    ])
    for row in sorted(interim, key=lambda f: (-int(f["caller_count"]), -int(f["body_bytes"])))[:25]:
        markdown.append("- " + row["address"] + " " + row["symbol"] + " — " +
                        row["caller_count"] + " direct callers, candidate " + row["cpp_owner_candidate"])
    markdown.extend([
        "",
        "## Fidelity limitations",
        "",
        "- Ghidra C-style prototypes are not the original C++ declarations.",
        "- Module ownership based on address and names is tentative.",
        "- This does not invent receiver/class layouts.",
        "- Original DAT addresses are lexical references, not proven data types or read/write fields.",
        "- Indirect calls and callback targets are not recovered from the direct-call graph.",
        "- All previous source-written and verified stages are preserved unchanged.",
        "- No additional original functions were converted to executable C++ by this mapping.",
        "",
        "Next: resolve struct layouts, classify global writes, bind actual class",
        "receivers and convert dependencies into integrated source files.",
        "",
    ])
    header = (
        "#pragma once\n"
        "// BTB Ghidra architecture metadata ONLY; not compiled function implementations.\n"
        "#include <array>\n#include <cstdint>\n#include <string_view>\n"
        "namespace btb::evidence {\n"
        "struct FunctionArchitecture { std::uint32_t address; std::string_view symbol; "
        "std::string_view module_candidate; std::string_view cpp_owner_candidate; "
        "std::uint16_t globals; std::uint16_t callers; std::uint16_t callees; "
        "bool original_thiscall; };\n"
        "inline constexpr std::array<FunctionArchitecture,200> kOriginalArchitecture200{{\n"
    )
    for f in interim:
        header += (
            "  {" + f["address"] + "U," + json.dumps(f["symbol"]) + "," +
            json.dumps(f["module_candidate"]) + "," + json.dumps(f["cpp_owner_candidate"]) +
            "," + f["global_count"] + "," + f["caller_count"] + "," +
            str(int(f["internal_callee_count"]) + int(f["external_callee_count"])) +
            "," + ("true" if f["receiver_evidence"]=="explicit_thiscall" else "false") + "},\n"
        )
    header += "}};\n} // namespace btb::evidence\n"
    return {
        ARCH / "architecture_200.csv": write_csv(COLS, interim),
        ARCH / "architecture_global_xrefs.json": json.dumps(globals_report, indent=2) + "\n",
        ARCH / "architecture_200_summary.json": json.dumps(summary, indent=2, sort_keys=True) + "\n",
        ROOT / "docs/architecture-200-functions.md": "\n".join(markdown),
        ROOT / "reconstruction/include/btb/original_architecture_map.hpp": header,
    }

def run(check=False):
    outputs = generate()
    for path, content in outputs.items():
        if check:
            if not path.is_file() or path.read_text(encoding="utf-8") != content:
                raise RuntimeError("Stale architecture mapping file: " + str(path))
        else:
            path.parent.mkdir(parents=True, exist_ok=True)
            path.write_text(content, encoding="utf-8")
    print("Verified 200 original architecture mappings" if check
          else "Generated 200 original architecture mappings")

if __name__ == "__main__":
    p = argparse.ArgumentParser()
    p.add_argument("--check", action="store_true")
    args = p.parse_args()
    run(check=args.check)

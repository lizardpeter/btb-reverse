#!/usr/bin/env python3
"""Measure direct-call symbol coverage for the Bob Builds a Park executable.

This intentionally measures *internal direct call targets*, not source-line
coverage. It is useful for identifying frequently-called unnamed helpers and
for separating game logic from static CRT/STL runtime code.
"""

from __future__ import annotations

import argparse
import csv
import re
import subprocess
from collections import Counter
from pathlib import Path

CALL_RE = re.compile(r"\bcall\s+0x([0-9a-fA-F]+)\b")


def load_known_symbols(path: Path) -> dict[int, str]:
    result: dict[int, str] = {}
    with path.open("r", encoding="utf-8", newline="") as f:
        for row in csv.DictReader(f):
            address = row.get("address", "").strip()
            name = row.get("name", "").strip()
            if not address or not name:
                continue
            result[int(address, 16)] = name
    return result


def collect_direct_calls(
    exe: Path,
    objdump: str,
    low: int,
    high: int,
) -> Counter[int]:
    proc = subprocess.run(
        [objdump, "-d", "-Mintel", str(exe)],
        check=True,
        capture_output=True,
        text=True,
        errors="replace",
    )

    calls: Counter[int] = Counter()
    for line in proc.stdout.splitlines():
        match = CALL_RE.search(line)
        if not match:
            continue
        target = int(match.group(1), 16)
        if low <= target < high:
            calls[target] += 1
    return calls


def main() -> int:
    ap = argparse.ArgumentParser()
    ap.add_argument("exe", type=Path)
    ap.add_argument(
        "--symbols",
        type=Path,
        default=Path("ghidra/known_symbols.csv"),
    )
    ap.add_argument("--objdump", default="objdump")
    ap.add_argument("--low", type=lambda x: int(x, 0), default=0x00401000)
    ap.add_argument("--high", type=lambda x: int(x, 0), default=0x0042F000)
    ap.add_argument("--top", type=int, default=50)
    args = ap.parse_args()

    known = load_known_symbols(args.symbols)
    calls = collect_direct_calls(args.exe, args.objdump, args.low, args.high)

    known_targets = {address for address in calls if address in known}
    unknown_targets = {address for address in calls if address not in known}

    total_sites = sum(calls.values())
    known_sites = sum(calls[address] for address in known_targets)

    print(f"internal direct-call targets: {len(calls)}")
    print(f"known targets:               {len(known_targets)}")
    print(f"unknown targets:             {len(unknown_targets)}")
    print(f"direct-call sites:           {total_sites}")
    print(f"sites to known targets:      {known_sites}")
    if total_sites:
        print(f"call-site coverage:          {known_sites / total_sites:.1%}")
    if calls:
        print(f"target coverage:             {len(known_targets) / len(calls):.1%}")

    print()
    print("Top unknown targets:")
    for address in sorted(
        unknown_targets,
        key=lambda a: (-calls[a], a),
    )[: max(args.top, 0)]:
        print(f"0x{address:08X}  calls={calls[address]}")

    return 0


if __name__ == "__main__":
    raise SystemExit(main())

#!/usr/bin/env python3
"""Fail a native x64 release build if it imports a retail codec or VC runtime.

This verifies the PE import directory, not dynamic LoadLibrary behavior.
Uses only Python's standard library and never executes the inspected binary.
"""
import argparse
import struct
from pathlib import Path


def imports(path):
    data = Path(path).read_bytes()
    def read(fmt, offset):
        if offset < 0 or offset + struct.calcsize(fmt) > len(data):
            raise ValueError("truncated PE")
        return struct.unpack_from(fmt, data, offset)
    if data[:2] != b"MZ":
        raise ValueError("not an MZ executable")
    pe, = read("<I", 0x3C)
    if data[pe:pe+4] != b"PE\0\0":
        raise ValueError("missing PE signature")
    machine, nsections, _, _, _, opt_size, _ = read("<HHIIIHH", pe+4)
    opt = pe+24
    magic, = read("<H", opt)
    if machine != 0x8664 or magic != 0x20B:
        raise ValueError("expected native x64 PE32+")
    count, = read("<I", opt+108)
    if count < 2 or opt_size < 128:
        raise ValueError("missing PE import directory")
    directory, size = read("<II", opt+120)
    if not directory or size < 20:
        raise ValueError("empty import directory")
    sections = []
    for i in range(nsections):
        start = opt+opt_size+i*40
        virtual_size, va, raw_size, raw = read("<IIII", start+8)
        sections.append((va, max(virtual_size, raw_size), raw_size, raw))
    def offset(rva):
        for va, span, raw_size, raw in sections:
            if va <= rva < va+span and rva-va < raw_size:
                pos = raw+rva-va
                if pos < len(data):
                    return pos
        raise ValueError(f"invalid RVA {rva:#x}")
    result = []
    for i in range(size//20):
        desc = read("<IIIII", offset(directory+i*20))
        if not any(desc):
            return result
        name_at = offset(desc[3])
        end = data.find(b"\0", name_at, min(len(data), name_at+512))
        if end < 0:
            raise ValueError("unterminated DLL name")
        result.append(data[name_at:end].decode("ascii").lower())
    raise ValueError("unterminated import descriptor table")


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("exe")
    args = parser.parse_args()
    try:
        names = imports(args.exe)
    except (ValueError, OSError, struct.error) as e:
        parser.exit(1, f"{e}\n")
    print("Native x64 imports:", ", ".join(sorted(names)))
    prohibited = ("bink", "msvcp", "msvcr", "vcruntime", "api-ms-win-crt", "concrt", "ucrtbase")
    blocked = [n for n in names if n.startswith(prohibited)]
    if blocked:
        parser.exit(1, f"Unexpected codec/runtime imports: {', '.join(blocked)}\n")


if __name__ == "__main__":
    main()

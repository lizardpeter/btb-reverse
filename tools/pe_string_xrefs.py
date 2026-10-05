#!/usr/bin/env python3
"""Find absolute x86 references from PE code to printable strings.

Designed for Bob Builds a Park's non-ASLR PE32 image. No third-party packages required.
"""

from __future__ import annotations

import argparse
import json
import re
import struct
from pathlib import Path


PRINTABLE = re.compile(rb"[\x20-\x7e]{4,}")


def parse_pe(data: bytes):
    pe_off = struct.unpack_from("<I", data, 0x3C)[0]
    if data[pe_off:pe_off + 4] != b"PE\0\0":
        raise ValueError("not a PE file")
    coff = pe_off + 4
    section_count = struct.unpack_from("<H", data, coff + 2)[0]
    optional_size = struct.unpack_from("<H", data, coff + 16)[0]
    optional = coff + 20
    magic = struct.unpack_from("<H", data, optional)[0]
    if magic != 0x10B:
        raise ValueError("expected PE32")
    image_base = struct.unpack_from("<I", data, optional + 28)[0]
    section_table = optional + optional_size

    sections = []
    for i in range(section_count):
        o = section_table + i * 40
        name = data[o:o + 8].rstrip(b"\0").decode("ascii", "replace")
        virtual_size, rva, raw_size, raw_offset = struct.unpack_from("<IIII", data, o + 8)
        sections.append({
            "name": name,
            "rva": rva,
            "virtual_size": virtual_size,
            "raw_size": raw_size,
            "raw_offset": raw_offset,
            "va": image_base + rva,
        })
    return image_base, sections


def main() -> int:
    ap = argparse.ArgumentParser()
    ap.add_argument("exe", type=Path)
    ap.add_argument("--out", type=Path, required=True)
    ap.add_argument("--min-len", type=int, default=4)
    ap.add_argument("--pathish-only", action="store_true")
    args = ap.parse_args()

    blob = args.exe.read_bytes()
    image_base, sections = parse_pe(blob)

    strings = []
    for sec in sections:
        raw = blob[sec["raw_offset"]:sec["raw_offset"] + sec["raw_size"]]
        for m in PRINTABLE.finditer(raw):
            text = m.group().decode("ascii", "replace")
            if len(text) < args.min_len:
                continue
            if args.pathish_only and not re.search(
                r"(?i)(?:data|loaddata)[\\/]|\.(?:bmp|txt|wav|bik|ico|dll)\b", text
            ):
                continue
            strings.append({
                "va": sec["va"] + m.start(),
                "section": sec["name"],
                "text": text,
            })

    code_sections = [s for s in sections if s["name"] == ".text"]
    refs = []
    for item in strings:
        needle = struct.pack("<I", item["va"])
        for sec in code_sections:
            raw = blob[sec["raw_offset"]:sec["raw_offset"] + sec["raw_size"]]
            start = 0
            while True:
                pos = raw.find(needle, start)
                if pos < 0:
                    break
                refs.append({
                    "from_va": sec["va"] + pos,
                    "to_va": item["va"],
                    "text": item["text"],
                })
                start = pos + 1

    refs.sort(key=lambda x: (x["from_va"], x["to_va"]))
    result = {
        "file": args.exe.name,
        "image_base": image_base,
        "string_count": len(strings),
        "xref_count": len(refs),
        "xrefs": refs,
    }

    args.out.parent.mkdir(parents=True, exist_ok=True)
    args.out.write_text(json.dumps(result, indent=2), encoding="utf-8")
    print(f"Wrote {len(refs)} code->string xrefs to {args.out}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())

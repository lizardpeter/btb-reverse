#!/usr/bin/env python3
"""Read-only PE architecture/import gate for the native Windows game."""
import argparse
import hashlib
import json
from pathlib import Path
import re

def inspect(path, require_static_crt=False):
    import pefile
    pe = pefile.PE(str(path), fast_load=True)
    try:
        pe.parse_data_directories(directories=[
            pefile.DIRECTORY_ENTRY["IMAGE_DIRECTORY_ENTRY_IMPORT"],
            pefile.DIRECTORY_ENTRY["IMAGE_DIRECTORY_ENTRY_DELAY_IMPORT"],
        ])
        imports = sorted({item.dll.decode("ascii").lower()
                          for attr in ("DIRECTORY_ENTRY_IMPORT",
                                       "DIRECTORY_ENTRY_DELAY_IMPORT")
                          for item in getattr(pe, attr, [])})
        violations = []
        if pe.FILE_HEADER.Machine != 0x8664 or pe.OPTIONAL_HEADER.Magic != 0x20b:
            violations.append("not an AMD64 PE32+ image")
        if pe.OPTIONAL_HEADER.Subsystem != 2:
            violations.append("not a Windows GUI executable")
        if pe.FILE_HEADER.Characteristics & 0x2000:
            violations.append("image is a DLL, not an executable")
        if not imports:
            violations.append("empty or unreadable import table")
        for dll in imports:
            if re.match(r"^(bink|radvideo|avcodec|avformat|avutil|swscale|swresample|ffmpeg)", dll):
                violations.append("external codec import: " + dll)
            if require_static_crt and re.match(
                r"^(msvcp1[0-9]+|vcruntime|concrt|ucrtbase|api-ms-win-crt-)", dll
            ):
                violations.append("dynamic C/C++ runtime import: " + dll)
        return {
            "scope": "PE import-table validation; not interactive gameplay or dynamic-load certification",
            "file": path.name, "bytes": path.stat().st_size,
            "sha256": hashlib.sha256(path.read_bytes()).hexdigest(),
            "machine": hex(pe.FILE_HEADER.Machine),
            "magic": hex(pe.OPTIONAL_HEADER.Magic),
            "subsystem": pe.OPTIONAL_HEADER.Subsystem,
            "imports": imports, "require_static_crt": require_static_crt,
            "violations": violations, "passed": not violations,
        }
    finally:
        pe.close()

def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("exe", type=Path)
    parser.add_argument("--out", type=Path, required=True)
    parser.add_argument("--require-static-crt", action="store_true")
    args = parser.parse_args()
    if args.exe.resolve() == args.out.resolve():
        parser.error("report must not overwrite input executable")
    report = inspect(args.exe, args.require_static_crt)
    args.out.write_text(json.dumps(report, indent=2) + "\n", encoding="utf-8")
    print(json.dumps(report, indent=2))
    return int(not report["passed"])

if __name__ == "__main__":
    raise SystemExit(main())

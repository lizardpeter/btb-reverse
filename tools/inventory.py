#!/usr/bin/env python3
"""Inventory a local Bob Builds a Park source tree without external dependencies."""

from __future__ import annotations

import argparse
import hashlib
import json
from collections import Counter
from pathlib import Path


def sha256(path: Path, chunk_size: int = 1024 * 1024) -> str:
    h = hashlib.sha256()
    with path.open("rb") as f:
        for chunk in iter(lambda: f.read(chunk_size), b""):
            h.update(chunk)
    return h.hexdigest()


def main() -> int:
    ap = argparse.ArgumentParser()
    ap.add_argument("root", type=Path)
    ap.add_argument("--out", type=Path, required=True)
    ap.add_argument("--no-hash", action="store_true", help="Skip SHA-256 for faster scans.")
    args = ap.parse_args()

    root = args.root.resolve()
    if not root.is_dir():
        raise SystemExit(f"Not a directory: {root}")

    records = []
    extensions = Counter()
    total_bytes = 0

    for path in sorted(p for p in root.rglob("*") if p.is_file()):
        st = path.stat()
        rel = path.relative_to(root).as_posix()
        ext = path.suffix.lower() or "<none>"
        extensions[ext] += 1
        total_bytes += st.st_size
        rec = {
            "path": rel,
            "size": st.st_size,
            "extension": ext,
        }
        if not args.no_hash:
            rec["sha256"] = sha256(path)
        records.append(rec)

    result = {
        "root_name": root.name,
        "file_count": len(records),
        "total_bytes": total_bytes,
        "extensions": dict(sorted(extensions.items())),
        "files": records,
    }

    args.out.parent.mkdir(parents=True, exist_ok=True)
    args.out.write_text(json.dumps(result, indent=2), encoding="utf-8")
    print(f"Wrote {len(records)} files to {args.out}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())

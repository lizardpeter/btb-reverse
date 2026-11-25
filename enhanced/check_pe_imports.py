"""Check both PE import directories, not strings or the linker command."""
import argparse
import struct
from pathlib import Path

def imports(data: bytes) -> set[str]:
    def read(fmt, off):
        if off < 0 or off + struct.calcsize(fmt) > len(data):
            raise ValueError("truncated PE")
        return struct.unpack_from(fmt, data, off)
    if data[:2] != b"MZ":
        raise ValueError("not a PE")
    pe, = read("<I", 0x3C)
    if data[pe:pe+4] != b"PE\0\0":
        raise ValueError("invalid PE signature")
    sections, = read("<H", pe+6)
    optional_size, = read("<H", pe+20)
    opt = pe+24
    magic, = read("<H", opt)
    if magic == 0x20B:
        image_base, = read("<Q", opt+24)
        count_off, dirs_off = 108, 112
    elif magic == 0x10B:
        image_base, = read("<I", opt+28)
        count_off, dirs_off = 92, 96
    else:
        raise ValueError("unsupported optional header")
    count, = read("<I", opt+count_off)
    header_size, = read("<I", opt+60)
    mappings = []
    for i in range(sections):
        virtual_size, va, raw_size, raw = read("<IIII", opt+optional_size+i*40+8)
        mappings.append((va, raw_size, raw))
    def offset(rva, size):
        if rva < header_size and rva+size <= min(header_size,len(data)):
            return rva
        for va, raw_size, raw in mappings:
            if va <= rva and rva+size <= va+raw_size and raw+rva-va+size <= len(data):
                return raw+rva-va
        raise ValueError(f"unmapped RVA {rva:#x}")
    names = set()
    def name(rva):
        chars = bytearray()
        for i in range(260):
            c = data[offset(rva+i,1)]
            if not c:
                return chars.decode("ascii").lower()
            chars.append(c)
        raise ValueError("unterminated DLL name")
    for index, stride, fmt in [(1,20,"<5I"), (13,32,"<8I")]:
        if count <= index:
            continue
        if dirs_off+(index+1)*8 > optional_size:
            raise ValueError("truncated data directories")
        rva, size = read("<II", opt+dirs_off+index*8)
        if not rva and not size:
            continue
        if not rva or size < stride:
            raise ValueError("invalid import directory")
        terminated = False
        for pos in range(0,size-stride+1,stride):
            fields = read(fmt,offset(rva+pos,stride))
            if not any(fields):
                terminated = True
                break
            nrva = fields[3] if index == 1 else fields[1]
            if index == 13 and not fields[0]&1:
                nrva -= image_base
            if nrva <= 0:
                raise ValueError("invalid DLL name RVA")
            names.add(name(nrva))
        if not terminated:
            raise ValueError("unterminated import directory")
    return names

def main():
    p=argparse.ArgumentParser()
    p.add_argument("exe", type=Path)
    p.add_argument("--forbid", action="append", default=[])
    p.add_argument("--require", action="append", default=[])
    a=p.parse_args()
    names=imports(a.exe.read_bytes())
    print(a.exe.name + ": " + ", ".join(sorted(names)))
    forbidden=names & {x.lower() for x in a.forbid}
    missing={x.lower() for x in a.require}-names
    if forbidden or missing:
        raise SystemExit(f"Unexpected imports: {sorted(forbidden)}; missing: {sorted(missing)}")
if __name__ == "__main__":
    main()

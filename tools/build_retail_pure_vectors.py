#!/usr/bin/env python3
"""Run three original, dependency-free PE32 routines in an x86 emulator.

Requires the user's verified retail EXE and Unicorn, never reconstructed C++.
Results become independent regression inputs for the native implementations.
"""
import argparse
import hashlib
import json
import random
import struct
from pathlib import Path
import unicorn
from unicorn import Uc, UC_ARCH_X86, UC_MODE_32
from unicorn.x86_const import UC_X86_REG_ESP, UC_X86_REG_EIP, UC_X86_REG_EAX, UC_X86_REG_EDX, UC_X86_REG_EFLAGS

SHA = "c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05"
STACK, DATA, STOP = 0x700000, 0x800000, 0x900000

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("exe", type=Path)
    parser.add_argument("--out", type=Path, required=True)
    args = parser.parse_args()
    raw = args.exe.read_bytes()
    if hashlib.sha256(raw).hexdigest() != SHA:
        raise ValueError("wrong retail executable SHA-256")
    pe = struct.unpack_from("<I", raw, 60)[0]
    sections, optional_size = struct.unpack_from("<H", raw, pe+6)[0], struct.unpack_from("<H", raw, pe+20)[0]
    base, size = struct.unpack_from("<I", raw, pe+52)[0], struct.unpack_from("<I", raw, pe+80)[0]
    emu = Uc(UC_ARCH_X86, UC_MODE_32)
    emu.mem_map(base, (size+4095)&~4095)
    emu.mem_write(base, raw[:min(len(raw),4096)])
    for i in range(sections):
        at = pe+24+optional_size+40*i
        _, address, file_size, file_at = struct.unpack_from("<IIII", raw, at+8)
        if file_size:
            emu.mem_write(base+address, raw[file_at:file_at+file_size])
    for address, length in [(STACK,65536),(DATA,65536),(STOP,4096)]:
        emu.mem_map(address,length)

    def call(address, words):
        stack = STACK+32768
        emu.mem_write(stack, struct.pack("<"+"I"*(len(words)+1), STOP,*words))
        emu.reg_write(UC_X86_REG_ESP,stack)
        emu.reg_write(UC_X86_REG_EFLAGS,2)
        emu.reg_write(UC_X86_REG_EAX,0)
        emu.reg_write(UC_X86_REG_EDX,0)
        emu.emu_start(address,STOP,count=100000)
        if emu.reg_read(UC_X86_REG_EIP)!=STOP:
            raise RuntimeError("original routine did not return within the instruction budget")
        return emu.reg_read(UC_X86_REG_EAX),emu.reg_read(UC_X86_REG_EDX)

    rows=[]
    rng=random.Random(0x2002)
    boundary=[0,1,2,0xffffffff,0x100000000,0xffffffffffffffff]
    pairs=[(a,b) for a in boundary for b in boundary if b]
    pairs += [(rng.getrandbits(64), (rng.getrandbits(32 if i%2 else 64)|1)) for i in range(1000)]
    for a,b in pairs:
        lo,hi=call(0x435630,[a&0xffffffff,a>>32,b&0xffffffff,b>>32])
        rows.append(f"REM {a:x} {b:x} {(hi<<32)|lo:x}")
    finds=0
    for offset in range(4):
        for text in [b"",b"original",b"ababa",bytes([128,255,1,128])]:
            for value in [0,1,ord('a'),ord('g'),ord('x'),128,255,0x179,0xffffffff]:
                payload=text+b"\0"
                emu.mem_write(DATA,b"\xcc"*64)
                emu.mem_write(DATA+offset,payload)
                result,_=call(0x4380f0,[DATA+offset,value])
                relative=-1 if result==0 else result-(DATA+offset)
                if relative < -1 or relative>=len(payload):
                    raise RuntimeError("original strchr returned outside source string")
                rows.append(f"FIND {value:x} {offset} {(b'\xcc'*offset+payload).hex()} {relative}")
                finds+=1
    moves=0
    for source in range(8):
        for destination in range(8):
            for count in [0,1,2,3,4,7,8,15,16,31,32]:
                payload=bytes(range(64))
                emu.mem_write(DATA,payload)
                result,_=call(0x430500,[DATA+destination,DATA+source,count])
                if result!=DATA+destination:
                    raise RuntimeError("original memmove returned the wrong destination")
                after=bytes(emu.mem_read(DATA,64))
                rows.append(f"MOVE {source} {destination} {count} {payload.hex()} {after.hex()}")
                moves+=1
    args.out.parent.mkdir(parents=True,exist_ok=True)
    text="\n".join(rows)+"\n"
    args.out.write_text(text)
    args.out.with_suffix(".json").write_text(json.dumps({
        "source_executable_sha256":SHA,"emulator":"Unicorn "+unicorn.__version__,
        "executed_original_addresses":["00435630","004380F0","00430500"],
        "remainder_vectors":len(pairs),"strchr_vectors":finds,"memmove_vectors":moves,
        "vectors_sha256":hashlib.sha256(text.encode()).hexdigest(),
        "scope":"Original x86 instruction result checks for bounded valid inputs; not whole-game or complete ABI verification."
    },indent=2)+"\n")
    print(f"Original x86 vectors: remainder={len(pairs)}, strchr={finds}, memmove={moves}")

if __name__=="__main__":
    main()

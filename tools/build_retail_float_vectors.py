#!/usr/bin/env python3
"""Generate independent floating conversion vectors from the original PE32.

Requires Unicorn and the SHA-verified retail executable, which stays outside Git.
"""
import argparse
import hashlib
import json
import random
import struct
from pathlib import Path
import unicorn
from unicorn import Uc, UC_ARCH_X86, UC_MODE_32
from unicorn.x86_const import UC_X86_REG_ESP, UC_X86_REG_EIP, UC_X86_REG_EAX, UC_X86_REG_EFLAGS
from build_retail_pure_vectors import SHA, STACK, DATA, STOP


def main():
    p = argparse.ArgumentParser()
    p.add_argument('exe', type=Path)
    p.add_argument('--out', type=Path, required=True)
    a = p.parse_args()
    raw = a.exe.read_bytes()
    if hashlib.sha256(raw).hexdigest() != SHA:
        raise ValueError('wrong executable SHA-256')
    pe = struct.unpack_from('<I', raw, 60)[0]
    sections = struct.unpack_from('<H', raw, pe+6)[0]
    opt = struct.unpack_from('<H', raw, pe+20)[0]
    base = struct.unpack_from('<I', raw, pe+52)[0]
    size = struct.unpack_from('<I', raw, pe+80)[0]
    emu = Uc(UC_ARCH_X86, UC_MODE_32)
    emu.mem_map(base, (size+4095)&~4095)
    emu.mem_write(base, raw[:4096])
    for i in range(sections):
        _, address, n, at = struct.unpack_from('<IIII', raw, pe+24+opt+40*i+8)
        if n:
            emu.mem_write(base+address, raw[at:at+n])
    for at, n in [(STACK,65536),(DATA,65536),(STOP,4096)]:
        emu.mem_map(at,n)

    def call(address, args):
        stack = STACK+32768
        emu.mem_write(stack, struct.pack('<'+'I'*(len(args)+1),STOP,*args))
        emu.reg_write(UC_X86_REG_ESP,stack)
        emu.reg_write(UC_X86_REG_EFLAGS,2)
        emu.reg_write(UC_X86_REG_EAX,0)
        emu.emu_start(address,STOP,count=100000)
        if emu.reg_read(UC_X86_REG_EIP)!=STOP:
            raise RuntimeError('original did not return')
        return emu.reg_read(UC_X86_REG_EAX)

    rows = []
    counts = {}
    def row(kind, fields):
        rows.append(kind+' '+' '.join(f'{x:x}' for x in fields))
        counts[kind] = counts.get(kind,0)+1
    rng = random.Random(0x43786e)
    for value in list(range(0x1000))+[rng.getrandbits(32) for _ in range(1000)]:
        row('CTRL',[value,call(0x437520,[value])])
    words = [[0,0,0],[0xffffffff]*3,[0x80000000,0,0],[0,0x80000000,0],
             [0xffffffff,0xffffffff,1]]
    words += [[rng.getrandbits(32) for _ in range(3)] for _ in range(100)]
    for w in words:
        for n in range(1,95):
            emu.mem_write(DATA,struct.pack('<3I',*w))
            result = call(0x437713,[DATA,n])
            after = struct.unpack('<3I',emu.mem_read(DATA,12))
            row('ROUND',[n,*w,result,*after])
        for n in range(129):
            emu.mem_write(DATA,struct.pack('<3I',*w))
            call(0x4377e1,[DATA,n])
            after = struct.unpack('<3I',emu.mem_read(DATA,12))
            row('SHIFT',[n,*w,*after])
    doubles = [0,1,0x8000000000000000,0x3ff0000000000000,0xbff0000000000000,
               0x7ff0000000000000,0xfff0000000000000,0x7ff8000000000001,
               0xffffffffffffffff,0x000fffffffffffff,0x0010000000000000]
    doubles += [rng.getrandbits(64) for _ in range(1000)]
    for bits in doubles:
        emu.mem_write(DATA,struct.pack('<Q',bits))
        emu.mem_write(DATA+32,b'\xcc'*12)
        call(0x437b3b,[DATA+32,DATA])
        lo,hi,ex = struct.unpack('<IIH',emu.mem_read(DATA+32,10))
        row('EXTEND',[bits,lo,hi,ex])
    exponents = [0,1,0x3fff-1077,0x3fff-1076,0x3fff-1024,0x3fff-1023,
                 0x3fff-151,0x3fff-150,0x3fff-127,0x3fff-126,0x3fff,
                 0x3fff+127,0x3fff+128,0x3fff+1023,0x3fff+1024,0x7fff]
    inputs = [[lo,0,0,hi&0xffff,hi>>16,exp|sign]
              for exp in exponents for sign in [0,0x8000]
              for lo,hi in [(0,0),(0,0x80000000),(0xffff,0xffffffff)]]
    inputs += [[rng.getrandbits(16) for _ in range(6)] for _ in range(2000)]
    for v in inputs:
        for fmt,table in [(32,0x449f60),(64,0x449f48)]:
            emu.mem_write(DATA,struct.pack('<6H',*v))
            emu.mem_write(DATA+32,b'\0'*8)
            result = call(0x43786e,[DATA,DATA+32,table])
            lo,hi = struct.unpack('<2I',emu.mem_read(DATA+32,8))
            row('CONVERT',[fmt,*v,result,lo,hi])
    for n in range(1,31):
        for _ in range(20):
            digits = [rng.randrange(1,10)]+[rng.randrange(10) for _ in range(n-1)]
            emu.mem_write(DATA,bytes(digits))
            call(0x4388a1,[DATA,n,DATA+128])
            after = struct.unpack('<3I',emu.mem_read(DATA+128,12))
            row('DECIMAL',[n,*digits,*after])
    a.out.parent.mkdir(parents=True,exist_ok=True)
    text = '\n'.join(rows)+'\n'
    a.out.write_text(text)
    a.out.with_suffix('.json').write_text(json.dumps({
        'source_executable_sha256':SHA,'emulator':'Unicorn '+unicorn.__version__,
        'original_addresses':['00437520','00437713','004377E1','0043786E','00437B3B','004388A1'],
        'vector_counts':counts,'vectors_sha256':hashlib.sha256(text.encode()).hexdigest(),
        'format_tables':{'00449F48':[1024,-1023,53,11,64,1023],
                         '00449F60':[128,-127,24,8,32,127]},
        'scope':'Bounded valid-input instruction output comparison; no whole-game or ABI certification.'
    },indent=2)+'\n')
    print(counts)

if __name__=='__main__':
    main()

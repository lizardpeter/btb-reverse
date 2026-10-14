#!/usr/bin/env python3
"""Original x86 command-line, strcat and PMD regression vectors."""
import argparse, hashlib, json, random, struct
from pathlib import Path
import unicorn
from unicorn import Uc, UC_ARCH_X86, UC_MODE_32
from unicorn.x86_const import UC_X86_REG_ESP, UC_X86_REG_EIP, UC_X86_REG_EAX, UC_X86_REG_EFLAGS
from build_retail_pure_vectors import SHA, STACK, DATA, STOP

def main():
    p=argparse.ArgumentParser();p.add_argument('exe',type=Path);p.add_argument('--out',type=Path,required=True);a=p.parse_args()
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


    rows=[];counts={};rng=random.Random(0x435a9d)
    def row(kind,fields):
        rows.append(kind+' '+' '.join(fields));counts[kind]=counts.get(kind,0)+1
    def hx(b):return b.hex() or '-'
    emu.mem_write(0x51c7e0,b'\0'*257)
    commands=[b'',b'bob.exe',b'"bob game.exe" one two',b'bob "a b" c',b'bob ""',
              b'bob "a""b" c',b'bob \\"a\\"',b'bob a\\\\"b',b'bob\tone  two',b'  leading',b'"unterminated']
    alphabet=b'abc123 \t"\\'
    commands += [b'bob '+bytes(rng.choice(alphabet) for _ in range(rng.randrange(100))) for _ in range(2000)]
    for command in commands:
        emu.mem_write(DATA,command+b'\0')
        emu.mem_write(DATA+0x1000,b'\0'*4096);emu.mem_write(DATA+0x2000,b'\0'*4096)
        call(0x435a9d,[DATA,DATA+0x1000,DATA+0x2000,DATA+0x3000,DATA+0x3004])
        count,bytes_count=struct.unpack('<2I',emu.mem_read(DATA+0x3000,8))
        args=[]
        for i in range(count-1):
            at=struct.unpack('<I',emu.mem_read(DATA+0x1000+i*4,4))[0]
            raw_value=bytes(emu.mem_read(at,1024)).split(b'\0',1)[0];args.append(hx(raw_value))
        if struct.unpack('<I',emu.mem_read(DATA+0x1000+(count-1)*4,4))[0]:raise RuntimeError('argv terminator')
        row('ARGV',[hx(command),f'{count:x}',f'{bytes_count:x}',*args])
    emu.mem_write(0x51c7e0+0x81+1,b'\x04')
    for command in [b'"bo\x81Ab.exe" one',b'"bo\x81"b.exe" one',b'bob \x81" a',b'bob "\x81"a" b',b'bob \x81\\ c']:
        emu.mem_write(DATA,command+b'\0')
        call(0x435a9d,[DATA,0,0,DATA+0x3000,DATA+0x3004])
        count,size=struct.unpack('<2I',emu.mem_read(DATA+0x3000,8))
        row('COUNT',[hx(command),f'{count:x}',f'{size:x}'])
    emu.mem_write(0x51c7e0+0x81+1,b'\0')
    for offset in range(4):
        for source_offset in range(4):
            for left in [b'',b'Bob',b'a'*15,b'b'*32]:
                for right in [b'',b' builds',b'c'*15,b'd'*32]:
                    emu.mem_write(DATA+offset,left+b'\0');emu.mem_write(DATA+512+source_offset,right+b'\0')
                    result=call(0x433640,[DATA+offset,DATA+512+source_offset])
                    if result!=DATA+offset:raise RuntimeError('strcat pointer return')
                    value=bytes(emu.mem_read(DATA+offset,128)).split(b'\0',1)[0]
                    row('CAT',[hx(left),hx(right),hx(value)])
    for _ in range(1000):
        base=DATA;member=rng.randrange(-128,128);virtual=rng.choice([-1,16,32]);vdisp=rng.randrange(-128,128)*4;adjustment=rng.randrange(-128,128)
        emu.mem_write(DATA+512,struct.pack('<iii',member,virtual,vdisp))
        if virtual>=0:
            emu.mem_write(base+virtual,struct.pack('<I',DATA+4096));emu.mem_write(DATA+4096+vdisp,struct.pack('<i',adjustment))
        result=call(0x43259c,[base,DATA+512])
        row('PMD',[f'{member&0xffffffff:x}',f'{virtual&0xffffffff:x}',f'{vdisp&0xffffffff:x}',f'{adjustment&0xffffffff:x}',f'{(result-base)&0xffffffff:x}'])
    # Controlled dependencies: DirectDraw Blt returns zero; voice returns.
    # Only original entry control-flow/global writes are being compared.
    emu.mem_write(0x401000,b'\xc3')
    emu.mem_write(STOP+0x100,b'\xc2\x18\x00')
    def put(at,v):emu.mem_write(at,struct.pack('<I',v))
    put(0x44de08,DATA+0x400);put(DATA+0x40c,DATA+0x600)
    put(0x51c298,DATA+0x700);put(DATA+0x700,DATA+0x500)
    put(DATA+0x51c,STOP+0x100)
    help_rows=[]
    for flow in range(68):
        for popup in [0,1]:
            for progress in [0,1]:
                put(0x44de14,flow);put(0x51c2f4,popup);put(0x51c324,progress)
                put(0x44a2a0,99);put(0x44dda0,99);put(0x44dda8,99)
                call(0x4011b0,[])
                active=struct.unpack('<I',emu.mem_read(0x44dda0,4))[0]
                context=struct.unpack('<I',emu.mem_read(0x44ddac,4))[0]
                last=struct.unpack('<I',emu.mem_read(0x44dda8,4))[0]
                help_rows.append(' '.join(f'{v:x}' for v in [flow,popup,progress,active,context,last]))
    help_text='\n'.join(help_rows)+'\n'
    help_path=a.out.with_name('retail-help-vectors.txt');help_path.write_text(help_text)
    help_path.with_suffix('.json').write_text(json.dumps({'source_executable_sha256':SHA,'original_address':'004011B0','vectors':len(help_rows),'vectors_sha256':hashlib.sha256(help_text.encode()).hexdigest(),'controlled_dependencies':['DirectDraw Blt stdcall return','00401000 PlayHelpVoice cdecl return'],'scope':'Original help-entry context/global writes for all 68 states and popup/progress flags; not graphical/audio behavior.'},indent=2)+'\n')
    text='\n'.join(rows)+'\n';a.out.write_text(text)
    a.out.with_suffix('.json').write_text(json.dumps({'source_executable_sha256':SHA,'emulator':'Unicorn '+unicorn.__version__,'original_addresses':['00435A9D','00433640','0043259C'],'vector_counts':counts,'vectors_sha256':hashlib.sha256(text.encode()).hexdigest(),'scope':'Bounded source-result comparison, ASCII argv and supplied PMD memory; no whole-game/ABI certification.'},indent=2)+'\n')
    print(counts)
if __name__=='__main__':main()

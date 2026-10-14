#!/usr/bin/env python3
"""Execute original editor logic with controlled cursor/audio dependencies."""
import argparse, copy, hashlib, json, random, struct
from pathlib import Path
import unicorn
from unicorn import Uc, UC_ARCH_X86, UC_MODE_32, UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_ESP, UC_X86_REG_EIP, UC_X86_REG_EAX, UC_X86_REG_EFLAGS
from build_retail_pure_vectors import SHA, STACK, DATA, STOP

def main():
    parser=argparse.ArgumentParser();parser.add_argument('exe',type=Path);parser.add_argument('--out',type=Path,required=True);a=parser.parse_args()
    raw=a.exe.read_bytes()
    if hashlib.sha256(raw).hexdigest()!=SHA:raise ValueError('wrong executable SHA')
    pe=struct.unpack_from('<I',raw,60)[0];n=struct.unpack_from('<H',raw,pe+6)[0];opt=struct.unpack_from('<H',raw,pe+20)[0]
    base=struct.unpack_from('<I',raw,pe+52)[0];size=struct.unpack_from('<I',raw,pe+80)[0]
    emu=Uc(UC_ARCH_X86,UC_MODE_32);emu.mem_map(base,(size+4095)&~4095);emu.mem_write(base,raw[:4096])
    for i in range(n):
        _,rva,sz,at=struct.unpack_from('<IIII',raw,pe+24+opt+40*i+8)
        if sz:emu.mem_write(base+rva,raw[at:at+sz])
    for at,sz in [(STACK,65536),(DATA,65536),(STOP,4096)]:emu.mem_map(at,sz)
    def put(at,v):emu.mem_write(at,struct.pack('<I',v&0xffffffff))
    def get(at):return struct.unpack('<I',emu.mem_read(at,4))[0]
    def put_array(at,v):emu.mem_write(at,struct.pack('<'+'I'*len(v),*[x&0xffffffff for x in v]))
    trace=[]
    def dependency(uc,address,size,user):
        if address not in (0x402cf0,0x407af0):return
        esp=uc.reg_read(UC_X86_REG_ESP);ret=get(esp)
        if address==0x402cf0:
            trace.extend([1,get(esp+4),get(esp+8),get(esp+12)]);pop=16
        else:
            trace.extend([2,(get(esp+4)-DATA)//16,0,0]);pop=4
        uc.reg_write(UC_X86_REG_EAX,1);uc.reg_write(UC_X86_REG_ESP,esp+pop);uc.reg_write(UC_X86_REG_EIP,ret)
    emu.hook_add(UC_HOOK_CODE,dependency)
    def call(at):
        esp=STACK+32768;put(esp,STOP);emu.reg_write(UC_X86_REG_ESP,esp);emu.reg_write(UC_X86_REG_EFLAGS,2)
        emu.emu_start(at,STOP,count=200000)
        if emu.reg_read(UC_X86_REG_EIP)!=STOP:raise RuntimeError('retail did not return')
        return emu.reg_read(UC_X86_REG_EAX)
    rows=[];counts={};rng=random.Random(0x426f30)
    def row(kind,v):rows.append(kind+' '+' '.join(f'{x&0xffffffff:x}' for x in v));counts[kind]=counts.get(kind,0)+1
    fields=[0x50a5bc,0x4fbd24,0x4fbd30,0x50a5c0,0x5093d8,0x50ab20,0x50ab24,0x509670,0x509674]
    for i in range(1000):
        seed=rng.getrandbits(32)
        regions=[]
        for j in range(30):
            if j<18:x,y=20+j%6*70,40+j//6*70;w,h=60,60
            else:x,y=400+(j-18)%3*60,300+(j-18)//3*35;w,h=50,30
            regions += [x,y,x+w,y+h,j%12]
        state=[rng.choice([0,1,13]),rng.randrange(640),rng.randrange(480),-3,-1,rng.choice([0,1,10]),rng.choice([0,1,10]),-3,-3]
        if i%2==0:
            j=rng.randrange(30);state[1]=regions[j*5]+20;state[2]=regions[j*5+1]+20
        grid=[rng.choice([-1,*range(12)]) for _ in range(18)]
        for at,v in zip(fields,state):put(at,v)
        put_array(0x50a678,grid);put_array(0x50a6c0,regions);put(0x509378,30);put(0x447558,seed)
        for typ in range(12):put(0x50a4b8+typ*4,DATA+typ*16)
        trace.clear();result=call(0x4126f0)
        after=[get(at) for at in fields];after_grid=[get(0x50a678+j*4) for j in range(18)]
        row('FIRE',[seed,*state,*grid,result,get(0x447558),*after,*after_grid,len(trace)//4,*trace])
    scalar=[0x5150cc,0x5150d0,0x514fb4,0x514f00,0x514f54,0x515098,0x514f5c,0x514edc]
    arrays=[(0x515078,3),(0x514f0c,12),(0x514fbc,12),(0x514ff0,4),(0x515088,4),(0x514ee0,4),(0x514eb8,8)]
    for mode in [0,1]:
        for i in range(1000):
            seed=rng.getrandbits(32)
            values=[rng.randrange(3),rng.randrange(3),rng.randrange(7),rng.randrange(3),-9,-9,99,99]
            values += [rng.randrange(4 if mode==1 else 3) for _ in range(3)]
            values += [rng.randrange(3) for _ in range(12)]
            values += [rng.randrange(36) for _ in range(12)]
            values += [-9]*20
            for at,v in zip(scalar,values[:8]):put(at,v)
            k=8
            for at,n in arrays:put_array(at,values[k:k+n]);k+=n
            assert k==len(values)==55
            put(0x447558,seed);call(0x4259d0 if mode==0 else 0x426f30)
            out=[get(at) for at in scalar]
            for at,n in arrays:out.extend(get(at+j*4) for j in range(n))
            row('SQUIRREL',[mode,seed,*values,get(0x447558),*out])
    text='\n'.join(rows)+'\n';a.out.write_text(text)
    a.out.with_suffix('.json').write_text(json.dumps({'source_executable_sha256':SHA,'emulator':'Unicorn '+unicorn.__version__,'original_addresses':['004126F0','004259D0','00426F30'],'vector_counts':counts,'vectors_sha256':hashlib.sha256(text.encode()).hexdigest(),'controlled_dependencies':['00402CF0 sound request recorded, thiscall return','00407AF0 cursor request recorded, cdecl return'],'scope':'Original editor state, RNG and cursor/audio request traces; not actual rendering/audio or whole-game certification.'},indent=2)+'\n')
    print(counts)
if __name__=='__main__':main()

#!/usr/bin/env python3
"""Independent original x86 time, signal and buffered-input regression vectors."""
import argparse, hashlib, json, random, struct
from pathlib import Path
import unicorn
from unicorn import Uc, UC_ARCH_X86, UC_MODE_32, UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_ESP, UC_X86_REG_EIP, UC_X86_REG_EAX, UC_X86_REG_EFLAGS
from build_retail_pure_vectors import SHA, STACK, DATA, STOP

def main():
    p=argparse.ArgumentParser();p.add_argument('exe',type=Path);p.add_argument('--out',type=Path,required=True);a=p.parse_args()
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
    def arr(at,v):emu.mem_write(at,struct.pack('<'+'I'*len(v),*[x&0xffffffff for x in v]))
    traces=[];read_count=0;allocation_ok=True;read_bytes=b'ABCD'
    def hook(uc,address,size,user):
        if address not in [0x430d93,0x433961,STOP+0x100,STOP+0x120]:return
        sp=uc.reg_read(UC_X86_REG_ESP);ret=get(sp);value=0
        if address==0x430d93:
            assert get(sp+4)==4096
            value=DATA+0x2000 if allocation_ok else 0
        elif address==0x433961:
            assert get(sp+4)==3
            count=read_count
            if count>0:emu.mem_write(get(sp+8),read_bytes[:count])
            value=count
        elif address==STOP+0x100:
            signal=get(sp+4);traces.extend([1,signal,get(sp+8) if signal==8 else 0,get(0x51c458),get(0x4497f4)])
        else:
            traces.extend([2,get(sp+4),0,get(0x51c458),get(0x4497f4)]);value=23
        uc.reg_write(UC_X86_REG_EAX,value&0xffffffff);uc.reg_write(UC_X86_REG_ESP,sp+4);uc.reg_write(UC_X86_REG_EIP,ret)
    emu.hook_add(UC_HOOK_CODE,hook)
    def call(at,args,stack_prefix=None):
        sp=STACK+32768;arr(sp,[STOP,*args])
        if stack_prefix is not None:arr(sp-40,stack_prefix)
        emu.reg_write(UC_X86_REG_ESP,sp);emu.reg_write(UC_X86_REG_EFLAGS,2);emu.reg_write(UC_X86_REG_EAX,0)
        emu.emu_start(at,STOP,count=200000)
        if emu.reg_read(UC_X86_REG_EIP)!=STOP:raise RuntimeError('original did not return')
        return emu.reg_read(UC_X86_REG_EAX)
    rows=[];counts={};rng=random.Random(0x436b93)
    def row(kind,v):rows.append(kind+' '+' '.join(f'{x&0xffffffff:x}' for x in v));counts[kind]=counts.get(kind,0)+1
    transition_fields=[0x449dc8,0x449dcc,0x449dd0,0x449dd8,0x449ddc,0x449de0]
    def reset_time(bias=-3600,daylight=1):
        for at in transition_fields:put(at,0xffffffff if at in [0x449dc8,0x449dd8] else 0)
        put(0x449d38,bias);put(0x449d34,daylight);put(0x51c570,0)
    def transitions():return [get(at) for at in transition_fields]
    for i in range(1500):
        bias=rng.choice([-7200,-3600,0,3600]);reset_time(bias)
        args=[rng.randrange(2),rng.randrange(2),rng.randrange(70,139),rng.randrange(1,13),rng.randrange(1,6),rng.randrange(7),rng.randrange(1,29),rng.randrange(24),rng.randrange(60),rng.randrange(60),rng.randrange(1000)]
        call(0x436b93,args);row('TRANS',[bias,*args,*transitions()])
    for i in range(1500):
        bias=rng.choice([-3600,0,3600]);daylight=rng.randrange(2);reset_time(bias,daylight)
        system=i%3!=0;put(0x51c570,int(system))
        start=[0,rng.choice([3,4,9,10]),rng.randrange(7),rng.randrange(1,6),rng.randrange(24),rng.randrange(60),rng.randrange(60),rng.randrange(1000)]
        end=[0,rng.choice([3,4,9,10]),rng.randrange(7),rng.randrange(1,6),rng.randrange(24),rng.randrange(60),rng.randrange(60),rng.randrange(1000)]
        if i%5==0:start[0]=2002;start[3]=rng.randrange(1,29);end[0]=2002;end[3]=rng.randrange(1,29)
        for at,v in [(0x51c610,start),(0x51c5bc,end)]:emu.mem_write(at,struct.pack('<8H',*v))
        tm=[rng.randrange(60),rng.randrange(60),rng.randrange(24),1,0,rng.randrange(70,139),0,rng.randrange(366),-1]
        arr(DATA,tm);result=call(0x4369e7,[DATA]);row('DST',[bias,daylight,int(system),*start,*end,*tm,result,*transitions()])
    for i in range(1500):
        bias=rng.choice([-3600,0,3600]);daylight=rng.randrange(2);reset_time(bias,daylight)
        zone=rng.randrange(-12,15)*3600;put(0x449d30,zone)
        # Original initialized latch avoids external tzset. Deliberately seed the
        # two unwritten stack words read by automatic DST on boundary dates.
        put(0x51c628,1)
        args=[rng.randrange(1960,2045),rng.randrange(1,13),rng.randrange(1,29),rng.randrange(24),rng.randrange(60),rng.randrange(60),rng.choice([-1,0,1])]
        prefix=[rng.randrange(60),rng.randrange(60)]
        result=call(0x4326b8,args,prefix);row('CAL',[bias,daylight,zone,*prefix,*args,result,*transitions()])
    # Descriptor flags live in the original 32-entry block table.
    put(0x51d920,DATA+0x4000)
    for i in range(2000):
        mode=i%2;allocation_ok=i%7!=0;read_count=rng.choice([-1,0,1,2])
        f=rng.choice([0,1,2,0x80,0x82,0x41,0x81,0x109,0x409,0x19,0x209])
        base_present=bool(i%3) or bool(f&0x10c);offset=rng.randrange(5) if base_present else 0
        count=rng.choice([0,1,2]);capacity=rng.choice([2,512,4096]);descflags=rng.randrange(256)
        # Non-string file byte values and string pushback success/failure.
        value=rng.choice([0xffffffff,65,66,90,256+65])
        words=[DATA+0x2000+offset if base_present else 0,count,DATA+0x2000 if base_present else 0,f,3,0,capacity,0]
        arr(DATA,words);emu.mem_write(DATA+0x2000,b'ABCD'+b'\0'*4092);put(0x51c568,17)
        emu.mem_write(DATA+0x4000+3*8+4,bytes([descflags]))
        result=call(0x43669f,[value,DATA]) if mode==0 else call(0x433888,[DATA])
        out=[get(DATA+j*4) for j in range(8)]
        # Normalize native/guest pointers against the resulting base.
        outptr=(out[0]-out[2])&0xffffffff if out[2] else 0
        before=[int(base_present),offset,count,f,capacity]
        buf=bytes(emu.mem_read(out[2],2)) if out[2] else bytes(2)
        row('IO',[mode,int(allocation_ok),read_count,descflags,value,*before,result,get(0x51c568),outptr,out[1],out[3],out[6],*buf])
    action_table=0x449770
    # The handler table starts at 449770. 449778 is its ACTION word, not its code.
    # Exercise both selected row inside and outside the cleared FPE range.
    put(0x4497f0,4)
    for i in range(1000):
        mode=i%2;signal=rng.choice([2,4,8,11,15,21,22,99]);choice=rng.choice([1,5,STOP+0x100])
        if mode==0 and choice==5:choice=STOP+0x100
        actions=[0xc000001d,4,choice,0xc000008e,8,choice,0xc0000005,11,choice,0xc0000090,8,choice]
        arr(action_table,actions);put(0x4497e8,rng.randrange(4));put(0x4497ec,1)
        first=get(0x4497e8);fpe=rng.randrange(129,145);old=DATA+0x6000;ptr=DATA+0x6100
        for at in [0x51c69c,0x51c6a8,0x51c6a0,0x51c6a4]:put(at,choice)
        put(0x51c458,old);put(0x4497f4,fpe);traces.clear()
        code=rng.choice([0xc000001d,0xc000008e,0xc0000005,0xc0000090])
        result=call(0x43854d,[signal]) if mode==0 else call(0x43576f,[code,ptr])
        after=[get(action_table+j*4) for j in range(12)]
        globals_after=[get(at) for at in [0x51c69c,0x51c6a8,0x51c6a0,0x51c6a4]]
        row('SIGNAL',[mode,signal,choice,code,first,fpe,old,ptr,result,get(0x51c458),get(0x4497f4),*after,*globals_after,len(traces)//5,*traces])
    text='\n'.join(rows)+'\n';a.out.write_text(text)
    a.out.with_suffix('.json').write_text(json.dumps({'source_executable_sha256':SHA,'emulator':'Unicorn '+unicorn.__version__,'vector_counts':counts,'vectors_sha256':hashlib.sha256(text.encode()).hexdigest(),'original_addresses':['00436B93','004369E7','004326B8','0043669F','00433888','0043854D','0043576F'],'controlled_dependencies':['00430D93 allocation success/failure','00433961 descriptor read result/data','guest signal handler trace','timezone initialization latch set; no tzset called'],'scope':'Original control flow, state mutations and service requests; not whole-game or operating-system certification.'},indent=2)+'\n')
    print(counts)
if __name__=='__main__':main()

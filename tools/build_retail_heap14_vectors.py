#!/usr/bin/env python3
"""Execute SHA-verified retail x86 bodies with explicit file, clock and heap dependencies."""
import argparse, hashlib, json, random, struct, zlib
from pathlib import Path
import unicorn
from unicorn import Uc, UC_ARCH_X86, UC_MODE_32, UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_ESP,UC_X86_REG_EIP,UC_X86_REG_EAX,UC_X86_REG_ECX,UC_X86_REG_EFLAGS
from build_retail_pure_vectors import SHA,STACK,DATA,STOP

def main():
 p=argparse.ArgumentParser();p.add_argument('exe',type=Path);p.add_argument('--out',type=Path,required=True);a=p.parse_args();raw=a.exe.read_bytes()
 if hashlib.sha256(raw).hexdigest()!=SHA:raise ValueError('wrong retail executable SHA')
 pe=struct.unpack_from('<I',raw,60)[0];optional=pe+24;ns=struct.unpack_from('<H',raw,pe+6)[0];optsize=struct.unpack_from('<H',raw,pe+20)[0];base=struct.unpack_from('<I',raw,optional+28)[0];size=struct.unpack_from('<I',raw,optional+56)[0]
 emu=Uc(UC_ARCH_X86,UC_MODE_32);emu.mem_map(base,(size+4095)&~4095);emu.mem_write(base,raw[:4096]);emu.mem_map(0,4096)
 for i in range(ns):
  _,rva,sz,off=struct.unpack_from('<IIII',raw,optional+optsize+i*40+8)
  if sz:emu.mem_write(base+rva,raw[off:off+sz])
 for addr,n in [(STACK,65536),(DATA,65536),(STOP,16384)]:emu.mem_map(addr,n)
 def put(at,v):emu.mem_write(at,struct.pack('<I',v&0xffffffff))
 def get(at):return struct.unpack('<I',emu.mem_read(at,4))[0]
 def array(at,v):emu.mem_write(at,struct.pack('<'+'I'*len(v),*[x&0xffffffff for x in v]))
 def cstr(at):return bytes(emu.mem_read(at,256)).split(b'\0',1)[0].decode('ascii')
 hooks={}
 def bind(name,argc,fn):
  addr=STOP+0x100+len(hooks)*16;hooks[addr]=(name,argc,fn);return addr
 def hook(uc,address,size,user):
  if address not in hooks:return
  name,n,fn=hooks[address];sp=uc.reg_read(UC_X86_REG_ESP);args=[get(sp+4+i*4) for i in range(abs(n))];result=fn(args);put_result=0 if result is None else result
  uc.reg_write(UC_X86_REG_EAX,put_result&0xffffffff);uc.reg_write(UC_X86_REG_ESP,sp+4+(n*4 if n>=0 else 0));uc.reg_write(UC_X86_REG_EIP,get(sp))
 emu.hook_add(UC_HOOK_CODE,hook)
 def call(at,args=(),this=0):
  sp=STACK+32768;array(sp,[STOP,*args]);put(0,0xffffffff);emu.reg_write(UC_X86_REG_ESP,sp);emu.reg_write(UC_X86_REG_ECX,this);emu.reg_write(UC_X86_REG_EFLAGS,2);emu.emu_start(at,STOP,count=3000000)
  if emu.reg_read(UC_X86_REG_EIP)!=STOP:raise RuntimeError(hex(at)+' did not return')
  return emu.reg_read(UC_X86_REG_EAX)
 # Replace IAT entries by named, bounded services, retaining original code.
 def readstr(at):
  b=bytearray()
  while emu.mem_read(at,1)[0]:b+=emu.mem_read(at,1);at+=1
  return b.decode('ascii')
 iat={};imp_rva=struct.unpack_from('<I',raw,optional+104)[0];descriptor=base+imp_rva
 while get(descriptor):
  original=get(descriptor);first=get(descriptor+16);i=0
  while get(base+original+i*4):
   entry=get(base+original+i*4)
   if not entry&0x80000000:iat[readstr(base+entry+2)]=base+first+i*4
   i+=1
  descriptor+=20
 def api(name,n,fn):put(iat[name],bind(name,n,fn))
 emu.mem_map(0xb00000,0x400000);emu.mem_map(0x1000000,0x1000000)
 rng=random.Random(0x434798);rows=[];counts={}
 def hx(b):return b.hex() or '-'
 def row(kind,values):rows.append(kind+' '+' '.join(v if isinstance(v,str) else f'{v&0xffffffff:x}' for v in values));counts[kind]=counts.get(kind,0)+1
 def direct(at,n,fn):ptr=bind(hex(at),n,fn);hooks[at]=hooks.pop(ptr)
 def signed(n):return n if n<0x80000000 else n-0x100000000
 # Full original LD12 multiply, including original limb helpers.
 for i in range(2400):
  exponent=rng.choice([0,1,0x7fff,rng.randrange(0x7fff),0x3fff,0x3fc0,0x4000]);other=rng.choice([0,1,0x7fff,rng.randrange(0x7fff),0x3fff,0x4000]);a12=[rng.randrange(65536) for _ in range(5)]+[exponent|rng.choice([0,0x8000])];b12=[rng.randrange(65536) for _ in range(5)]+[other|rng.choice([0,0x8000])]
  if i%23==0:a12[:5]=[0]*5
  if i%31==0:b12[:5]=[0]*5
  emu.mem_write(DATA,struct.pack('<6H',*a12));emu.mem_write(DATA+32,struct.pack('<6H',*b12));call(0x43938b,[DATA,DATA+32]);row('MUL',[*a12,*b12,*struct.unpack('<6H',emu.mem_read(DATA,12))])
 for i in range(1200):
  value=[rng.randrange(65536) for _ in range(5)]+[rng.choice([0,1,0x3fff,0x4000,0x7fff])|rng.choice([0,0x8000])];exponent=rng.randrange(-5000,5001);retain=i%2;emu.mem_write(DATA,struct.pack('<6H',*value));call(0x4395ab,[DATA,exponent,retain]);row('SCALE',[*value,exponent,retain,*struct.unpack('<6H',emu.mem_read(DATA,12))])
 trace=[];steps=fail=heap_cursor=reserve_cursor=0;allocations={};reserves={};committed=set();retry=0
 def event(id,*args):
  nonlocal steps
  trace.extend([id,*args]);steps+=1;return steps==fail
 def allocate_native(x):
  nonlocal heap_cursor
  if event(1,x[1],x[2]):return 0
  p=heap_cursor;heap_cursor+=(x[2]+15)&~15;allocations[p]=x[2];emu.mem_write(p,bytes([0 if x[1]&8 else 0xcc])*x[2]);return p
 def resize_native(x):
  nonlocal heap_cursor
  if event(2,x[2],x[3]):return 0
  p=heap_cursor;heap_cursor+=(x[3]+15)&~15;allocations[p]=x[3];old=allocations[x[2]];emu.mem_write(p,bytes(emu.mem_read(x[2],min(old,x[3])))+bytes([0xcc])*max(0,x[3]-old));return p
 def virtual_allocate(x):
  nonlocal reserve_cursor
  address,n,kind,protect=x
  if event(3,*x):return 0
  if kind==0x2000:
   p=reserve_cursor
   if p+n>0x2000000:return 0
   reserve_cursor+=(n+0xfffff)&~0xfffff;reserves[p]=n;return p
  if address<0x1000000 or address+n>0x2000000:raise RuntimeError(f"bad commit {address:x} {n:x} reserve={reserve_cursor:x} steps={steps}")
  for page in range(address,address+n,4096):
   if page not in committed:emu.mem_write(page,b'\0'*4096);committed.add(page)
  return address
 def virtual_free(x):
  if event(4,*x):return 0
  p,n,kind=x
  if kind==0x8000:n=reserves.get(p,0)
  for page in range(p,p+n,4096):committed.discard(page)
  return 1
 api('HeapAlloc',3,allocate_native);api('HeapReAlloc',4,resize_native);api('HeapFree',3,lambda x:(event(5,x[2]),1)[1]);api('VirtualAlloc',4,virtual_allocate);api('VirtualFree',3,virtual_free)
 def new_handler(x):
  nonlocal retry
  event(6,x[0]);old=retry;retry=0;return old
 direct(0x435610,-1,new_handler)
 def reset(mode,fail_at):
  nonlocal heap_cursor,reserve_cursor,steps,fail,retry
  heap_cursor=0xb00140;reserve_cursor=0x1000000;steps=0;fail=fail_at;retry=0;trace.clear();allocations.clear();allocations[0xb00000]=320;reserves.clear();committed.clear();emu.mem_write(0xb00000,b'\xcc'*0x400000);emu.mem_write(0x1000000,b'\xcc'*0x1000000);emu.mem_write(0x447748,b'\0'*8224);array(0x447748,[0x447748,0x447748,0,0,0xffffffff,0]);put(0x449768,0x447748);put(0x51c444,0);put(0x51da40,0x55);put(0x51da44,mode);put(0x51da3c,1016);put(0x44976c,480);array(0x51da24,[16,0,0xb00000,0,0,0xb00000]);put(0x51c448,0)
 def checksum():
  crc=zlib.crc32(emu.mem_read(0x447748,8224));crc=zlib.crc32(emu.mem_read(0xb00000,heap_cursor-0xb00000),crc)
  for p in sorted(committed):crc=zlib.crc32(emu.mem_read(p,4096),crc)
  return crc
 def op(kind,args):
  trace.clear();address={'ALLOC':0x430dd1,'FREE':0x430d2a,'REALLOC':0x439b8b,'A3':0x434798,'A2':0x43523b}[kind];ret=call(address,args);out=0 if kind=='FREE' else ret;row('HOP',[kind,len(args),*args,out,get(0x449768),get(0x51c444),get(0x51da38),get(0x51da24),get(0x51da34),get(0x51da2c),get(0x51da30),get(0x51da28),heap_cursor,reserve_cursor,checksum(),len(trace),*trace]);return ret
 for mode in [2,3]:
  for case in range(10):
   fail_at=case if case<7 else 0;reset(mode,fail_at);row('HBEGIN',[mode,fail_at]);live=[]
   for i in range(60):
    if live and i%4==0:p=live.pop(rng.randrange(len(live)));op('FREE',[p])
    elif live and i%3==0:
     j=rng.randrange(len(live));n=rng.choice([0,16,100,300,600,1000,2000]);p=op('REALLOC',[live[j],n]);
     if p:live[j]=p
     elif not n:live.pop(j)
    else:
     n=rng.choice([0,1,15,16,80,200,300,479,480,600,1000,1016]);p=op('ALLOC',[n]);
     if p:live.append(p)
   for p in live:op('FREE',[p])
 # Repeated page recycling reaches the original 32-page reclamation trigger.
 reset(2,0);row('HBEGIN',[2,0])
 for i in range(100):p=op('ALLOC',[16]);op('FREE',[p])
 # Multiple groups and regions, then delayed spare-group/region compaction.
 reset(3,0);row('HBEGIN',[3,0]);live=[op('A3',[1000]) for _ in range(800)]
 fail=steps+1;row('HCONFIG',[fail,0,0])
 for p in live:op('FREE',[p])
 for mode in [0,2,3]:
  reset(mode,0);row('HBEGIN',[mode,0]);p=op('ALLOC',[32]);fail=steps+1;retry=1;put(0x51c448,1);row('HCONFIG',[fail,1,1]);q=op('REALLOC',[p,9000]);op('FREE',[q or p])
 # Small page-slot fragments and direct grow/shrink boundary conditions.
 for i in range(300):
  page=0x1000000;region=DATA+0x1000;emu.mem_write(page,b'\0'*4096);emu.mem_write(region,b'\0'*8224);put(region+16,page);put(page,page+8);put(page+4,240);emu.mem_write(page+248,b'\xff');n=rng.randrange(1,80);ret=call(0x435443,[page,240,n]);row('PAGE',[n,ret,hx(bytes(emu.mem_read(page,256)))])
 for i in range(300):
  page=0x1000000;region=DATA+0x1000;emu.mem_write(page,b'\0'*4096);emu.mem_write(region,b'\0'*8224);put(region+16,page);put(page,page+8);put(page+4,240);emu.mem_write(page+248,b'\xff');slot=page+8+rng.randrange(100);old=rng.randrange(1,50);new=rng.randrange(1,100);emu.mem_write(slot,bytes([old]));obstacle=slot+old+rng.randrange(20);emu.mem_write(obstacle,b'\x05');array(region+24,[200,241]);ret=call(0x435567,[region,page,slot,new]);row('RSLOT',[slot-page,old,new,obstacle-page,ret,hx(bytes(emu.mem_read(page,256))),get(region+24),get(region+28)])
 # Ordinary C locale and original signed/unsigned integer conversion.
 classes=[0]+[(4 if 48<=c<=57 else 0)|(8 if c in [9,10,11,12,13,32] else 0)|(1 if 65<=c<=90 else 2 if 97<=c<=122 else 0) for c in range(256)];emu.mem_write(DATA+0x7000,struct.pack('<257H',*classes));put(0x449b1c,DATA+0x7002);put(0x449b10,1);put(0x51c680,0)
 for i in range(1200):
  text=rng.choice(['',' ',' -','0x','0xg','99999999999999999999999999','-4294967295','2147483648','-2147483649','  +123end','0777','0xdeadBEEF','zzzzzz','-80000000'])+rng.choice(['','x','123']);base=rng.choice([-1,0,1,2,8,10,16,36,37]);flags=rng.randrange(2);emu.mem_write(DATA,text.encode()+b'\0');put(0x51c3c8,71);put(DATA+0x100,0);ret=call(0x437eca,[DATA,DATA+0x100,base,flags]);row('PARSE',[hx(text.encode()),base,flags,ret,get(DATA+0x100)-DATA,get(0x51c3c8)])
 # Heap selection and diagnostic IAT boundaries.
 major=4;platform=1;version_ok=True;environment='';module='';module_ok=True;trace=[]
 def ver(x):event(7);put(x[0]+4,major);put(x[0]+16,platform);return int(version_ok)
 def env(x):event(8);emu.mem_write(x[1],environment.encode()+b'\0');return len(environment)
 def module_name(x):event(9);emu.mem_write(x[1],module.encode()+b'\0');return len(module) if module_ok else 0
 api('GetModuleHandleA',1,lambda x:0x400000);api('GetVersionExA',1,ver);api('GetEnvironmentVariableA',3,env);api('GetModuleFileNameA',3,module_name)
 for i in range(200):
  major=rng.choice([4,5,6]);platform=rng.choice([1,2]);version_ok=i%5!=0;environment=rng.choice(['','__GLOBAL_HEAP_SELECTED,1','__GLOBAL_HEAP_SELECTED,2;X','C:\\GAME.EXE,3','c:\\game.exe,2','__GLOBAL_HEAP_SELECTED,9','__GLOBAL_HEAP_SELECTED, -2']);module='C:\\game.exe';module_ok=True;linker=rng.randrange(4,8);# Patch the original optional-header major byte only.
  pe_offset=get(0x40003c);emu.mem_write(0x400000+pe_offset+26,bytes([linker]));trace.clear();put(0x51c3c8,71);ret=call(0x434257);row('SELECT',[major,platform,int(version_ok),hx(environment.encode()),hx(module.encode()),linker,ret,get(0x51c3c8),len(trace),*trace])
 # Full original message loader is exercised with named procedures.
 captured=[];msg=bind('MessageBoxA',4,lambda x:(captured.append(readstr(x[1])),1)[1]);api('LoadLibraryA',1,lambda x:0x80);api('GetProcAddress',2,lambda x:msg if readstr(x[1])=='MessageBoxA' else 0);api('GetStdHandle',1,lambda x:0x90)
 def write_error(x):captured.append(bytes(emu.mem_read(x[1],x[2])).decode('ascii'));put(x[3],x[2]);return 1
 api('WriteFile',5,write_error)
 for i in range(180):
  code=rng.choice([2,8,9,10,16,17,18,19,24,25,26,27,28,120,121,122,252,255,999]);mode=rng.randrange(3);console=bool(i%2);module='C:\\'+('longpath\\'*8 if i%3==0 else '')+'game.exe';module_ok=i%7!=0;array(0x51c66c,[0,0,0]);put(0x51c41c,mode);put(0x447594,int(console));captured.clear();call(0x436049,[code]);row('REPORT',[code,mode,int(console),hx(module.encode()),int(module_ok),len(captured),*[hx(x.encode()) for x in captured]])
 a.out.write_text('\n'.join(rows)+'\n');a.out.with_suffix('.json').write_text(json.dumps({'retail_sha256':SHA,'rows':len(rows),'comparisons':sum(n for k,n in counts.items() if k not in ['HBEGIN','HCONFIG']),'counts':counts,'scope':'SHA-verified original x86 instructions with bounded allocation/virtual-memory/NLS/IAT providers; source parity, not retail ABI/game certification.'},indent=2)+'\n');print(json.dumps(counts))
if __name__=='__main__':main()

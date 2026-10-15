#!/usr/bin/env python3
"""Execute SHA-verified retail x86 bodies with explicit file, clock and heap dependencies."""
import argparse, hashlib, json, random, struct
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
 rng=random.Random(0x433961);rows=[];counts={}
 def hx(b):return b.hex() or '-'
 def row(kind,fields):rows.append(kind+' '+' '.join(str(x) if isinstance(x,str) else f'{x&0xffffffff:x}' for x in fields));counts[kind]=counts.get(kind,0)+1
 def direct(at,n,fn):ptr=bind(hex(at),n,fn);hooks[at]=hooks.pop(ptr)
 def signed(n):return n if n<0x80000000 else n-0x100000000
 def hashbytes(b):
  value=2166136261
  for x in b:value=((value^x)*16777619)&0xffffffff
  return value
 errors=[get(0x4475b8+i*4) for i in range((0x447720-0x4475b8)//4)];row('ERRORS',[len(errors),*errors])
 trace=[];step=fail_at=last_error=0;error_code=5;position=0;read_quantum=write_quantum=64;file_type=1;file=bytearray()
 def event(id,*args):
  nonlocal step,last_error
  trace.extend([id,*args]);step+=1
  if step==fail_at:last_error=error_code;return True
  return False
 def read(x):
  nonlocal position
  if event(1,x[2]):put(x[3],0);return 0
  n=min(x[2],read_quantum,max(0,len(file)-position));put(x[3],n)
  if n:emu.mem_write(x[1],bytes(file[position:position+n]));position+=n
  return 1
 def write(x):
  nonlocal position
  if event(2,x[2]):put(x[3],0);return 0
  n=min(x[2],write_quantum);put(x[3],n)
  if position+n>len(file):file.extend(b'\0'*(position+n-len(file)))
  if n:file[position:position+n]=emu.mem_read(x[1],n);position+=n
  return 1
 def seek(x):
  nonlocal position,last_error
  if event(3,x[1],x[3]):return 0xffffffff
  result=signed(x[1])+(position if x[3]==1 else len(file) if x[3]==2 else 0)
  if result<0:last_error=131;return 0xffffffff
  if result>16384:raise RuntimeError('unbounded seek')
  position=result;return position
 def create(x):return 0xffffffff if event(4,x[1],x[2],x[4],x[5],get(x[3]+8)) else 0x77
 def ftype(x):return 0 if event(5) else file_type
 def close(x):return int(not event(6))
 def setend(x):
  if event(7):return 0
  if position<len(file):del file[position:]
  else:file.extend(b'\0'*(position-len(file)))
  return 1
 api('ReadFile',5,read);api('WriteFile',5,write);api('SetFilePointer',4,seek);api('CreateFileA',7,create);api('GetFileType',1,ftype);api('CloseHandle',1,close);api('SetEndOfFile',1,setend);api('GetLastError',0,lambda x:last_error)
 api('SetStdHandle',2,lambda x:(event(8,*x),1)[1])
 direct(0x430d93,-1,lambda x:0 if event(9,x[0]) else DATA+0x8000)
 def reset(flags,look,initial,pos,rq,wq,fail,err,typ,stream_address,words):
  nonlocal file,position,read_quantum,write_quantum,fail_at,error_code,file_type,step,last_error
  file=bytearray(initial);position=pos;read_quantum=rq;write_quantum=wq;fail_at=fail;error_code=err;file_type=typ;step=last_error=0;trace.clear();emu.mem_write(DATA,b'\xcc'*65536);array(0x51d920,[DATA+0x6000]+[0]*63);put(0x51da20,32);put(0x447594,1);put(0x51c3c8,71);put(0x51c3cc,72);put(0x51c568,0)
  for i in range(32):array(DATA+0x6000+i*8,[0xffffffff,0xa00])
  array(DATA+0x6000,[0x77,flags|(look<<8)]);array(stream_address,words)
 def result_fields(ret,stream):return [ret,get(0x51c3c8),get(0x51c3cc),get(DATA+0x6000),get(DATA+0x6004)&255,(get(DATA+0x6004)>>8)&255,position,hx(file),*[get(stream+j*4) for j in range(8)],get(0x51c568),hashbytes(emu.mem_read(DATA+0x1000,512)),hashbytes(emu.mem_read(DATA+0x3000,4096)),hashbytes(emu.mem_read(DATA+0x8000,4096)),len(trace),*trace]
 for kind,address in [('READ',0x433961),('WRITE',0x43407d),('RESIZE',0x4390cc),('OPEN',0x437bfa),('FLUSH',0x43285c),('FREAD',0x430937),('FWRITE',0x430a52)]:
  for i in range(360):
   flags=rng.choice([0,1,0x81,0x83,0x89,0xc1,0xa1,0xe1,0xc9]);look=rng.choice([10,13,65]);fd=32 if i%19==0 else 0;initial=bytes(rng.choice([10,13,26,65,66,67]) for _ in range(rng.randrange(100)));pos=rng.randrange(len(initial)+1);rq=rng.choice([1,2,7,64,4096]);wq=rng.choice([1,2,7,64,4096]);fail=i%12;err=rng.choice([2,5,6,87,109,131]);typ=rng.choice([0,1,2,3]);stream=0x4498b0 if i%17==0 else DATA+0x2000
   fflags=rng.choice([1,2,0x82,0x81,0x40,0x42,0x89,0x8a,0x10a,0x112]);capacity=rng.choice([2,8,16,4096]);used=rng.randrange(min(16,capacity));available=rng.randrange(min(16,capacity));words=[DATA+0x3000+used,available,DATA+0x3000,fflags,fd,0xaabbccdd,capacity,0x12345678]
   if i%5==0:words[0]=words[2]=0;words[1]=0;words[3]=1 if kind=='FREAD' else 2
   n=rng.randrange(200);size=rng.choice([0,1,2,3,4]);count=rng.randrange(40);payload=bytes(rng.choice([10,13,26,65,66]) for _ in range(200));open_flags=rng.choice([0,1,2,3])|rng.choice([0,0x100,0x200,0x300,0x400,0x500,0x600,0x700])|rng.choice([0,0x80,0x8000,0x4000])|rng.choice([0,8,0x10,0x20,0x40,0x1000]);share=rng.choice([0x10,0x20,0x30,0x40,0]);permissions=rng.choice([0,0x80,0x1a4]);mask=rng.choice([0,0x80]);default=rng.choice([0x4000,0x8000]);length=rng.choice([-1,0,1,50,100,4097]);value=rng.randrange(256)
   if kind in ['FLUSH','FREAD','FWRITE']:fd=0;words[4]=0
   if kind=='FWRITE':wq=4096
   reset(flags,look,initial,pos,rq,wq,fail,err,typ,stream,words)
   if kind in ['WRITE','FWRITE']:emu.mem_write(DATA+0x1000,payload)
   if kind=='READ':ret=call(address,[fd,DATA+0x1000,n]);extra=[n]
   elif kind=='WRITE':ret=call(address,[fd,DATA+0x1000,n]);extra=[n,hx(payload)]
   elif kind=='RESIZE':ret=call(address,[fd,length]);extra=[length]
   elif kind=='OPEN':put(DATA+0x6000,0xffffffff);emu.mem_write(DATA+0x6004,b'\0');emu.mem_write(DATA,b'test\0');put(0x51c6b0,default);put(0x51c3d0,mask);ret=call(address,[DATA,open_flags,share,permissions]);extra=[open_flags,share,permissions,mask,default]
   elif kind=='FLUSH':ret=call(address,[value,stream]);extra=[value]
   else:ret=call(address,[DATA+0x1000,size,count,stream]);extra=[size,count]+([hx(payload)] if kind=='FWRITE' else [])
   row(kind,[flags,look,fd,hx(initial),pos,rq,wq,fail,err,typ,stream,*words,*extra,*result_fields(ret,stream)])
 # Time zone and directory providers; CTYPE and environment functions remain real.
 classes=[0]+[(4 if 48<=c<=57 else 0)|(8 if c in [9,10,13,32] else 0) for c in range(256)];emu.mem_write(DATA+0x7000,struct.pack('<257H',*classes));put(0x449b1c,DATA+0x7002);put(0x449b10,1);put(0x51c680,0)
 # Environment comparison stays a required NLS boundary.
 def nls(x):
  a=bytes(emu.mem_read(x[2],x[3])).lower();b=bytes(emu.mem_read(x[4],x[5])).lower();return 1 if a<b else 3 if a>b else 2
 direct(0x43969d,-7,nls)
 free_count=alloc_count=0;allocation_failure=False
 def allocate(x):
  nonlocal alloc_count
  alloc_count+=1;return 0 if allocation_failure else DATA+0x8000
 def release(x):
  nonlocal free_count
  free_count+=1
 direct(0x430d93,-1,allocate);direct(0x430d2a,-1,release)
 zone_data=b'';zone_result=0;conversion_fail=0;convert_count=0
 api('GetTimeZoneInformation',1,lambda x:(emu.mem_write(x[0],zone_data),zone_result)[1])
 def convert(x):
  nonlocal convert_count
  convert_count+=1;put(x[7],int(conversion_fail==convert_count+2))
  if conversion_fail==convert_count:return 0
  src=bytes(emu.mem_read(x[2],64)).decode('utf-16-le').split('\0')[0].encode('ascii')+b'\0';emu.mem_write(x[4],src);return len(src)
 api('WideCharToMultiByte',8,convert)
 for i in range(300):
  tz=rng.choice(['UTC0','EST5EDT','PST-8','AAA+5:30:10BBB','XYZ2:04','EST5','XXX-02:15:00DST','']);cached=i%5==0;allocation_failure=i%11==0;alloc_count=free_count=convert_count=0;array(DATA+0x6000,[DATA+0x6100,0]);emu.mem_write(DATA+0x6100,('TZ='+tz).encode()+b'\0');put(0x51c3f0,DATA+0x6000);put(0x51c3f4,0);put(0x51c3f8,0);put(0x51da4c,1);emu.mem_write(DATA+0x6200,tz.encode()+b'\0');put(0x51c624,DATA+0x6200 if cached else 0);put(0x449dbc,DATA+0x6300);put(0x449dc0,DATA+0x6400);emu.mem_write(DATA+0x6300,b'STD'+b'\0'*61);emu.mem_write(DATA+0x6400,b'DST'+b'\0'*61);array(0x449d30,[1234,1,0xfffff1f0]);put(0x51c570,1);put(0x449dc8,5);put(0x449dd8,6);call(0x436789);row('TZ',[hx(tz.encode()),int(cached),int(allocation_failure),get(0x449d30),get(0x449d34),get(0x449d38),get(0x51c570),get(0x449dc8),get(0x449dd8),hx(bytes(emu.mem_read(DATA+0x6300,64))),hx(bytes(emu.mem_read(DATA+0x6400,64))),alloc_count,free_count])
 for i in range(120):
  zone_result=0xffffffff if i%11==0 else i%3;conversion_fail=i%5;convert_count=0;bias=rng.randrange(-720,721);standard_bias=rng.choice([-60,0,30]);daylight_bias=rng.choice([-60,0,60]);standard_month=rng.choice([0,3,11]);daylight_month=rng.choice([0,3,11]);z=bytearray(172);struct.pack_into('<i',z,0,bias);z[4:12]='STD\0'.encode('utf-16-le');struct.pack_into('<H',z,70,standard_month);struct.pack_into('<i',z,84,standard_bias);z[88:96]='DST\0'.encode('utf-16-le');struct.pack_into('<H',z,154,daylight_month);struct.pack_into('<i',z,168,daylight_bias);zone_data=bytes(z);put(0x51c3f0,DATA+0x6000);put(DATA+0x6000,0);put(0x51c3f8,0);put(0x51c624,0);put(0x51c690,1252);put(0x449dbc,DATA+0x6300);put(0x449dc0,DATA+0x6400);emu.mem_write(DATA+0x6300,b'STD'+b'\0'*61);emu.mem_write(DATA+0x6400,b'DST'+b'\0'*61);array(0x449d30,[1234,1,0xfffff1f0]);put(0x51c570,1);put(0x449dc8,5);put(0x449dd8,6);call(0x436789);row('ZONE',[zone_result,conversion_fail,hx(zone_data),get(0x449d30),get(0x449d34),get(0x449d38),get(0x51c570),get(0x449dc8),get(0x449dd8),hx(bytes(emu.mem_read(DATA+0x6300,64))),hx(bytes(emu.mem_read(DATA+0x6400,64))),convert_count])
 directory='';dir_fail=0;dir_trace=[]
 api('SetCurrentDirectoryA',1,lambda x:(dir_trace.append(1),int(dir_fail!=1))[1])
 def getdir(x):
  dir_trace.append(2)
  if dir_fail==2:return 0
  emu.mem_write(x[1],directory.encode()+b'\0');return len(directory)
 api('GetCurrentDirectoryA',2,getdir)
 def setvar(x):dir_trace.extend([3,len(readstr(x[0]))]);return int(dir_fail!=3)
 api('SetEnvironmentVariableA',2,setvar)
 api('GetLastError',0,lambda x:5);emu.mem_write(0x51c7e0,bytes([0]+[32 if 97<=i<=122 else 0 for i in range(256)]));emu.mem_write(0x51c6e0,bytes(i-32 if 97<=i<=122 else i for i in range(256)))
 for i in range(120):
  directory=rng.choice(['c:\\games','D:\\x','\\\\server\\share','//server/share','/path']);dir_fail=i%4;dir_trace.clear();put(0x51c3c8,71);put(0x51c3cc,72);emu.mem_write(DATA,b'x\0');ret=call(0x4300be,[DATA]);row('CHDIR',[hx(directory.encode()),dir_fail,ret,get(0x51c3c8),get(0x51c3cc),len(dir_trace),*dir_trace])
 # Small heap 2 lookup/release/decommit and small heap 3 region construction.
 heap_trace=[];heap_step=heap_fail=0
 def hevent(id,*values):
  nonlocal heap_step
  heap_trace.extend([id,*values]);heap_step+=1;return heap_step==heap_fail
 api('VirtualFree',3,lambda x:int(not hevent(14,*x)))
 api('HeapFree',3,lambda x:(hevent(15,x[0],x[2]),1)[1])
 api('HeapReAlloc',4,lambda x:0 if hevent(11,x[0],x[2],x[3]) else DATA+0x2000)
 api('HeapAlloc',3,lambda x:0 if hevent(12,*x) else DATA+0x7000)
 api('VirtualAlloc',4,lambda x:0 if hevent(13,x[1],x[2],x[3]) else 0x3000000)
 head=0x447748;region=DATA+0x1000
 def heap_reset():
  nonlocal heap_step
  heap_step=0;heap_trace.clear();emu.mem_write(head,b'\0'*8224);emu.mem_write(region,b'\0'*8224);array(head,[region,region,0,0,0x1000000,0x1400000]);array(region,[head,head,0,0,0x2000000,0x2400000]);put(0x51da40,0x55);put(0x449768,region);put(0x51c444,6)
  for ptr in [head,region]:
   for j in range(1024):put(ptr+24+j*8,0xffffffff)
 for i in range(500):
  heap_reset();pointer=rng.choice([0x1000000,0x1000001,0x1000100,0x1000ff0,0x2000100,0x23fffe0,0x2400000,0x4000000])+rng.choice([0,0,0,1]);array(DATA,[0xdead,0xbeef]);ret=call(0x43519f,[pointer,DATA,DATA+4]);row('HLOOK',[pointer,ret,get(DATA),get(DATA+4)])
 for i in range(120):
  heap_reset();which=head if i%2 else region;current=which if i%3 else 0x123;put(0x449768,current);heap_fail=i%3;call(0x435087,[which]);row('HRELEASE',[which,current,heap_fail,get(0x449768),get(head),get(head+4),get(head+16),get(region),get(region+4),get(region+16),len(heap_trace),*heap_trace])
 for i in range(120):
  heap_reset();free_slots=rng.sample(range(1024),3)
  for ptr in [head,region]:
   for j in free_slots:put(ptr+24+j*8,240)
  requested=rng.choice([-1,0,1,2,3,4,6,100]);heap_fail=i%8;call(0x4350dd,[requested]);row('HDECOMMIT',[requested,heap_fail,*free_slots,get(0x449768),get(0x51c444),get(head),get(head+4),get(head+12),get(head+16),get(region),get(region+4),get(region+12),get(region+16),hashbytes(emu.mem_read(head+24,8192)),hashbytes(emu.mem_read(region+24,8192)),len(heap_trace),*heap_trace])
 for i in range(120):
  heap_step=0;heap_fail=i%4;heap_trace.clear();count=i%3;capacity=count if i%2 else count+4;put(0x51da34,count);put(0x51da24,capacity);put(0x51da38,DATA+0x1000);emu.mem_write(DATA+0x1000,b'\xcc'*0x5000);emu.mem_write(DATA+0x7000,b'\xcc'*0x41c4);ret=call(0x434aa1);table=get(0x51da38);record=table+count*20;row('HADD',[count,capacity,heap_fail,ret,get(0x51da34),get(0x51da24),table,*[get(record+j*4) for j in range(5)],get(DATA+0x7000),len(heap_trace),*heap_trace])
 a.out.write_text('\n'.join(rows)+'\n');a.out.with_suffix('.json').write_text(json.dumps({'retail_sha256':SHA,'cases':len(rows)-1,'counts':counts,'scope':'Original x86 bodies with controlled native file, time, NLS, allocation and virtual memory services; not full game or ABI verification.'},indent=2)+'\n');print(json.dumps(counts))
if __name__=='__main__':main()

#!/usr/bin/env python3
"""Execute SHA-verified retail x86 bodies with explicit allocation, Win32 and COM dependencies."""
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
  sp=STACK+32768;array(sp,[STOP,*args]);put(0,0xffffffff);emu.reg_write(UC_X86_REG_ESP,sp);emu.reg_write(UC_X86_REG_ECX,this);emu.reg_write(UC_X86_REG_EFLAGS,2);emu.emu_start(at,STOP,count=100000)
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
 rng=random.Random(0x430b5c);rows=[];counts={}
 def hx(b):return b.hex() or '-'
 def row(kind,fields):rows.append(kind+' '+' '.join(str(x) if isinstance(x,str) else f'{x&0xffffffff:x}' for x in fields));counts[kind]=counts.get(kind,0)+1
 def direct(at,n,fn):
  ptr=bind(hex(at),n,fn);hooks[at]=hooks.pop(ptr)
 def signed(n):return n if n<0x80000000 else n-0x100000000
 for i in range(1000):
  b=bytes(rng.randrange(256) for _ in range(512));source=rng.randrange(256);dest=rng.randrange(256);n=rng.randrange(257);emu.mem_write(DATA,b);result=call(0x433b60,[DATA+dest,DATA+source,n]);row('COPY',[source,dest,n,hx(b),result-DATA,hx(bytes(emu.mem_read(DATA,512)))])
 trace=[]
 def comparator(args):
  a,b=args;trace.extend([(a-DATA)//width,(b-DATA)//width]);x,y=signed(get(a)),signed(get(b));return (x>y)-(x<y)
 comparator_ptr=bind('qsort_compare',-2,comparator)
 for i in range(600):
  count=rng.randrange(100);width=rng.choice([4,5,8,12]);b=b''.join(struct.pack('<i',rng.randrange(-8,9))+bytes([j])* (width-4) for j in range(count));emu.mem_write(DATA,b or b'\0');trace.clear();call(0x430b5c,[DATA,count,width,comparator_ptr]);row('SORT',[count,width,hx(b),hx(bytes(emu.mem_read(DATA,len(b)))),len(trace),*trace])
 trace=[];small=0;found=False
 def event(id,*values):trace.extend([id,*values])
 direct(0x434798,-1,lambda x:(event(1,x[0]),small)[1]);direct(0x43523b,-1,lambda x:(event(2,x[0]),small)[1])
 api('HeapAlloc',3,lambda x:(event(3,x[0],x[2]),0x1234)[1]);api('HeapFree',3,lambda x:(event(4,x[0],x[2]),1)[1])
 direct(0x434444,-1,lambda x:(event(5,x[0]),0x12 if found else 0)[1]);direct(0x43446f,-2,lambda x:event(6,*x))
 def find2(x):
  event(7,x[0]);put(x[1],0x56);put(x[2],0x78);return 0x9a if found else 0
 direct(0x43519f,-3,find2);direct(0x4351f6,-3,lambda x:event(8,*x))
 for i in range(500):
  mode=rng.randrange(5);n=rng.choice([0,1,15,16,17,511,512,513,0xfffffff1]);t2=rng.choice([0,256,512]);t3=rng.choice([0,256,512]);small=rng.choice([0,0x2345]);found=bool(rng.randrange(2));put(0x51da44,mode);put(0x51da40,0xab);put(0x51da3c,t3);put(0x44976c,t2);trace.clear();ret=call(0x430dd1,[n]);row('ALLOC',[mode,n,t2,t3,small,ret,len(trace),*trace]);trace.clear();ptr=rng.choice([0,0x1234]);call(0x430d2a,[ptr]);row('FREE',[mode,ptr,int(found),len(trace),*trace])
 # Runtime dynamic MessageBox loader. Two calls prove caching and retries.
 trace=[];missing=0;owner=0;message=bind('MessageBoxA',4,lambda x:(event(7,x[0],x[3]),0x44)[1]);active=bind('GetActiveWindow',0,lambda x:(event(5),owner)[1]);popup=bind('GetLastActivePopup',1,lambda x:(event(6,x[0]),x[0]+1)[1])
 api('LoadLibraryA',1,lambda x:(event(1),0 if missing==1 else 0x80)[1])
 def proc(x):
  name=readstr(x[1]);id={'MessageBoxA':2,'GetActiveWindow':3,'GetLastActivePopup':4}[name];event(id);return 0 if missing==id else {'MessageBoxA':message,'GetActiveWindow':active,'GetLastActivePopup':popup}[name]
 api('GetProcAddress',2,proc)
 for missing in range(5):
  for owner in [0,0x90]:
   array(0x51c66c,[0,0,0]);trace.clear();r1=call(0x438272,[DATA,DATA,0x12010]);r2=call(0x438272,[DATA,DATA,0x12010]);row('MESSAGE',[missing,owner,r1,r2,len(trace),*trace])
 # Geometry: original x87 including __ftol is executed, never replaced.
 caps=[640,480,300,300,96,96];trace=[]
 def cap(x):event(1,x[0],x[1]);return caps[4 if x[0]==2 and x[1]==88 else 5 if x[0]==2 and x[1]==90 else {8:0,10:1,88:2,90:3}[x[1]]]
 api('GetDeviceCaps',2,cap);api('GetDC',1,lambda x:(event(2,x[0]),2)[1])
 def intersect(x):
  aa=[signed(get(x[1]+j*4)) for j in range(4)];bb=[signed(get(x[2]+j*4)) for j in range(4)];rr=[max(aa[0],bb[0]),max(aa[1],bb[1]),min(aa[2],bb[2]),min(aa[3],bb[3])];valid=rr[0]<rr[2] and rr[1]<rr[3];array(x[0],rr if valid else [0]*4);return int(valid)
 api('IntersectRect',3,intersect)
 for i in range(500):
  mode=i%5;w=rng.randrange(1,2000);h=rng.choice([-1,1])*rng.randrange(1,2000);caps[:]=[rng.randrange(300,6000),rng.randrange(300,6000),rng.choice([72,96,150,300,600]),rng.choice([72,96,150,300,600]),rng.choice([72,96,120]),rng.choice([72,96,120])];array(DATA+0x100,[40,w,h]);put(DATA+4,DATA+0x100);array(DATA+0x110,[11,12,13,14]);put(DATA+0x120,mode);put(0x482570,0x777);trace.clear();call(0x405490,[1],DATA);row('TARGET',[mode,w,h,*caps,*[get(DATA+0x110+j*4) for j in range(4)],len(trace),*trace])
 for i in range(600):
  target=[rng.randrange(-100,100),rng.randrange(-100,100),rng.randrange(200,1000),rng.randrange(200,1000)];dest=[rng.randrange(-200,1000),rng.randrange(-200,1000),rng.randrange(200,1300),rng.randrange(200,1300)];source=[rng.randrange(-100,0),rng.randrange(-100,0),rng.randrange(100,1500),rng.randrange(100,1500)];array(DATA+0x110,target);array(DATA+0x200,dest);array(DATA+0x220,source);ret=call(0x4056c0,[DATA+0x200,DATA+0x220],DATA);row('CLIP',[*target,*dest,*source,int((ret&255)!=0),*[get(DATA+0x200+j*4) for j in range(4)],*[get(DATA+0x220+j*4) for j in range(4)]])
 # Environment: original get/put/conversion, with only allocator/NLS boundaries hooked.
 allocation=DATA+0x4000;alloc_count=free_count=resize_count=0;fail=0;os_trace=[]
 def alloc(x):
  nonlocal allocation,alloc_count
  alloc_count+=1
  if alloc_count==fail:return 0
  ptr=allocation;allocation+=(x[0]+15)&~15;emu.mem_write(ptr,b'\0'*max(1,x[0]));return ptr
 direct(0x430d93,-1,alloc)
 def free(x):
  nonlocal free_count
  free_count+=1
 direct(0x430d2a,-1,free)
 def resize(x):
  nonlocal resize_count
  resize_count+=1;return x[0] # Tests provision large backing, retain logical old elements.
 direct(0x439b8b,-2,resize)
 def nls(x):
  a=bytes(emu.mem_read(x[2],x[3])).lower();b=bytes(emu.mem_read(x[4],x[5])).lower();return 1 if a<b else 3 if a>b else 2
 direct(0x43969d,-7,nls)
 api('SetEnvironmentVariableA',2,lambda x:os_trace.append((readstr(x[0]),readstr(x[1]) if x[1] else '-')))
 wide_bytes=b'';narrow_bytes=b'';wide_ok=True;narrow_ok=True;conversion_fail=0;convert_calls=0;block_frees=0
 api('GetEnvironmentStringsW',0,lambda x:DATA+0x1800 if wide_ok else 0);api('GetEnvironmentStrings',0,lambda x:DATA+0x2000 if narrow_ok else 0)
 def blockfree(x):
  nonlocal block_frees
  block_frees+=1;return 1
 api('FreeEnvironmentStringsW',1,blockfree);api('FreeEnvironmentStringsA',1,blockfree)
 def convert(x):
  nonlocal convert_calls
  convert_calls+=1
  if conversion_fail==convert_calls:return 0
  n=signed(x[3]);b=bytearray();at=x[2]
  if n==-1:
   while True:
    value=struct.unpack('<H',emu.mem_read(at,2))[0];at+=2;b.append(value&255)
    if value==0:break
  else:b=bytearray(struct.unpack('<'+'H'*n,emu.mem_read(at,n*2)))
  if x[4]:emu.mem_write(x[4],bytes(b))
  return len(b)
 api('WideCharToMultiByte',8,convert)
 for i in range(240):
  mode=i%4;wide_ok=i%3!=0;narrow_ok=i%5!=0;conversion_fail=i%4;allocation=DATA+0x4000;alloc_count=free_count=block_frees=convert_calls=0;fail=1 if i%7==0 else 0
  b=b'A=one\0B=two\0\0';emu.mem_write(DATA+0x1800,b.decode('ascii').encode('utf-16-le'));emu.mem_write(DATA+0x2000,b);put(0x51c560,mode)
  ret=call(0x435c51);length=1 if b==b'\0' else len(b);row('BLOCK',[mode,int(wide_ok),int(narrow_ok),conversion_fail,fail,int(ret!=0),get(0x51c560),alloc_count,free_count,block_frees,convert_calls,hx(bytes(emu.mem_read(ret,length))) if ret else '-'])
 for i in range(300):
  initial=rng.choice([[],['A=one'],['A=one','B=two','C=three']]);text=rng.choice(['A=new','B=','D=four','bad','=bad','a=','D=']);update=i%2;allocation=DATA+0x4000;alloc_count=free_count=resize_count=0;os_trace.clear();fail=1 if i%13==0 else 0
  emu.mem_write(DATA+0x1000,b'\0'*256);pointers=[]
  for j,value in enumerate(initial):ptr=DATA+0x1200+j*128;emu.mem_write(ptr,value.encode()+b'\0');pointers.append(ptr)
  array(DATA+0x1000,[*pointers,0]);put(0x51c3f0,DATA+0x1000);put(0x51c3f4,0);put(0x51c3f8,DATA+0x1100);put(DATA+0x1100,0);put(0x51c6cc,0);emu.mem_write(DATA,text.encode()+b'\0');ret=call(0x439945,[DATA,update]);entries=[];ptr=get(0x51c3f0)
  while get(ptr):entries.append(readstr(get(ptr)));ptr+=4
  put(0x51da4c,1);emu.mem_write(DATA+0x100,b'A\0');value=call(0x43874a,[DATA+0x100]);row('ENV',[len(initial),*[hx(x.encode()) for x in initial],hx(text.encode()),update,fail,ret,len(entries),*[hx(x.encode()) for x in entries],alloc_count,free_count,resize_count,len(os_trace),*[v for pair in os_trace for v in [hx(pair[0].encode()),hx(pair[1].encode())]],hx(readstr(value).encode()) if value else '-'])
 for i in range(120):
  allocation=DATA+0x4000;alloc_count=free_count=resize_count=0;fail=1 if i%5==0 else 0;conversion_fail=i%7;convert_calls=0;os_trace.clear();put(0x51c3f0,DATA+0x1000);put(0x51c3f4,0);put(DATA+0x1000,0);array(DATA+0x1100,[DATA+0x1800,DATA+0x1900,0]);put(0x51c3f8,DATA+0x1100);emu.mem_write(DATA+0x1800,'A=one\0'.encode('utf-16-le'));emu.mem_write(DATA+0x1900,'B=two\0'.encode('utf-16-le'));ret=call(0x43931d);entries=[];ptr=get(0x51c3f0)
  while get(ptr):entries.append(readstr(get(ptr)));ptr+=4
  row('WIDE',[fail,conversion_fail,ret,alloc_count,free_count,resize_count,convert_calls,len(entries),*[hx(x.encode()) for x in entries]])
 # Sound constructor and creation control flow, with previously tested wave/fill boundaries.
 alloc_sizes=[];audio_trace=[];new_failure=0;new_count=0;audio_heap=DATA+0x3000;first_empty=False;fallback_empty=False;open_count=0;create_code=0;duplicate_failure=0;count=1
 def audio_new(x):
  nonlocal new_count,audio_heap
  new_count+=1;alloc_sizes.append(x[0])
  if new_count==new_failure:return 0
  ptr=audio_heap;audio_heap+=max(256,x[0]);emu.mem_write(ptr,b'\0'*max(256,x[0]));return ptr
 direct(0x42fbce,-1,audio_new);direct(0x42fbdc,-1,lambda x:0)
 def wave_open(x):
  nonlocal open_count
  open_count+=1;wave=emu.reg_read(UC_X86_REG_ECX);put(wave,DATA+0x1600);put(wave+0x7c,1);put(wave+0x30,0 if (first_empty if open_count==1 else fallback_empty) else 8);return 0x80004005
 direct(0x404560,3,wave_open);direct(0x404b30,0,lambda x:0)
 def fill(x):audio_trace.extend([10,x[1]]);return 0x80004005
 direct(0x404100,2,fill)
 def position(x):audio_trace.extend([11,x[0]-DATA-0x2000,x[1]]);return 0x80004005
 position_ptr=bind('SetCurrentPosition',2,position);array(DATA+0x2500,[0]*32);put(DATA+0x2500+0x34,position_ptr)
 def create_buffer(x):
  audio_trace.extend([1,get(x[1]),get(x[1]+4),get(x[1]+8),*[get(x[1]+20+j*4) for j in range(4)]]);put(x[2],DATA+0x2000);put(DATA+0x2000,DATA+0x2500);return create_code
 def duplicate(x):
  idx=(x[2]-get(0x51c550))//4;audio_trace.extend([2,idx]);put(x[2],DATA+0x2000+idx*16);put(DATA+0x2000+idx*16,DATA+0x2500);return 0x80004005 if idx==duplicate_failure else 0
 create_ptr=bind('CreateSoundBuffer',4,create_buffer);duplicate_ptr=bind('DuplicateSoundBuffer',3,duplicate);array(DATA+0x2600,[0]*8);put(DATA+0x260c,create_ptr);put(DATA+0x2614,duplicate_ptr);put(DATA+0x2700,DATA+0x2600);put(DATA+0x2800,DATA+0x2700)
 for i in range(180):
  count=1+i%4;audio_heap=DATA+0x3000;new_count=0;new_failure=0;alloc_sizes.clear();audio_trace.clear();array(DATA+0x1000,[DATA+0x2000+j*16 for j in range(count)])
  for j in range(count):put(DATA+0x2000+j*16,DATA+0x2500)
  call(0x403ff0,[DATA+0x1000,8,count,DATA+0x1500],DATA);row('SOUND',[count,*[get(DATA+j*4) if j not in [1,3] else 0 for j in range(5)],len(alloc_sizes),*alloc_sizes,len(audio_trace),*audio_trace])
 for i in range(300):
  count=1+i%4;audio_heap=DATA+0x3000;put(0x51c550,audio_heap);new_count=open_count=0;new_failure=i%4;alloc_sizes.clear();audio_trace.clear();first_empty=i%3==0;fallback_empty=i%5==0;cd=3 if i%2 else -1;create_code=[0,1,0x878000a,0x80004005][i%4];duplicate_failure=i%4;emu.mem_write(DATA+0x100,b'test.wav\0');put(DATA+0x180,0xdead)
  put(0x43e874,cd);result=call(0x403d20,[DATA+0x180,DATA+0x100,0xface,1,2,3,4,count],DATA+0x2800);out=get(DATA+0x180);state=[]
  if out not in [0,0xdead]:state=[get(out),get(out+8),get(out+16)]
  row('CREATE',[count,new_failure,int(first_empty),int(fallback_empty),cd,create_code,duplicate_failure,result,0 if out==0 else 1 if out==0xdead else 2,open_count,len(state),*state,len(alloc_sizes),*alloc_sizes,len(audio_trace),*audio_trace])
 a.out.write_text('\n'.join(rows)+'\n');a.out.with_suffix('.json').write_text(json.dumps({'retail_sha256':SHA,'cases':len(rows),'counts':counts,'scope':'Original x86 bodies with explicit named dependency providers; not full retail verification.'},indent=2)+'\n');print(json.dumps(counts))
if __name__=='__main__':main()

#!/usr/bin/env python3
"""Execute original x86 display, window, device import and gameplay bodies; no C++ oracle."""
import argparse,hashlib,json,random,struct
from pathlib import Path
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import *
from build_retail_pure_vectors import SHA,STACK,DATA,STOP

def main():
 p=argparse.ArgumentParser();p.add_argument('exe',type=Path);p.add_argument('--out',type=Path,required=True);a=p.parse_args();raw=a.exe.read_bytes()
 if hashlib.sha256(raw).hexdigest()!=SHA:raise ValueError('wrong retail executable')
 pe=struct.unpack_from('<I',raw,60)[0];opt=pe+24;ns=struct.unpack_from('<H',raw,pe+6)[0];os=struct.unpack_from('<H',raw,pe+20)[0];base=struct.unpack_from('<I',raw,opt+28)[0];size=struct.unpack_from('<I',raw,opt+56)[0]
 u=Uc(UC_ARCH_X86,UC_MODE_32);u.mem_map(base,(size+4095)&~4095);u.mem_write(base,raw[:4096]);u.mem_map(0,4096)
 for i in range(ns):
  _,rva,n,off=struct.unpack_from('<IIII',raw,opt+os+i*40+8)
  if n:u.mem_write(base+rva,raw[off:off+n])
 for at,n in [(STACK,65536),(DATA,65536),(STOP,8192)]:u.mem_map(at,n)
 def put(at,v):u.mem_write(at,struct.pack('<I',v&0xffffffff))
 def get(at):return struct.unpack('<I',u.mem_read(at,4))[0]
 def array(at,vs):
  for i,v in enumerate(vs):put(at+i*4,v)
 def signed(v):return v if v<0x80000000 else v-0x100000000
 import zlib
 hooks={};trace=[];rows=[];counts={};rng=random.Random(0x403080);fail=steps=surface_count=0;wr=[-100,-100,600,500];style=0x80c80000;work=[10,20,1920,1080];args=[]
 DRAW,PRIMARY,BACK,CLIP=DATA+0x1000,DATA+0x2000,DATA+0x3000,DATA+0x4000
 def signed(v):return v if v<0x80000000 else v-0x100000000
 def event(*v):trace.extend(x&0xffffffff for x in v)
 def values(at,n):return [get(at+i*4) for i in range(n)]
 def string(at):return bytes(u.mem_read(at,256)).split(b'\0',1)[0]
 def text_hash(at):return zlib.crc32(string(at))
 def status():
  nonlocal steps
  steps+=1;return 0x88760001 if fail==steps else 1
 def bind(at,n,fn):hooks[at]=(n,fn)
 def hook(uc,at,size,user):
  if at not in hooks:return
  n,fn=hooks[at];sp=uc.reg_read(UC_X86_REG_ESP);a=[get(sp+4+i*4) for i in range(abs(n))];result=fn(a);uc.reg_write(UC_X86_REG_EAX,(result or 0)&0xffffffff);uc.reg_write(UC_X86_REG_ESP,sp+4+max(n,0)*4);uc.reg_write(UC_X86_REG_EIP,get(sp))
 u.hook_add(UC_HOOK_CODE,hook)
 iat={};desc=base+struct.unpack_from('<I',raw,opt+104)[0]
 while get(desc):
  original,first=get(desc),get(desc+16);i=0
  while get(base+original+i*4):
   entry=get(base+original+i*4)
   if not entry&0x80000000:iat[string(base+entry+2).decode()]=base+first+i*4
   i+=1
  desc+=20
 def api(name,n,fn):at=STOP+0x100+len(hooks)*16;bind(at,n,fn);put(iat[name],at)
 def reset(a):event(1);return 0
 bind(STOP+0x1000,0,reset)
 def create_draw(a):
  event(2,0 if not a[0] else zlib.crc32(bytes(u.mem_read(a[0],16))),*values(a[2],4),a[3]);result=status()
  if signed(result)>=0:put(a[1],DRAW)
  return result
 api('DirectDrawCreateEx',4,create_draw)
 def create_input(a):
  event(21,a[0],a[1],*values(a[2],4),a[4]);result=status()
  if signed(result)>=0:put(a[3],DATA+0x6000)
  return result
 api('DirectInput8Create',5,create_input)
 def create_sound(a):
  event(22,0 if not a[0] else zlib.crc32(bytes(u.mem_read(a[0],16))),a[2]);result=status()
  if signed(result)>=0:put(a[1],DATA+0x6100)
  return result
 put(0x43b020,STOP+0x1010);bind(STOP+0x1010,3,create_sound)
 def cooperative(a):event(3,*a);return status()
 def display_mode(a):event(4,*a);return status()
 def surface(a):
  nonlocal surface_count
  desc=values(a[1],31);assert all(not x for i,x in enumerate(desc) if i not in [0,1,2,3,5,26]);event(5,a[0],desc[0],desc[1],desc[2],desc[3],desc[5],desc[26]);result=status()
  if signed(result)>=0:put(a[2],PRIMARY if not surface_count else BACK);surface_count+=1
  return result
 def attached(a):
  event(6,a[0],*values(a[1],4));result=status()
  if signed(result)>=0:put(a[2],BACK)
  return result
 def add_ref(a):event(7,*a);return 2
 def create_clip(a):
  event(8,a[0],a[1],a[3]);result=status()
  if signed(result)>=0:put(a[2],CLIP)
  return result
 def clip_window(a):event(9,*a);return status()
 def set_clip(a):event(10,*a);return status()
 def release(a):event(11,*a);return 0
 for off,n,fn in [(0x50,3,cooperative),(0x54,6,display_mode),(0x18,4,surface),(0x10,4,create_clip)]:bind(STOP+0x1100+off,n,fn)
 for off,n,fn in [(0x30,3,attached),(0x70,2,set_clip)]:bind(STOP+0x1200+off,n,fn)
 bind(STOP+0x1304,1,add_ref);bind(STOP+0x1408,1,release);bind(STOP+0x1420,3,clip_window)
 def destination(a):
  event(12);array(DATA+24,[0,0,args[1],args[2]] if not get(DATA+40) else [wr[0]+7,wr[1]+30,wr[0]+7+args[1],wr[1]+30+args[2]]);return 0
 bind(0x403590,0,destination)
 def get_long(a):event(13,*a);return style if signed(a[1])==-16 else 0x200
 def set_long(a):
  nonlocal style
  event(14,*a);style=a[2];return 0
 api('GetWindowLongA',2,get_long);api('SetWindowLongA',3,set_long)
 api('GetMenu',1,lambda a:(event(15,*a),0x77)[1])
 api('SetRect',5,lambda a:(array(a[0],a[1:]),1)[1])
 def adjust(a):
  v=values(a[0],4);event(16,*v,*a[1:]);array(a[0],[signed(v[0])-7,signed(v[1])-30,signed(v[2])+7,signed(v[3])+7]);return 1
 api('AdjustWindowRectEx',4,adjust)
 def position(a):
  nonlocal wr
  event(17,*a);x,y,w,h=map(signed,a[2:6]);oldw,oldh=wr[2]-wr[0],wr[3]-wr[1]
  if not a[6]&2:wr[0],wr[1]=x,y
  wr[2]=wr[0]+(oldw if a[6]&1 else w);wr[3]=wr[1]+(oldh if a[6]&1 else h);return 1
 api('SetWindowPos',7,position)
 api('SystemParametersInfoA',4,lambda a:(event(18,a[0],a[1],a[3]),array(a[2],work),1)[2])
 def window_rect(a):event(19,a[0]);array(a[1],wr);return 1
 api('GetWindowRect',2,window_rect)
 # Original class creation import boundary.
 api('FindWindowA',2,lambda a:(event(100,text_hash(a[0]),text_hash(a[1])),get(DATA+0xf000))[1])
 api('LoadIconA',2,lambda a:(event(101,*a),0x400+a[1])[1]);api('LoadCursorA',2,lambda a:(event(102,*a),0x500+a[1])[1])
 def register(a):
  v=values(a[0],12);event(104,*v[:10],text_hash(v[10]),v[11]);return 0 if get(DATA+0xf004)==1 else 1
 api('RegisterClassExA',1,register)
 api('LoadAcceleratorsA',2,lambda a:(event(105,*a),0x501)[1])
 api('GetSystemMetrics',1,lambda a:(event(106,*a),{32:8,33:8,15:19,4:25}[a[0]])[1])
 def create_window(a):event(107,a[0],text_hash(a[1]),text_hash(a[2]),*a[3:]);return 0 if get(DATA+0xf004)==2 else 0x1234
 api('CreateWindowExA',12,create_window);api('ShowWindow',2,lambda a:event(108,*a));api('UpdateWindow',1,lambda a:event(109,*a))
 # Bitmap loader and registry are explicit dependencies, including a retail wrong-slot registration.
 bitmap_count=0
 def bitmap(a):
  nonlocal bitmap_count
  bitmap_count+=1;event(30,a[0],text_hash(a[1]),a[2],a[3]);return 0 if fail==bitmap_count else 0xa000+text_hash(a[1])%4096
 bind(0x4038d0,-4,bitmap);bind(0x403770,-2,lambda a:event(31,a[0],get(a[0]),text_hash(a[1])));bind(0x4036e0,-1,lambda a:event(32,*a));bind(0x403b70,-2,lambda a:event(33,*a))
 def init():
  nonlocal trace,steps,surface_count,wr,style,bitmap_count
  trace=[];steps=surface_count=bitmap_count=0;wr=[-100,-100,600,500];style=0x80c80000;u.mem_write(DATA,bytes(65536));u.mem_write(STACK,bytes(65536));array(0x44ddf8,[77,88,99,111]);array(DATA,[DATA+0x100,*([0]*11)]);put(DATA+0x100,STOP+0x1000)
  for obj,vt in [(DRAW,DATA+0x1100),(PRIMARY,DATA+0x2100),(BACK,DATA+0x3100),(CLIP,DATA+0x4100)]:put(obj,vt)
  for off in [0x50,0x54,0x18,0x10]:put(DATA+0x1100+off,STOP+0x1100+off)
  for off in [0x30,0x70]:put(DATA+0x2100+off,STOP+0x1200+off)
  put(DATA+0x3104,STOP+0x1304);put(DATA+0x4108,STOP+0x1408);put(DATA+0x4120,STOP+0x1420)
 def call(at,a,this=DATA):
  array(STACK+32768,[STOP,*a]);u.reg_write(UC_X86_REG_ESP,STACK+32768);u.reg_write(UC_X86_REG_ECX,this);u.reg_write(UC_X86_REG_EFLAGS,2);u.reg_write(UC_X86_REG_FPCW,0x37f);u.reg_write(UC_X86_REG_FPSW,0);u.reg_write(UC_X86_REG_FPTAG,0xffff);u.emu_start(at,STOP,count=100000)
  if u.reg_read(UC_X86_REG_EIP)!=STOP:raise RuntimeError(hex(at)+' failed')
  return u.reg_read(UC_X86_REG_EAX)
 def row(k,v):rows.append(k+' '+' '.join(f'{x&0xffffffff:x}' for x in v));counts[k]=counts.get(k,0)+1
 for k,n,at in [('FULL',200,0x403080),('WIN',300,0x4031e0)]:
  for i in range(n):
   init();fail=i%11;args=[0x1234,rng.choice([320,640,800]),rng.choice([240,480,600]),rng.choice([16,24,32])];result=call(at,args if k=='FULL' else args[:3]);row(k,[fail,*args,result,*values(DATA+4,10),len(trace),*trace])
 for i in range(200):
  init();exists=i%5==0;failure=i%3;put(DATA+0xf000,int(exists));put(DATA+0xf004,failure);array(DATA+0x8000,[0xaaaa,0xbbbb]);result=call(0x401e20,[0x99,5,DATA+0x8000,DATA+0x8004]);row('WINDOW',[int(exists),failure,result,*values(DATA+0x8000,2),*values(0x44ddf8,4),len(trace),*trace])
 for k,n,at in [('DRAW',100,0x42ead0),('INPUT',100,0x42fb50),('SOUND',100,0x42fb56)]:
  for i in range(n):
   init();fail=i%2;guid=[rng.getrandbits(32) for _ in range(4)];array(DATA+0x7000,guid);put(DATA+0x7010,0xeeee)
   call_args=[0,DATA+0x7010,DATA+0x7000,0] if k=='DRAW' else [0x99,0x800,DATA+0x7000,DATA+0x7010,0] if k=='INPUT' else [0,DATA+0x7010,0]
   result=call(at,call_args);row(k,[fail,*guid,result,get(DATA+0x7010),len(trace),*trace])
 slots=[0x50ab18,0x50ab1c,0x50aaa8,0x50ab08,0x50a49c,0x50a4a0,0x50a4a4,0x50a4a8,0x50a4ac,0x50a4b0,0x50a5c4,0x50a5c8,0x50a494,0x50a498]
 for i in range(80):
  init();fail=i%14;put(0x44de08,DATA);put(DATA+4,DRAW);call(0x4112a0,[]);row('FIRE',[fail,*[get(p) for p in slots],len(trace),*trace])
 for i in range(2500):
  init();left=rng.choice([-1,-1,0,2,3,4,5]);right=rng.choice([-1,-1,0,2,3,4,5]);moving=rng.randrange(2);mode=rng.randrange(9,15);direction=rng.randrange(4);x=rng.uniform(-100,800);y=rng.uniform(-100,600);nx=rng.randrange(-100,801);ny=rng.randrange(-100,601);speed=rng.randrange(-3,12)
  put(0x445ef8,left);put(0x445efc,right);put(0x5144d4,moving);put(0x510d18,0);array(0x510e18,[nx,ny]);a32=[struct.unpack('<I',struct.pack('<f',v))[0] for v in [x,y]]+[0,0,0,direction,0,mode];array(DATA+0x7000,a32);call(0x4232b0,[0,DATA+0x7000,speed]);row('MOVE',[left,right,moving,speed,nx,ny,*a32,*values(DATA+0x7000,8)])
 a.out.parent.mkdir(parents=True,exist_ok=True);a.out.write_text('\n'.join(rows)+'\n');a.out.with_suffix('.json').write_text(json.dumps({'exe_sha256':SHA,'rows':len(rows),'counts':counts,'scope':'Unmodified original display/window/import/bitmap/movement instructions; controlled Win32/COM/resource dependencies. Movement float comparisons permit a bounded final-float tolerance across host transcendental implementations; native device/retail ABI parity not certified.'},indent=2)+'\n');print(len(rows),counts)
if __name__=='__main__':main()

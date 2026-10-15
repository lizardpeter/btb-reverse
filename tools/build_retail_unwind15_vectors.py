#!/usr/bin/env python3
"""Execute original x86 exception, conversion and heap-size bodies; no C++ oracle."""
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
 hooks={};trace=[];kind='';done=False;notices=(0x44754c,0x447554,0x447550)
 def bind(at,n,fn):hooks[at]=(n,fn)
 def hook(uc,at,n,user):
  nonlocal done
  if at not in hooks:return
  count,fn=hooks[at];sp=uc.reg_read(UC_X86_REG_ESP);args=[get(sp+4+i*4) for i in range(abs(count))];v=fn(args)
  if done:uc.emu_stop();return
  uc.reg_write(UC_X86_REG_EAX,(v or 0)&0xffffffff);uc.reg_write(UC_X86_REG_EIP,get(sp));uc.reg_write(UC_X86_REG_ESP,sp+4+max(0,count)*4)
 u.hook_add(UC_HOOK_CODE,hook)
 def event(*values):trace.extend(v&0xffffffff for v in values)
 def frame(args):
  at=u.reg_read(UC_X86_REG_EIP);bp=u.reg_read(UC_X86_REG_EBP);reg=get(0);handler=get(reg+4)
  event(2,at,bp,*[get(x) for x in notices],handler)
  if kind=='LOCAL':event(get(reg+8),get(reg+12),get(DATA+12))
  elif kind=='CATCH':event(get(reg+16),get(reg+12),get(reg+8))
  elif kind=='CPP':event(get(reg+12),get(DATA+8))
  return at^bp^0x12345678
 def obj(args):
  at=u.reg_read(UC_X86_REG_EIP);ob=u.reg_read(UC_X86_REG_ECX);event(1,at,ob,len(args),*args);return at^ob^sum(args)
 def rtl(args):event(3,*args);put(0,DATA+0x9000);return 0
 def raise_exception(args):event(8,args[0],args[1],args[2],*[get(args[3]+i*4) for i in range(args[2])]);return 0
 def handler(args):event(4,u.reg_read(UC_X86_REG_EIP));return 0
 def report(args):event(5,args[0]);return 0
 def signal(args):event(6,args[0]);return 0
 def exit_fn(args):
  nonlocal done
  event(7,*args);done=True;return 0
 def hsize(args):event(9,*args);return (args[2]^0x12345678)&0xffffffff
 bind(0x43a220,4,rtl);bind(0x436049,-1,report);bind(0x43854d,-1,signal);bind(0x430884,-3,exit_fn)
 # Real IAT thunks retain their original calling conventions.
 put(0x43b11c,STOP+0x100);bind(STOP+0x100,4,raise_exception)
 # HeapSize IAT: recover the import name from the image.
 desc=base+struct.unpack_from('<I',raw,opt+104)[0]
 def cstr(at):return bytes(u.mem_read(at,200)).split(b'\0',1)[0].decode()
 while get(desc):
  original,first=get(desc),get(desc+16);i=0
  while get(base+original+i*4):
   entry=get(base+original+i*4)
   if not entry&0x80000000 and cstr(base+entry+2)=='HeapSize':put(base+first+i*4,STOP+0x120)
   i+=1
  desc+=20
 bind(STOP+0x120,3,hsize)
 for i in range(32):bind(STOP+0x300+i*16,0,frame)
 bind(STOP+0x180,0,handler);bind(STOP+0x190,0,handler)
 rows=[];counts={};rng=random.Random(0x42ff0e)
 def call(at,args=(),bp=DATA+0xf000):
  nonlocal done
  done=False;array(STACK+32768,[STOP,*args]);put(0,DATA+0x8000);u.reg_write(UC_X86_REG_ESP,STACK+32768);u.reg_write(UC_X86_REG_EBP,bp);u.reg_write(UC_X86_REG_EFLAGS,2);u.emu_start(at,STOP,count=100000)
  if not done and u.reg_read(UC_X86_REG_EIP)!=STOP:raise RuntimeError(kind+' did not return')
  return u.reg_read(UC_X86_REG_EAX)
 def run(k,at,args,mem,outputs=(),bp=DATA+0xf000,returns=True):
  nonlocal kind,trace
  kind=k;trace=[];u.mem_write(DATA,bytes(65536));u.mem_write(STACK,bytes(65536));put(0,DATA+0x8000)
  for x in notices:put(x,0)
  put(0x51c42c,0);put(0x4475b4,0)
  array(DATA+0x8000,[DATA+0x9000,0x777,0xffffffff,DATA+0x500]);array(DATA+0x9000,[0xffffffff,0x888,0xffffffff,DATA+0x600])
  for x,v in mem:put(x,v)
  result=call(at,args,bp)
  words=[len(args),*args,len(mem),*[x for pair in mem for x in pair],len(outputs),*[x for address in outputs for x in (address,get(address))],len(trace),*trace]
  words += [int(done)]
  if returns and not done:words += [result]
  # Normalized chain and notification state are independently checked too.
  words += [get(0),*[get(x) for x in notices]]
  rows.append(k+' '+' '.join(f'{x&0xffffffff:x}' for x in words));counts[k]=counts.get(k,0)+1
 for i in range(300):
  ob,info=rng.getrandbits(32),rng.getrandbits(32);run('THROW',0x430495,[ob,info],[],returns=False)
 for i in range(300):
  n=rng.randrange(4);args=[rng.getrandbits(32) for _ in range(n)];target=STOP+0x200;bind(target,n,obj);ob=rng.getrandbits(32);address=rng.choice([0x42fc1b,0x42fc22]);run('OBJ',address,[ob,target,*args],[])
 for i in range(300):
  frame_address=DATA;info=DATA+0x1000;target=STOP+0x300+rng.randrange(32)*16;nest=rng.randrange(-2,5);status=rng.choice([0x100,0x101,0x103,0]);run('CATCH',0x42fcae,[frame_address,info,target,nest,status],[])
 for i in range(400):
  count=rng.randrange(1,25);level=rng.randrange(-1,count);stop=rng.randrange(-1,level+1) if level>=0 else -1;mem=[(DATA+8,DATA+0x1000),(DATA+12,level),(DATA+0xf008,rng.getrandbits(32))];table=[]
  for j in range(count):table += [(DATA+0x1000+j*12,j-1),(DATA+0x1004+j*12,rng.randrange(2)),(DATA+0x1008+j*12,STOP+0x300+j*16)]
  run('LOCAL',0x42ff0e,[DATA,stop],mem+table,[DATA+12],returns=False)
 for i in range(400):
  count=rng.randrange(1,25);level=rng.randrange(-1,count);stop=rng.randrange(-1,level+1) if level>=0 else -1;info=DATA+0x2000;table=DATA+0x1000;mem=[(DATA+8,level),(info+4,count),(info+8,table)]
  for j in range(count):mem += [(table+j*8,j-1),(table+j*8+4,0 if rng.randrange(4)==0 else STOP+0x300+j*16)]
  run('CPP',0x43210c,[DATA,0,info,stop],mem,[DATA+8],returns=False)
 for i in range(300):
  frame_address=DATA+0x500;handler_address=rng.choice([0x42feec,0x42fd02,0x777]);level=rng.randrange(-2,8);compare=level if i%2 else rng.randrange(-2,8)
  mem=[(DATA+0x8004,handler_address),(DATA+0x8008,level),(DATA+0x800c,frame_address),(frame_address+12,compare)];run('ABNORMAL',0x42ff76,[],mem)
 for i in range(160):
  record=DATA+0x1000;flags=rng.getrandbits(32);run('UNWIND',0x42fc29,[DATA+0x500,record],[(record+4,flags)],[record+4,DATA+0x8000],returns=False)
 for i in range(80):run('GLOBAL',0x42fecc,[rng.getrandbits(32)],[],returns=False)
 for i in range(200):
  record=DATA+0x1000 if i%5 else 0;target=STOP+0x220 if i%3 else 0;bind(STOP+0x220,0,obj);mem=[]
  if record:mem=[(record+24,rng.getrandbits(32)),(record+28,DATA+0x2000),(DATA+0x2004,target)]
  run('DESTROY',0x432535,[record],mem,returns=False)
 for i in range(80):
  unexpected=i%2;mem=[(0x51c42c,STOP+0x180 if i&2 else 0),(0x4475b4,STOP+0x190 if i&4 else 0)]
  run('UNEXPECTED' if unexpected else 'TERMINATE',0x432662 if unexpected else 0x43260c,[],mem,returns=False)
 # TryBlockMap interval selection, including count=0 and negative nesting.
 for i in range(400):
  count=rng.randrange(0,20);table=DATA+0x2000;info=DATA+0x1000;level=rng.randrange(-1,50);mem=[(info+12,count),(info+16,table),(table-16,-100),(table-12,100)]
  for j in range(count):mem += [(table+j*20+4,j*2),(table+j*20+8,j*2+3)]
  nesting=0 if count else -1
  run('RANGE',0x42fe50,[info,nesting,level,DATA+0x4000,DATA+0x4004],mem,[DATA+0x4000,DATA+0x4004])
 # _msize routes the exact small-heap lookups; no allocator helper is replaced.
 for i in range(600):
  mode=rng.choice([0,1,2,3]);pointer=DATA+0x3000+rng.randrange(256)*16
  if i%7==0:pointer+=1
  if i%11==0:pointer=DATA+0x1000
  mem=[(0x51da44,mode),(0x51da40,0x90),(0x51da34,1),(0x51da38,DATA+0x7000),(DATA+0x700c,DATA+0x3000),(pointer-4,rng.randrange(9,10000)),(0x447748,0x447748),(0x447758,DATA+0x3000),(0x44775c,DATA+0x6000)]
  slot=(pointer&~4095)+8+(((pointer&4095)-256)>>4);at=slot&~3;old=next((v for x,v in mem if x==at),0);mem.append((at,old|(rng.randrange(1,241)<<((slot&3)*8))))
  run('SIZE',0x43a2f5,[pointer],mem)
 # A small loader supplies ST(0) in the original x87 register ABI.
 loader=STOP+0x1000;code=b'\xdb\x2d'+struct.pack('<I',DATA)+b'\xe8'+struct.pack('<i',0x4304d0-(loader+11))+b'\xe9'+struct.pack('<i',STOP-(loader+16));u.mem_write(loader,code)
 for i in range(2500):
  exp=rng.choice([0,1,0x3ffe,0x3fff,0x4000,0x403d,0x403e,0x403f,0x7fff,rng.randrange(0x7fff)]);mantissa=rng.getrandbits(64)|(1<<63)
  if i%9==0:mantissa=1<<63
  if i%17==0:mantissa=0
  sign=rng.choice([0,0x8000]);cw=rng.choice([0x37f,0x77f,0xb7f,0xf7f]);u.mem_write(DATA,struct.pack('<QH',mantissa,exp|sign));u.reg_write(UC_X86_REG_FPCW,cw);u.reg_write(UC_X86_REG_FPSW,0);u.reg_write(UC_X86_REG_FPTAG,0xffff);call(loader)
  low,high=u.reg_read(UC_X86_REG_EAX),u.reg_read(UC_X86_REG_EDX);rows.append('FTOL '+' '.join(f'{x:x}' for x in [mantissa&0xffffffff,mantissa>>32,exp|sign,cw,low,high,u.reg_read(UC_X86_REG_FPCW)]));counts['FTOL']=counts.get('FTOL',0)+1
 a.out.parent.mkdir(parents=True,exist_ok=True);a.out.write_text('\n'.join(rows)+'\n');a.out.with_suffix('.json').write_text(json.dumps({'exe_sha256':SHA,'rows':len(rows),'counts':counts,'scope':'Original x86 instructions with controlled callback/Win32 boundaries. No host OS SEH, unmasked x87 trap or complete retail ABI certification.'},indent=2)+'\n');print(len(rows),counts)
if __name__=='__main__':main()

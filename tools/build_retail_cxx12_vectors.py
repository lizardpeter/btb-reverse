#!/usr/bin/env python3
"""Execute original x86 C++ exception dispatch and catch bodies; no C++ oracle."""
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
 hooks={};trace=[];kind='';mode=0;mem=[];rows=[];counts={};rng=random.Random(0x431dc5)
 REC,FRAME,INFO,TRY,HANDLER,THROW,LIST,TYPE,NAME,NAME2,OBJECT,REG= [DATA+x for x in [0x1000,0x2000,0x3000,0x4000,0x5000,0x6000,0x6100,0x6200,0x6400,0x6500,0x7000,0xa000]]
 def event(*values):trace.extend(v&0xffffffff for v in values)
 def stop(args):
  nonlocal mode
  event(7,*args);mode=1;return 0
 def resume(args):
  nonlocal mode
  event(12,u.reg_read(UC_X86_REG_EIP),u.reg_read(UC_X86_REG_ESP),u.reg_read(UC_X86_REG_EBP));mode=2;return 0
 def hook(uc,at,n,user):
  if at not in hooks:return
  count,fn=hooks[at];sp=uc.reg_read(UC_X86_REG_ESP);args=[get(sp+4+i*4) for i in range(abs(count))];result=fn(args)
  if mode:uc.emu_stop();return
  uc.reg_write(UC_X86_REG_EAX,(result or 0)&0xffffffff);uc.reg_write(UC_X86_REG_EIP,get(sp));uc.reg_write(UC_X86_REG_ESP,sp+4+max(0,count)*4)
 u.hook_add(UC_HOOK_CODE,hook)
 def bind(at,count,fn):hooks[at]=(count,fn)
 def probe(args,id,bit):event(id,*args);return 0 if get(DATA+0xf000)&bit else 1
 bind(0x43670d,-2,lambda x:probe(x,10,1));bind(0x436729,-2,lambda x:probe(x,11,2));bind(0x436745,-1,lambda x:probe(x,13,4))
 def obj(args):
  target=u.reg_read(UC_X86_REG_EIP);ob=u.reg_read(UC_X86_REG_ECX);event(1,target,ob,len(args),*args)
  if args:u.mem_write(ob,bytes(u.mem_read(args[0],4)))
  return 0x12345678
 bind(STOP+0x600,1,obj);bind(STOP+0x610,2,obj);bind(STOP+0x620,0,obj)
 def frame(args):
  target=u.reg_read(UC_X86_REG_EIP);bp=u.reg_read(UC_X86_REG_EBP)
  event(2,target,bp,get(0x44754c),get(0x447554),get(0x447550),get(0x51c420),get(0x51c424));put(FRAME-4,0xabcdef);return get(DATA+0xf004)
 bind(STOP+0x630,0,frame)
 def rtl(args):event(3,*args);put(0,DATA+0x9000);return 0
 bind(0x43a220,4,rtl)
 bind(0x436049,-1,lambda x:event(5,*x));bind(0x43854d,-1,lambda x:event(6,*x));bind(0x430884,-3,stop)
 bind(STOP+0x700,0,resume)
 def alternate(args):event(14,*args);return get(DATA+0xf004)
 bind(STOP+0x800,-8,alternate)
 def translation(args):
  reg=get(0);event(15,STOP+0x810,args[0],get(args[1]),get(args[1]+4),get(reg+4),get(reg+8),get(reg+12),get(reg+16),get(reg+20));return 0
 bind(STOP+0x810,-2,translation)
 def common():
  nonlocal mem
  mem=[]
  def set(at,value):mem.append((at,value));return value
  set(REC,0xe06d7363);set(REC+16,3);set(REC+20,0x19930520);set(REC+24,OBJECT);set(REC+28,THROW)
  set(FRAME-4,DATA+0xd000);set(FRAME+8,0);set(INFO,0x19930520);set(INFO+4,5);set(INFO+8,DATA+0x3100);set(INFO+12,1);set(INFO+16,TRY)
  for j in range(5):set(DATA+0x3100+j*8,j-1);set(DATA+0x3104+j*8,0)
  set(TRY,-1);set(TRY+4,1);set(TRY+8,3);set(TRY+12,1);set(TRY+16,HANDLER)
  set(TRY-16,-100);set(TRY-12,100)
  set(HANDLER+4,NAME);set(HANDLER+8,16);set(HANDLER+12,STOP+0x630)
  set(THROW+4,STOP+0x620);set(THROW+12,LIST);set(LIST,1);set(LIST+4,TYPE)
  set(TYPE,1);set(TYPE+4,NAME2);set(TYPE+8,0);set(TYPE+12,-1);set(TYPE+16,0);set(TYPE+20,4)
  set(NAME+8,0x006f6f66);set(NAME2+8,0x006f6f66)
  for j in range(32):set(OBJECT+j*4,0x800000+j*0x101)
  set(OBJECT,DATA+0x7400);set(OBJECT+16,DATA+0x7300);set(DATA+0x7308,8);set(DATA+0x7410,DATA+0x7300)
  set(REG+8,INFO);set(REG+12,FRAME);set(REG+16,0);set(REG+20,FRAME);set(REG+24,STOP+0x700);set(REG+28,DATA+0xd000);set(REG+32,DATA+0xe000)
  set(0x51c420,DATA+0x1100);set(0x51c424,0x1234);set(DATA+0xf004,0);return set
 outputs=[REC+4,FRAME-4,FRAME+8,0x51c420,0x51c424,REG+36]+[FRAME+28+j*4 for j in range(8)]
 def run(k,address,args,registers=()):
  nonlocal trace,kind,mode
  kind=k;trace=[];mode=0;u.mem_write(DATA,bytes(65536));u.mem_write(STACK,bytes(65536));put(0,DATA+0x8000);array(DATA+0x8000,[DATA+0x9000,0x777,0xffffffff,FRAME]);array(DATA+0x9000,[0xffffffff,0x888,0xffffffff,FRAME])
  for at in [0x44754c,0x447550,0x447554,0x4475b4,0x51c42c,0x51c428]:put(at,0)
  for at,value in mem:put(at,value)
  sp=STACK+32768;array(sp,[STOP,*args]);u.reg_write(UC_X86_REG_ESP,sp);u.reg_write(UC_X86_REG_EBP,DATA+0xe000);u.reg_write(UC_X86_REG_EFLAGS,2)
  for reg,value in registers:u.reg_write(reg,value)
  u.emu_start(address,STOP,count=100000)
  if not mode and u.reg_read(UC_X86_REG_EIP)!=STOP:raise RuntimeError(k+' failed to return')
  returns=k in ['INVOKE','DISPATCH','FRAME','CATCH_HANDLER','TRANSLATE','TRANSHANDLER'] and not mode
  words=[len(args),*args,len(mem),*[v for pair in mem for v in pair],len(outputs),*[v for at in outputs for v in (at,get(at))],len(trace),*trace,mode]
  if returns:words.append(u.reg_read(UC_X86_REG_EAX))
  words += [get(0),get(0x44754c),get(0x447554),get(0x447550)]
  rows.append(k+' '+' '.join(f'{x&0xffffffff:x}' for x in words));counts[k]=counts.get(k,0)+1
 for i in range(800):
  set=common();set(HANDLER,rng.choice([0,8]));set(TYPE,rng.choice([0,1,4]));set(TYPE+8,rng.randrange(4)*4);set(TYPE+12,rng.choice([-1,16]));set(TYPE+16,8);set(TYPE+20,rng.choice([4,8,12,16]));set(TYPE+24,rng.choice([0,STOP+0x600,STOP+0x610]));set(DATA+0xf000,rng.choice([0,0,0,1,2,4]))
  if dict(mem)[TYPE]&4:set(TYPE+24,STOP+0x610)
  elif dict(mem)[TYPE+24]:set(TYPE+24,STOP+0x600)
  if i%13==0:set(HANDLER+4,0)
  if i%17==0:set(NAME+8,0)
  if i%19==0:set(HANDLER+8,0)
  run('COPY',0x432371,[REC,FRAME,HANDLER,TYPE])
 for i in range(300):
  set=common();result=rng.choice([0,STOP+0x700]);set(DATA+0xe000-0x28,DATA+0xd000);set(DATA+0xe000-0x1c,DATA+0x1100);set(DATA+0xe000-0x20,0x1234);set(DATA+0xe000-0x24,i%2);set(DATA+0xe000-0x2c,result)
  if i%3==0:set(REC,0xc0000005)
  run('FINISH',0x432301,[REC,FRAME,result,i%2],[(UC_X86_REG_ESI,FRAME),(UC_X86_REG_EDI,REC),(UC_X86_REG_EBX,0)])
 for i in range(300):
  set=common();set(DATA+0xf004,rng.choice([0,STOP+0x700]));run('INVOKE',0x43223b,[REC,FRAME,0x123456,INFO,STOP+0x630,rng.randrange(3),rng.choice([0x100,0x103])])
 for k,n,address in [('DISPATCH',400,0x431dc5),('FRAME',200,0x42fc78),('CATCH_HANDLER',200,0x42fd02),('SEARCH',400,0x431e60),('FOREIGN',200,0x432007),('ENTER',200,0x4321c0)]:
  for i in range(n):
   set=common();set(DATA+0xf004,rng.choice([0,STOP+0x700]));translated=(k=='SEARCH' and i%6==0)
   if i%5==0:set(NAME2+8,0x00726162)
   if i%7==0:set(HANDLER+4,0)
   if k in ['DISPATCH','FRAME','CATCH_HANDLER']:
    set(REC+4,rng.choice([0,0,0,2,4,0x40]));set(INFO+12,rng.choice([0,1]))
    if i%11==0:set(INFO,0)
    if i%13==0:set(REC+20,0x19930521);set(THROW+8,STOP+0x800)
   if k=='FOREIGN':set(REC,0xc0000005);set(HANDLER+4,0);set(0x51c428,STOP+0x810 if i%3==0 else 0)
   if k=='SEARCH' and i%9==0:
    set(REC+28,0);set(0x51c420,DATA+0x1100);set(DATA+0x1100,0xe06d7363);set(DATA+0x1110,3);set(DATA+0x1114,0x19930520);set(DATA+0x1118,OBJECT);set(DATA+0x111c,THROW)
   if k=='DISPATCH':args=[REC,FRAME,0x123456,0x88,INFO,0,0,int(translated)]
   elif k=='FRAME':args=[REC,FRAME,0x123456,0x88]
   elif k=='CATCH_HANDLER':args=[REC,REG,0x123456]
   elif k=='SEARCH':args=[REC,FRAME,0x123456,0x88,INFO,int(translated),0,0]
   elif k=='FOREIGN':args=[REC,FRAME,0x123456,0x88,INFO,0,0,0]
   else:args=[REC,FRAME,0x123456,0x88,INFO,HANDLER,TYPE,TRY,0,0]
   run(k,address,args,[(UC_X86_REG_EAX,INFO)] if k=='FRAME' else [])
 for i in range(120):
  set=common();set(0x51c428,STOP+0x810);run('TRANSLATE',0x42fd27,[REC,FRAME,0x123456,0x88,INFO,i%3,FRAME])
 for i in range(200):
  set=common();set(REC+4,rng.choice([0,2,4,0x40]));set(INFO+12,0);run('TRANSHANDLER',0x42fddb,[REC,REG,0x123456])
 for i in range(100):
  set=common();run('RESUME',0x42fbe7,[STOP+0x700,FRAME])
 a.out.parent.mkdir(parents=True,exist_ok=True);a.out.write_text('\n'.join(rows)+'\n');a.out.with_suffix('.json').write_text(json.dumps({'exe_sha256':SHA,'rows':len(rows),'counts':counts,'scope':'Unmodified original x86 C++ dispatch bodies with explicit callback, pointer-probe, RtlUnwind and nonreturning exit/continuation boundaries; no OS exception dispatcher or full ABI certification.'},indent=2)+'\n');print(len(rows),counts)
if __name__=='__main__':main()

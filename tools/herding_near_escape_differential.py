#!/usr/bin/env python3
"""Execute original 00416A30 and compare the recovered near-animal branch.

Requires Unicorn, the verified retail executable and the compiled
btb_herding_near_escape_audit probe. Geometry/audio are controlled dependencies:
a broad group-two polygon avoids its independent pre-dispatch recovery loop.
This verifies this branch only, not the entire activity or full game.
"""
import argparse
import struct,hashlib,subprocess
from pathlib import Path
parser=argparse.ArgumentParser(description=__doc__)
parser.add_argument('--exe',type=Path,required=True)
parser.add_argument('--native',type=Path,required=True)
args=parser.parse_args()
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_ESP,UC_X86_REG_EIP,UC_X86_REG_EAX,UC_X86_REG_EFLAGS,UC_X86_REG_FPCW
b=args.exe.read_bytes()
if hashlib.sha256(b).hexdigest()!='c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05':
 raise ValueError('wrong retail executable SHA256')
pe=struct.unpack_from('<I',b,60)[0];n=struct.unpack_from('<H',b,pe+6)[0];op=struct.unpack_from('<H',b,pe+20)[0];base=struct.unpack_from('<I',b,pe+52)[0];sz=struct.unpack_from('<I',b,pe+80)[0]
image=[]
for i in range(n):
 _,rva,size,off=struct.unpack_from('<IIII',b,pe+24+op+i*40+8)
 if size:image.append((base+rva,b[off:off+size]))
cases=[(t,dx,dy,speed,frame,count,seed,status) for t in (1,2,3) for dx,dy in ((40,0),(-40,0),(0,40),(0,-40),(30,40),(-30,-40),(1,1),(79,0)) for speed in (0.0,0.4,1.0) for frame,count in ((0,10),(8,21)) for seed in (1,0x12345678,3) for status in (0,7)]
proc=subprocess.run([str(args.native.resolve())],input=''.join(' '.join(map(str,x))+'\n' for x in cases),text=True,capture_output=True,check=True)
rows=proc.stdout.splitlines();passed=0;bad=[]
if len(rows)!=len(cases):raise RuntimeError("native probe returned an incomplete case set")
for case,row in zip(cases,rows):
 t,dx,dy,speed,frame,count,seed,status=case
 u=Uc(UC_ARCH_X86,UC_MODE_32);u.mem_map(base,(sz+4095)&~4095)
 for addr,part in image:u.mem_write(addr,part)
 u.mem_map(0x700000,0x10000);u.mem_map(0x900000,0x1000)
 def put(addr,value):u.mem_write(addr,struct.pack('<I',value&0xffffffff))
 def get(addr):return struct.unpack('<I',u.mem_read(addr,4))[0]
 def fput(addr,value):u.mem_write(addr,struct.pack('<f',value))
 for start in (0x50af14,0x50af78):
  for i in range(20):put(start+i*4,-1)
 put(0x510718,-1);put(0x510760,2);put(0x447558,seed)
 put(0x50b398,4)
 for i,(x,y) in enumerate(((0,0),(2000,0),(2000,2000),(0,2000))):put(0x5104e8+i*8,x);put(0x5104ec+i*8,y)
 h=0x50b3a8;a=h+100
 put(h+0x30,0);put(h+12,500);put(h+16,400);put(h+60,100);put(h+64,100)
 put(a,1);put(a+4,frame);put(a+12,551+dx);put(a+16,433+dy)
 fput(a+32,551+dx);fput(a+36,433+dy);put(a+44,count)
 put(a+48,t);put(a+60,102);put(a+64,95);fput(a+76,speed)
 trace=[]
 def hook(uc,addr,size,user):
  if addr not in (0x402cf0,0x402c60):return
  esp=uc.reg_read(UC_X86_REG_ESP);ret=get(esp)
  if addr==0x402cf0:trace.append([get(esp+4),get(esp+8),get(esp+12)]);pop=16
  else:pop=8
  playing=int(bool(status & (1 << ((get(esp+4)-817)%3)))) if addr==0x402c60 else 0
  uc.reg_write(UC_X86_REG_EAX,playing);uc.reg_write(UC_X86_REG_ESP,esp+pop);uc.reg_write(UC_X86_REG_EIP,ret)
 u.hook_add(UC_HOOK_CODE,hook)
 put(0x708000,0x900000);put(0x708004,1)
 u.reg_write(UC_X86_REG_ESP,0x708000);u.reg_write(UC_X86_REG_EFLAGS,2);u.reg_write(UC_X86_REG_FPCW,0x37f)
 u.emu_start(0x416a30,0x900000,count=100000)
 assert u.reg_read(UC_X86_REG_EIP)==0x900000, (case, hex(u.reg_read(UC_X86_REG_EIP)))
 def si(off):return struct.unpack('<i',u.mem_read(a+off,4))[0]
 def fl(off):return struct.unpack('<f',u.mem_read(a+off,4))[0]
 actual=[si(12),si(16),fl(32),fl(36),si(8),fl(76),si(4),si(44),si(72),get(0x447558),trace[0][0] if trace else -1]
 expected=list(map(float,row.split()))
 assert len(expected)==len(actual)
 equal=all(abs(x-y)<0.0001 if i in (2,3,5) else x==y
           for i,(x,y) in enumerate(zip(actual,expected)))
 equal=equal and len(trace)<=1 and all(sound[1:]==[10,0] for sound in trace)
 if equal:passed+=1
 else:bad.append({'case':case,'original':actual,'native':expected,'sound':trace})
print('Original x86 vs native near-animal cases:',passed,'passed;',len(bad),'failed')
for x in bad[:6]:print(x)

raise SystemExit(1 if bad else 0)

#!/usr/bin/env python3
"""Execute SHA-verified retail x86 bodies with explicit Win32/COM dependencies."""
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
 rng=random.Random(0x404750);rows=[];counts={}
 def row(kind,fields):rows.append(kind+' '+' '.join(str(x) if isinstance(x,str) else f'{x&0xffffffff:x}' for x in fields));counts[kind]=counts.get(kind,0)+1
 def hx(bytes):return bytes.hex() or '-'
 # Classification globals, exact scalar table layout and ordinary C locale.
 classes=[0]+[(4 if 48<=c<=57 else 0)|(8 if c in [9,10,13,32] else 0) for c in range(256)]
 emu.mem_write(DATA+0x7000,struct.pack('<257H',*classes));put(0x449b1c,DATA+0x7002);put(0x449b10,1);put(0x51c680,0);emu.mem_write(0x449b14,b'.')
 texts=[b'',b' -2147483648',b'+2147483648',b'9999999999999999999999',b' \t+0123abc']
 texts += [rng.choice([b'',b' ',b'\t',b'\n'])+rng.choice([b'',b'+',b'-'])+bytes(rng.choice(b'0123456789') for _ in range(rng.randrange(30)))+b'X' for _ in range(995)]
 for text in texts:
  emu.mem_write(DATA,text+b'\0');row('ATOI',[hx(text),call(0x4386bf,[DATA])])
 for value in [*range(256),0xffffffff,0x8141,0x80000000]:row('UPPER',[value,call(0x439212,[value])])
 for i in range(500):
  text=rng.choice([b'e+001',b'E-002',b'123e+001',b'-2E-002',b'0',b'INF',b'+5'])
  decimal=rng.choice([ord('.'),ord(',')]);emu.mem_write(DATA,text+b'\0'+b'\0'*16);emu.mem_write(0x449b14,bytes([decimal]));call(0x4331f4,[DATA]);out=bytes(emu.mem_read(DATA,32)).split(b'\0',1)[0];row('DECIMAL',[hx(text),decimal,hx(out)])
 for i in range(500):
  cursor=DATA+rng.randrange(0x100,0xf000);value=rng.randrange(65536);emu.mem_write(cursor,struct.pack('<H',value));put(DATA,cursor);ret=call(0x43316d,[DATA]);row('SHORT',[cursor,value,ret,get(DATA)])
 open_trace=[];open_result=0
 def open_file(args):open_trace[:]=args[1:];return open_result
 addr=bind('descriptor_open',-4,open_file);hooks[0x437bfa]=hooks.pop(addr)
 for i in range(600):
  mode=rng.choice(['r','w','a','x'])+''.join(rng.choice('+btc nRSTDz') for _ in range(rng.randrange(10)))
  defaults=rng.choice([0,0x4000]);open_result=rng.choice([-1,0,3]);share=rng.choice([0x10,0x20,0x40]);emu.mem_write(DATA+64,mode.encode()+b'\0');array(DATA+128,[11,12,13,14,15,16,17,18]);put(0x51c668,defaults);put(0x51c568,9);open_trace.clear()
  ret=call(0x433e95,[DATA,DATA+64,share,DATA+128]);row('OPEN',[hx(mode.encode()),defaults,open_result,share,int(ret!=0),get(0x51c568),*[get(DATA+128+j*4) for j in range(8)],len(open_trace),*open_trace])
 # Case table's GetCPInfo failure branch is real original ASCII fallback.
 api('GetCPInfo',2,lambda x:0);api('GetOEMCP',0,lambda x:932);api('GetACP',0,lambda x:1252)
 for request in [0,932,936,949,950,1361,1252,99999,-2,-3,-4]*5:
  put(0x51c6b8,77);put(0x51c690,1252);put(0x51c6cc,2);put(0x51c8e4,55);array(0x51c6c0,[1,2,3]);put(0x51c62c,0);emu.mem_write(0x51c7e0,b'\x02'*257);emu.mem_write(0x51c6e0,b'\x11'*256)
  ret=call(0x436cd3,[request]);row('CODEPAGE',[request,ret,get(0x51c62c),get(0x51c6b8),get(0x51c6cc),get(0x51c8e4),get(0x51c6c0),get(0x51c6c4),get(0x51c6c8),*bytes(emu.mem_read(0x51c7e0,257)),*bytes(emu.mem_read(0x51c6e0,256))])
 # MMIO service protocol, identical numbered requests to native test provider.
 trace=[];step=0;failure=0;phase='wave';format_data=b'';format_pos=0;position=0;quantum=3;empty=False
 def tick(id):
  nonlocal step
  trace.append(id);step+=1;return step==failure
 def mmopen(x):return 0 if tick(1) else 1
 def mmclose(x):tick(2);return 0
 def descend(x):
  if tick(3):return 1
  if x[2]==0:array(x[1],[0x46464952,48,0x45564157,12,0])
  elif get(x[1])==0x20746d66:put(x[1]+4,len(format_data))
  elif get(x[1])==0x61746164:put(x[1]+4,8)
  return 0
 def read(x):
  nonlocal format_pos
  if tick(5):return -1
  if format_pos+x[2]>len(format_data):return -1
  if x[2]:emu.mem_write(x[1],format_data[format_pos:format_pos+x[2]])
  format_pos+=x[2];return x[2]
 def getinfo(x):
  if tick(9):return 1
  emu.mem_write(x[1],b'\0'*72);put(x[1]+28,DATA+0x4000+position);put(x[1]+32,DATA+0x4000+min(8,position+quantum));return 0
 def setinfo(x):
  nonlocal position
  if tick(10):return 1
  if get(x[1]+28):position=get(x[1]+28)-DATA-0x4000
  return 0
 def advance(x):
  if tick(11):return 1
  current=get(x[1]+28)-DATA-0x4000;put(x[1]+32,DATA+0x4000+(current if empty else min(8,current+quantum)));return 0
 def seek(x):
  nonlocal position
  position=0;return -1 if tick(7) else 0
 for name,n,fn in [('mmioOpenA',3,mmopen),('mmioClose',2,mmclose),('mmioDescend',4,descend),('mmioAscend',3,lambda x:1 if tick(4) else 0),('mmioRead',3,read),('mmioWrite',3,lambda x:-1 if tick(6) else x[2]),('mmioSeek',3,seek),('mmioCreateChunk',3,lambda x:1 if tick(8) else 0),('mmioGetInfo',3,getinfo),('mmioSetInfo',3,setinfo),('mmioAdvance',3,advance)]:api(name,n,fn)
 api('FindResourceA',3,lambda x:0 if tick(14) else 2)
 api('LoadResource',2,lambda x:0 if tick(15) else 3)
 api('SizeofResource',2,lambda x:0 if tick(16) else 18)
 api('LockResource',1,lambda x:0 if tick(17) else DATA+0x3000)
 def allocate(x):return 0 if tick(3 if phase=='printer' else 12) else DATA+0xa000
 addr=bind('operator_new',-1,allocate);hooks[0x42fbce]=hooks.pop(addr)
 def free(x):tick(4 if phase=='printer' else 13);return 0
 addr=bind('delete',-1,free);hooks[0x42fbdc]=hooks.pop(addr)
 obj=DATA+0x1000
 def wave_state():return [int(get(obj)!=0),get(obj+4),get(obj+0x7c),get(obj+8),get(obj+12),get(obj+28),get(obj+32),get(obj+36),get(obj+40),get(obj+48)]
 for mode in range(5):
  for i in range(120):
   trace.clear();step=0;failure=rng.randrange(0,12);format_pos=0;position=0;quantum=rng.randrange(1,5);empty=False
   tag=rng.choice([1,3]);extra=rng.choice([0,2]);format_data=struct.pack('<HHIIHHH',tag,1,22050,22050,1,8,extra)+(bytes([0x55])*extra if tag!=1 else b'')
   emu.mem_write(obj,b'\0'*144);put(obj+4,1);put(obj+0x7c,1 if mode in [0,1,4] else 2);emu.mem_write(DATA+0x4000,b'ABCDEFGH');emu.mem_write(DATA+0x3000,format_data);emu.mem_write(DATA+0x5000,b'\0'*64)
   requested=rng.randrange(12);memory=rng.randrange(2);offset=rng.randrange(9);actual_initial=99;put(DATA+0x5100,actual_initial)
   if mode==1:
    put(obj+12,8);put(obj+0x80,memory);put(obj+0x84,DATA+0x4000);put(obj+0x88,DATA+0x4000+offset);put(obj+0x8c,8)
    result=call(0x4049b0,[DATA+0x5000,requested,DATA+0x5100],obj)
   elif mode==0:result=call(0x404750,(),obj)
   elif mode==2:result=call(0x404c60,[DATA+0x3000],obj)
   elif mode==3:result=call(0x404b30,(),obj)
   else:result=call(0x404560,[DATA+0x6000,DATA+0x3000,1],obj)
   fmt=bytes(emu.mem_read(get(obj),18+(extra if tag!=1 else 0))) if get(obj) else b''
   row('WAVE',[mode,failure,tag,extra,quantum,requested,memory,offset,result,*wave_state(),get(DATA+0x5100),hx(bytes(emu.mem_read(DATA+0x5000,12))),hx(fmt),len(trace),*trace])
 # Primary buffer format: real original body, controlled COM calls.
 sound_trace=[];sound_failure=0;primary_format=b''
 def primary(x):sound_trace.append(1);put(x[2],DATA+0x8100);return 0x80004005 if sound_failure==1 else 0
 def setformat(x):
  nonlocal primary_format
  sound_trace.append(2);primary_format=bytes(emu.mem_read(x[1],18));return 0x80004005 if sound_failure==2 else 0
 def release(x):sound_trace.append(3);return 0
 manager=DATA+0x8000;buffer=DATA+0x8100;vtable=DATA+0x8200;put(manager,DATA+0x8080);put(DATA+0x8080,vtable);put(buffer,vtable);put(vtable+12,bind('CreateSoundBuffer',4,primary));put(vtable+56,bind('SetFormat',2,setformat));put(vtable+8,bind('Release',1,release))
 for i in range(300):
  sound_trace.clear();primary_format=b'';sound_failure=rng.choice([0,1,2]);channels=rng.choice([1,2,3,65537]);freq=rng.choice([8000,22050,44100,48000]);bits=rng.choice([8,16,24,32]);present=rng.randrange(2);result=call(0x403c40,[channels,freq,bits],manager if present else DATA+0x8300)
  row('PRIMARY',[present,sound_failure,channels,freq,bits,result,hx(primary_format),len(sound_trace),*sound_trace])
 # Complete sound-fill body calls real original Restore/Reset/Read code.
 status_bits=0;restores=0;fill_size=0
 def status(x):sound_trace.append(4);put(x[1],status_bits);return 0x80004005 if sound_failure==4 else 0
 def restore(x):
  nonlocal restores
  sound_trace.append(5)
  if restores:restores-=1;return 0x88780096
  return 0
 def sleep(x):sound_trace.append(6);return 0
 def lock(x):sound_trace.append(7);put(x[3],DATA+0x6000);put(x[4],x[2]);return 0x80004005 if sound_failure==7 else 0
 def unlock(x):sound_trace.append(8);return 0x80004005
 put(vtable+36,bind('GetStatus',2,status));put(vtable+80,bind('Restore',1,restore));put(vtable+44,bind('Lock',8,lock));put(vtable+76,bind('Unlock',5,unlock));api('Sleep',1,sleep)
 for i in range(300):
  sound_trace.clear();sound_failure=rng.choice([0,4,7]);status_bits=rng.choice([0,2]);initial_restores=rng.randrange(4);restores=initial_restores;fill_size=rng.randrange(33);data_size=rng.randrange(9);repeat=rng.randrange(2);bits=rng.choice([8,16])
  emu.mem_write(obj,b'\0'*144);put(obj,DATA+0x3000);put(obj+0x80,1);put(obj+0x84,DATA+0x4000);put(obj+0x88,DATA+0x4001);put(obj+0x8c,data_size);emu.mem_write(DATA+0x4000,bytes(range(1,9)));emu.mem_write(DATA+0x3000,struct.pack('<HHIIHHH',1,1,8000,8000,1,bits,0));emu.mem_write(DATA+0x6000,b'\0'*64)
  sound=DATA+0x8500;array(sound,[0,0,fill_size,obj,1]);result=call(0x404100,[buffer,repeat],sound)
  row('FILL',[fill_size,data_size,bits,repeat,sound_failure,status_bits,initial_restores,result,get(obj+0x88)-get(obj+0x84),hx(bytes(emu.mem_read(DATA+0x6000,32))),len(sound_trace),*sound_trace])
 # Bitmap printing: execute complete successful original GDI control flow.
 phase='printer';failure=0;printer=DATA+0x1000;printer_vtable=DATA+0x2000
 width=8;height=4;bits=24;colors=0;section=False;doc_result=1;page_result=1
 def bitmap_object(x):
  tag=1 if x[1]==24 else 2;tick(tag)
  bm=struct.pack('<4I2HI',0,width,height,((width*bits+31)//32)*4,1,bits,DATA+0x4000 if section or x[1]==84 else 0)
  emu.mem_write(x[2],bm)
  if x[1]==84:array(x[2]+24,[40,width,height,1|(bits<<16),0,0,0,0,colors,0]);emu.mem_write(x[2]+64,b'\0'*20)
  return x[1]
 def dibits(x):
  tick(9);emu.mem_write(x[4],b'\x3c'*(height*((width*24+31)//32)*4));return height
 def color_table(x):tick(12);emu.mem_write(x[3],b'\x77'*(x[2]*4));return x[2]
 def compute_target(x):tick(19);array(printer+0x110,[0,0,640,480]);return 0
 api('GetObjectA',3,bitmap_object);api('GetDC',1,lambda x:(tick(5),1)[1]);api('ReleaseDC',2,lambda x:(tick(6),1)[1]);api('SelectPalette',3,lambda x:(tick(7),2)[1]);api('RealizePalette',1,lambda x:(tick(8),1)[1]);api('GetDIBits',7,dibits)
 api('CreateCompatibleDC',1,lambda x:(tick(10),1)[1]);api('SelectObject',2,lambda x:(tick(11),2)[1]);api('GetDIBColorTable',4,color_table);api('DeleteDC',1,lambda x:(tick(13),1)[1]);api('IsRectEmpty',1,lambda x:0)
 api('GetDeviceCaps',2,lambda x:(tick(14),0)[1]);api('Escape',5,lambda x:(tick(15),1)[1]);api('GetVersionExA',1,lambda x:(tick(16),1)[1]);api('SetStretchBltMode',2,lambda x:(tick(17),1)[1]);api('StretchDIBits',13,lambda x:(tick(18),1)[1])
 api('StartDocA',2,lambda x:(tick(20),doc_result)[1]);api('StartPage',1,lambda x:(tick(21),page_result)[1]);api('EndPage',1,lambda x:(tick(22),1)[1]);api('EndDoc',1,lambda x:(tick(23),1)[1])
 for slot,address in [(0,0x404f90),(8,0x4057e0),(44,0x405c40),(48,0x405ee0)]:put(printer_vtable+slot,address)
 put(printer_vtable+36,bind('ComputeTargetRect',1,compute_target))
 for mode in range(6):
  for i in range(40):
   trace.clear();step=0;width=rng.randrange(2,33);height=rng.randrange(1,5);bits=rng.choice([1,4,8,16,24,32]);colors=0;section=bool(rng.randrange(2));doc_result=rng.randrange(2);page_result=rng.randrange(2);palette=rng.randrange(2)
   emu.mem_write(printer,b'\0'*292);put(printer,printer_vtable);array(printer+0x110,[0,0,640,480]);put(printer+0x120,3);emu.mem_write(DATA+0x4000,b'\x5a'*4096);emu.mem_write(DATA+0xa000,b'\0'*8192)
   if mode>=3:
    put(printer+4,DATA+0xa000);put(printer+8,DATA+0xa000+40);array(DATA+0xa000,[40,width,height,1|(24<<16),0,0,0,0,0,0])
   if mode==0:call(0x405c40,[1,palette],printer)
   elif mode==1:call(0x405ee0,[1],printer)
   elif mode==2:call(0x404e30,[1,palette],printer)
   elif mode==3:call(0x404f90,[1],printer)
   elif mode==4:call(0x405140,[1],printer)
   else:call(0x4057e0,[1],printer)
   ptr=get(printer+4);offset=get(printer+8)-ptr
   size=40+height*((width*24+31)//32)*4 if mode==0 or mode==2 and not section else (offset+height*((width*bits+31)//32)*4 if mode<3 else 40)
   row('PRINTER',[mode,width,height,bits,colors,int(section),doc_result,page_result,palette,offset,hx(bytes(emu.mem_read(ptr,size))),len(trace),*trace])
 text='\n'.join(rows)+'\n';a.out.write_text(text);a.out.with_suffix('.json').write_text(json.dumps({'source_executable_sha256':SHA,'emulator':'Unicorn '+unicorn.__version__,'vector_counts':counts,'vectors_sha256':hashlib.sha256(text.encode()).hexdigest(),'scope':'Original scalar/mode/codepage/MMIO/primary-COM control flow with explicit controlled dependencies; not physical audio/printing or full retail certification.'},indent=2)+'\n');print(counts)
if __name__=='__main__':main()

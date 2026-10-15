#include "btb/original_sound2.hpp"
#include <algorithm>
#include <bit>
#include <cstring>
#include <string>
#include <stdexcept>
namespace btb::original_sound2 {
namespace {bool failed(U32 x){return std::bit_cast<I32>(x)<0;}}
Sound& construct_sound(Sound& s,std::span<const Handle> buffers,U32 size,Wave& w,MmioServices& mmio,SoundServices& audio,CreationServices& io){
 s.vtable=0x43b2e4;s.buffers=io.allocate_buffers(static_cast<U32>(buffers.size()));
 if(!s.buffers)throw std::bad_alloc();
 std::copy(buffers.begin(),buffers.end(),s.buffers);s.size=size;s.count=static_cast<U32>(buffers.size());s.wave=&w;
 if(buffers.empty())throw std::invalid_argument("original sound constructor needs a first buffer");
 fill_sound(s.buffers[0],size,w,false,mmio,audio); // Result deliberately ignored.
 for(U32 i=0;i<s.count;++i)io.set_position(s.buffers[i],0);
 return s;
}
U32 create_sound(Handle manager,Sound** out,const char* path,U32,const std::array<U32,4>& algorithm,U32 count,I32 cd_drive,MmioServices& mmio,SoundServices& audio,CreationServices& io){
 if(!manager)return 0x800401f0;if(!path || !out || !count)return 0x80070057;
 auto* buffers=io.allocate_buffers(count);if(!buffers)return 0x8007000e;
 auto* wave=io.allocate_wave();if(!wave){io.free_buffers(buffers);return 0x8007000e;}
 // Same four field writes as recovered 00404510, preserving remaining allocator bytes.
 wave->format=nullptr;wave->handle=0;wave->size=0;wave->from_memory=false;
 open_wave(*wave,path,{},1,mmio); // Original ignores HRESULT and checks size.
 U32 result=0;
 const auto cleanup=[&]{close_wave(*wave,mmio);if(!wave->from_memory && wave->format){mmio.release(wave->format);wave->format=nullptr;}io.free_wave(wave);io.free_buffers(buffers);};
 if(!wave->size && cd_drive!=-1){
  std::string fallback="e:\\";fallback[0]=static_cast<char>(static_cast<U32>(cd_drive)+static_cast<U32>('a'));fallback+=path;
  if(fallback.size()>=100)throw std::out_of_range("original 100-byte CD sound path");
  open_wave(*wave,fallback.c_str(),{},1,mmio);if(!wave->size){cleanup();return 0x80004005;}
 }
 BufferDescription desc;desc.bytes=wave->size;desc.format=wave->format;desc.algorithm=algorithm;
 U32 code=io.create_buffer(manager,desc,buffers[0]);
 if(code==0x0878000a)result=code;else if(failed(code)){cleanup();return code;}
 for(U32 i=1;i<count;++i){code=io.duplicate_buffer(manager,buffers[0],buffers[i]);if(failed(code)){cleanup();return code;}}
 auto* sound=io.allocate_sound();if(sound)construct_sound(*sound,{buffers,count},wave->size,*wave,mmio,audio,io);
 *out=sound;io.free_buffers(buffers);return result; // Null Sound is still reported as success in retail.
}
}

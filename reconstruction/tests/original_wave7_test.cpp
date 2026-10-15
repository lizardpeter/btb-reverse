#include "batch20_services.hpp"
#include "batch20_vectors.hpp"
#include <iostream>
using namespace btb::original_wave7;
using namespace batch20;
int main(int argc,char** argv){
 if(argc!=2 || !verify_7_original_wave_functions())return 1;std::ifstream file(argv[1]);if(!file)return 2;std::string line,kind;unsigned checked{};
 while(std::getline(file,line)) {
  std::istringstream in(line);in>>kind;bool ok=true;
  if(kind=="WAVE") {
   const U32 mode=word(in);Mmio io;io.fail_at=word(in);const U32 tag=word(in),extra=word(in);io.quantum=word(in);const U32 request=word(in),memory=word(in),offset=word(in),expected=word(in);
   io.format.assign(18+(tag!=1?extra:0),std::byte{0x55});const std::array<U32,4> prefix{tag|(1U<<16),22050,22050,1U|(8U<<16)};std::memcpy(io.format.data(),prefix.data(),16);io.format[16]=std::byte{static_cast<std::uint8_t>(extra)};io.format[17]=std::byte{0};
   std::array<U32,10> after{};for(auto& v:after)v=word(in);const U32 expected_actual=word(in);auto expected_bytes=bytes(in),expected_format=bytes(in);auto expected_trace=trace(in);
   Wave w;w.handle=1;w.flags=mode==0 || mode==1 || mode==4?1:2;U32 actual=99,result=0;std::array<std::byte,64> out{};
   if(mode==0)result=read_format(w,io);
   else if(mode==1){w.chunk.cksize=8;w.from_memory=memory!=0;w.data=reinterpret_cast<const std::byte*>(io.bytes.data());w.current=w.data+offset;w.data_size=8;result=read_wave(w,out.data(),request,&actual,io);}
   else if(mode==2)result=write_format(w,io.format,io);
   else if(mode==3)result=close_wave(w,io);
   else result=open_wave(w,"file",io.format,1,io);
   const std::array<U32,10> state{static_cast<U32>(w.format!=nullptr),static_cast<U32>(w.handle),w.flags,w.chunk.ckid,w.chunk.cksize,w.riff.ckid,w.riff.cksize,w.riff.fccType,w.riff.dwDataOffset,w.size};
   std::string fmt;if(w.format)fmt.assign(reinterpret_cast<char*>(w.format),18+(tag!=1?extra:0));
   ok=result==expected && state==after && actual==expected_actual && std::string(reinterpret_cast<char*>(out.data()),12)==expected_bytes && fmt==expected_format && io.trace==expected_trace;
  } else if(kind=="PRIMARY") {
   const U32 present=word(in);Sound io;io.failure=word(in);const U32 channels=word(in),freq=word(in),bits=word(in),expected=word(in);auto fmt=bytes(in);auto expected_trace=trace(in);
   const auto result=set_primary_format(present,channels,freq,bits,io);
   ok=result==expected && io.trace==expected_trace && (fmt.empty() || std::string(reinterpret_cast<char*>(io.format.data()),18)==fmt);
  } else if(kind=="FILL") {
   const U32 size=word(in),data_size=word(in),bits=word(in),repeat=word(in);Sound sound;sound.failure=word(in);sound.status_bits=word(in);sound.restore_failures=word(in);const U32 expected=word(in),offset=word(in);const auto data=bytes(in);const auto expected_trace=trace(in);
   Mmio mmio;Wave wave;std::array<std::byte,18> format{};format[14]=std::byte{static_cast<std::uint8_t>(bits)};std::array<std::byte,8> input{};for(unsigned i=0;i<8;++i)input[i]=std::byte{static_cast<std::uint8_t>(i+1)};
   wave.format=format.data();wave.from_memory=true;wave.data=input.data();wave.current=wave.data+1;wave.data_size=data_size;
   const auto result=fill_sound(1,size,wave,repeat!=0,mmio,sound);
   ok=result==expected && static_cast<U32>(wave.current-wave.data)==offset && std::string(reinterpret_cast<char*>(sound.bytes.data()),32)==data && sound.trace==expected_trace;
  } else continue;
  if(!in || !ok){std::cerr<<"Wave reference mismatch "<<checked<<": "<<line<<'\n';return 3;}++checked;
 }
 assert(checked==1200);
 // Fill pipeline calls the actual recovered restore/reset/read procedures.
 for(unsigned repeat=0;repeat<2;++repeat)for(unsigned bits:{8U,16U}) {
  Mmio mmio;Sound sound;Wave wave;std::array<std::byte,18> format{};format[14]=std::byte{static_cast<std::uint8_t>(bits)};
  std::array<std::byte,3> samples{std::byte{1},std::byte{2},std::byte{3}};wave.format=format.data();wave.from_memory=true;wave.data=samples.data();wave.current=samples.data()+1;wave.data_size=3;
  assert(fill_sound(1,8,wave,repeat!=0,mmio,sound)==0);assert(sound.bytes[0]==std::byte{1});
  assert(sound.bytes[3]==(repeat?std::byte{1}:bits==8?std::byte{128}:std::byte{0}));assert(sound.trace.back()==8);
 }
 Mmio mmio;Sound sound;Wave wave;std::array<std::byte,18> format{};format[14]=std::byte{8};std::array<std::byte,2> empty{};wave.format=format.data();wave.from_memory=true;wave.data=empty.data();wave.current=empty.data();
 sound.status_bits=2;sound.restore_failures=2;assert(fill_sound(1,4,wave,false,mmio,sound)==0);
 assert(std::count(sound.trace.begin(),sound.trace.end(),6)==1 && sound.bytes[3]==std::byte{128});
 sound.failure=7;sound.trace.clear();assert(fill_sound(1,4,wave,false,mmio,sound)==0x80004005 && sound.trace.back()==7);
 wave.flags=2;wave.handle=0;assert(close_wave(wave,mmio)==0x800401f0 && (wave.output.flags&0x10000000));
 std::cout<<checked<<" original wave/COM comparisons and connected fill tests passed\n";
}

#include "btb/original_wave7.hpp"
#include "btb/original_activity9.hpp"
#include <algorithm>
#include <bit>
#include <cstring>
#include <stdexcept>
namespace btb::original_wave7 {
namespace {
constexpr U32 fail=0x80004005,not_initialized=0x800401f0;
bool failed(U32 result){return std::bit_cast<I32>(result)<0;}
std::uint16_t word(const std::byte* p){return std::to_integer<std::uint16_t>(p[0])|(std::to_integer<std::uint16_t>(p[1])<<8);}
struct Context {Wave& wave;MmioServices& io;dsutil::RetailCWaveFile32& retail;};
bool seek_reset(void* ptr,I32 offset) noexcept{auto& c=*static_cast<Context*>(ptr);return c.io.seek(c.wave.handle,offset,0)!=-1;}
bool descend_reset(void* ptr) noexcept{auto& c=*static_cast<Context*>(ptr);c.wave.chunk=c.retail.m_ck;const bool okay=c.io.descend(c.wave.handle,c.wave.chunk,&c.wave.riff,0x10)==0;c.retail.m_ck=c.wave.chunk;return okay;}
bool create_reset(void* ptr) noexcept{auto& c=*static_cast<Context*>(ptr);c.wave.chunk=c.retail.m_ck;const bool okay=c.io.create_chunk(c.wave.handle,c.wave.chunk,0)==0;c.retail.m_ck=c.wave.chunk;return okay;}
bool info_reset(void* ptr) noexcept{auto& c=*static_cast<Context*>(ptr);return c.io.get_info(c.wave.handle,c.wave.output)==0;}
}
U32 reset_wave(Wave& w,MmioServices& io) noexcept {
    dsutil::RetailCWaveFile32 retail;retail.m_bIsReadingFromMemory=w.from_memory?1:0;
    retail.m_hmmio=w.handle?1:0;retail.m_dwFlags=w.flags;retail.m_ck=w.chunk;retail.m_ckRiff=w.riff;
    Context ctx{w,io,retail};
    const U32 result=original_activity9::reset_wave_file(retail,&ctx,seek_reset,descend_reset,create_reset,info_reset);
    w.chunk=retail.m_ck;
    if(w.from_memory)w.current=w.data;
    return result;
}
U32 read_format(Wave& w,MmioServices& io) noexcept {
    w.format=nullptr;
    if(io.descend(w.handle,w.riff,nullptr,0) || w.riff.ckid!=0x46464952 || w.riff.fccType!=0x45564157)return fail;
    Chunk fmt;fmt.ckid=0x20746d66;
    if(io.descend(w.handle,fmt,&w.riff,0x10) || fmt.cksize<16)return fail;
    std::array<std::byte,16> header{};
    if(io.read(w.handle,header.data(),16)!=16)return fail;
    std::uint16_t extra=0;
    const bool extended=word(header.data())!=1;
    if(extended) {
        std::byte bytes[2]{};if(io.read(w.handle,bytes,2)!=2)return fail;extra=word(bytes);
    }
    w.format=io.allocate(18U+extra);if(!w.format)return fail;
    std::memcpy(w.format,header.data(),16);w.format[16]=std::byte{static_cast<std::uint8_t>(extra)};w.format[17]=std::byte{static_cast<std::uint8_t>(extra>>8)};
    if((extended && io.read(w.handle,w.format+18,extra)!=extra) || io.ascend(w.handle,fmt)) {
        io.release(w.format);w.format=nullptr;return fail;
    }
    return 0;
}
U32 read_wave(Wave& w,std::byte* out,U32 requested,U32* actual,MmioServices& io) noexcept {
    if(w.from_memory) {
        if(!w.current)return not_initialized;
        if(actual)*actual=0;
        const auto remaining=static_cast<U32>((w.data+w.data_size)-w.current);
        requested=std::min(requested,remaining);
        if(requested)std::memcpy(out,w.current,requested);
        // Retail does NOT advance m_pbDataCur in this branch.
        if(actual)*actual=requested;
        return 0;
    }
    if(!w.handle)return not_initialized;
    if(!out || !actual)return 0x80070057;
    *actual=0;Info info;
    if(io.get_info(w.handle,info))return fail;
    requested=std::min(requested,w.chunk.cksize);w.chunk.cksize-=requested;
    for(U32 i=0;i<requested;++i) {
        if(info.next==info.end_read && (io.advance(w.handle,info) || info.next==info.end_read))return fail;
        out[i]=std::byte{static_cast<std::uint8_t>(*info.next++)};
    }
    if(io.set_info(w.handle,info))return fail;
    *actual=requested;return 0;
}
U32 close_wave(Wave& w,MmioServices& io) noexcept {
    if(w.flags!=1) {
        w.output.flags|=0x10000000;
        if(!w.handle)return not_initialized;
        if(io.set_info(w.handle,w.output) || io.ascend(w.handle,w.chunk) || io.ascend(w.handle,w.riff))return fail;
        io.seek(w.handle,0,0);
        if(io.descend(w.handle,w.riff,nullptr,0))return fail;
        w.chunk.ckid=0x74636166;
        if(!io.descend(w.handle,w.chunk,&w.riff,0x10)) {
            const std::array<std::byte,4> zero{};
            io.write(w.handle,zero.data(),4);io.ascend(w.handle,w.chunk);
        }
        if(io.ascend(w.handle,w.riff))return fail;
    }
    io.close(w.handle);w.handle=0;return 0;
}
U32 write_format(Wave& w,std::span<const std::byte> fmt,MmioServices& io) {
    if(fmt.size()<16 || (word(fmt.data())!=1 && fmt.size()<18))throw std::out_of_range("short original WAVEFORMATEX");
    w.riff.fccType=0x45564157;w.riff.cksize=0;
    if(io.create_chunk(w.handle,w.riff,0x20))return fail;
    w.chunk.ckid=0x20746d66;w.chunk.cksize=16;
    if(io.create_chunk(w.handle,w.chunk,0))return fail;
    const U32 count=word(fmt.data())==1?16:18U+word(fmt.data()+16);
    if(fmt.size()<count)throw std::out_of_range("short original extended WAVEFORMATEX");
    if(io.write(w.handle,fmt.data(),count)!=static_cast<I32>(count) || io.ascend(w.handle,w.chunk))return fail;
    Chunk fact;fact.ckid=0x74636166;fact.cksize=0;
    if(io.create_chunk(w.handle,fact,0))return fail;
    const std::array<std::byte,4> unknown{std::byte{255},std::byte{255},std::byte{255},std::byte{255}};
    if(io.write(w.handle,unknown.data(),4)!=4)return fail;
    return io.ascend(w.handle,fact)?fail:0;
}
U32 open_wave(Wave& w,const char* path,std::span<const std::byte> format,U32 flags,MmioServices& io) {
    w.flags=flags;w.from_memory=false;
    U32 result;
    if(flags==1) {
        if(!path)return 0x80070057;
        if(w.format){io.release(w.format);w.format=nullptr;}
        w.handle=io.open(path,nullptr,0,0x10000);
        if(!w.handle) {
            Handle resource=io.find_resource(path,"WAVE");if(!resource)resource=io.find_resource(path,"WAV");
            if(!resource)return fail;
            const Handle loaded=io.load_resource(resource);if(!loaded)return fail;
            const U32 size=io.resource_size(resource);if(!size)return fail;
            const auto* source=io.lock_resource(loaded);if(!source)return fail;
            auto* copy=io.allocate(size);if(!copy)throw std::bad_alloc(); // Retail dereferences failed new here.
            std::memcpy(copy,source,size);
            w.handle=io.open(nullptr,copy,size,0x10000); // Original does not retain/free this allocation.
        }
        result=read_format(w,io);
        if(failed(result)){io.close(w.handle);return result;}
        result=reset_wave(w,io);if(!failed(result))w.size=w.chunk.cksize;
    } else {
        w.handle=io.open(path,nullptr,0,0x11002);if(!w.handle)return fail;
        result=write_format(w,format,io);if(failed(result)){io.close(w.handle);return result;}
        result=reset_wave(w,io);
    }
    return result;
}
U32 set_primary_format(Handle manager,U32 channels,U32 frequency,U32 bits,SoundServices& io) {
    if(!manager)return not_initialized;
    const std::array<U32,9> desc{36,0x81,0,0,0,0,0,0,0};Handle buffer{};
    U32 result=io.create_primary(manager,desc,buffer);if(failed(result))return result;
    std::array<std::byte,18> fmt{};
    auto put16=[&](unsigned at,U32 value){fmt[at]=std::byte{static_cast<std::uint8_t>(value)};fmt[at+1]=std::byte{static_cast<std::uint8_t>(value>>8)};};
    auto put32=[&](unsigned at,U32 value){for(unsigned i=0;i<4;++i)fmt[at+i]=std::byte{static_cast<std::uint8_t>(value>>(i*8))};};
    const U32 block=(((bits&65535)>>3)*channels)&65535;
    put16(0,1);put16(2,channels);put32(4,frequency);put32(8,block*frequency);put16(12,block);put16(14,bits);
    result=io.set_format(buffer,fmt);if(failed(result))return result; // Retail leaks primary buffer on SetFormat failure.
    if(buffer)io.release(buffer);return 0;
}
U32 fill_sound(Handle buffer,U32 buffer_size,Wave& w,bool repeat,MmioServices& mmio,SoundServices& io) {
    if(!buffer)return not_initialized;
    struct RestoreContext {Handle buffer;SoundServices& io;} ctx{buffer,io};
    const auto status=[](void* p,U32* value) noexcept{auto& c=*static_cast<RestoreContext*>(p);return c.io.status(c.buffer,value);};
    const auto restore=[](void* p) noexcept{auto& c=*static_cast<RestoreContext*>(p);return c.io.restore(c.buffer);};
    const std::function<void(U32)> sleep=[&io](U32 ms){io.sleep(ms);};
    U32 result=original_activity9::restore_sound_buffer(&ctx,nullptr,status,restore,sleep);
    if(failed(result))return result;
    std::byte* data{};U32 size{};result=io.lock(buffer,buffer_size,data,size);if(failed(result))return result;
    reset_wave(w,mmio);U32 read{};result=read_wave(w,data,size,&read,mmio);if(failed(result))return result;
    if(read==0 || (!repeat && read<size)) {
        if(!w.format)throw std::logic_error("missing original wave format");
        const auto silence=word(w.format+14)==8?std::byte{128}:std::byte{0};
        std::fill(data+read,data+size,silence);
    } else while(read<size) {
        result=reset_wave(w,mmio);if(failed(result))return result;
        U32 more{};result=read_wave(w,data+read,size-read,&more,mmio);if(failed(result))return result;
        if(!more)throw std::runtime_error("original looping sound makes no progress");
        read+=more;
    }
    io.unlock(buffer,data,size);return 0;
}
}

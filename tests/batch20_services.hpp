#pragma once
#include "btb/original_wave7.hpp"
#include "btb/original_printer7.hpp"
#include "btb/original_runtime6.hpp"
#include <algorithm>
#include <cassert>
#include <cstring>
#include <memory>
#include <vector>
namespace batch20 {
using U32=std::uint32_t;using I32=std::int32_t;
struct Mmio final:btb::original_wave7::MmioServices {
    using Handle=btb::original_wave7::Handle;using Chunk=btb::original_wave7::Chunk;using Info=btb::original_wave7::Info;
    std::vector<U32> trace;U32 step{},fail_at{},format_cursor{},position{},quantum{3};bool resource_fallback{},resource_found{true},empty_advance{};
    std::vector<std::byte> format=std::vector<std::byte>(18);std::array<char,8> bytes{'A','B','C','D','E','F','G','H'};
    std::vector<std::unique_ptr<std::byte[]>> allocations;U32 releases{};
    bool tick(U32 id) noexcept{trace.push_back(id);return ++step==fail_at;}
    Handle open(const char* path,const std::byte*,U32,U32 flags) noexcept override{if(tick(1))return 0;if(path && resource_fallback)return 0;assert(flags==0x10000 || flags==0x11002);return 1;}
    U32 close(Handle) noexcept override{tick(2);return 0;}
    U32 descend(Handle,Chunk& ck,const Chunk* parent,U32 flags) noexcept override{
        if(tick(3))return 1;
        if(!parent){assert(flags==0);ck={0x46464952,48,0x45564157,12,0};}
        else {assert(flags==0x10);if(ck.ckid==0x20746d66)ck.cksize=static_cast<U32>(format.size());if(ck.ckid==0x61746164)ck.cksize=8;}
        return 0;
    }
    U32 ascend(Handle,Chunk&) noexcept override{return tick(4)?1:0;}
    I32 read(Handle,std::byte* out,U32 count) noexcept override{
        if(tick(5))return -1;
        if(format_cursor+count>format.size())return -1;
        if(count)std::memcpy(out,format.data()+format_cursor,count);format_cursor+=count;return static_cast<I32>(count);
    }
    I32 write(Handle,const std::byte*,U32 n) noexcept override{return tick(6)?-1:static_cast<I32>(n);}
    I32 seek(Handle,I32,U32) noexcept override{position=0;return tick(7)?-1:0;}
    U32 create_chunk(Handle,Chunk&,U32) noexcept override{return tick(8)?1:0;}
    U32 get_info(Handle,Info& info) noexcept override{if(tick(9))return 1;info.next=bytes.data()+position;info.end_read=bytes.data()+std::min<U32>(8,position+quantum);return 0;}
    U32 set_info(Handle,const Info& info) noexcept override{if(tick(10))return 1;if(info.next)position=static_cast<U32>(info.next-bytes.data());return 0;}
    U32 advance(Handle,Info& info) noexcept override{if(tick(11))return 1;const U32 at=static_cast<U32>(info.next-bytes.data());info.next=bytes.data()+at;info.end_read=bytes.data()+(empty_advance?at:std::min<U32>(8,at+quantum));return 0;}
    std::byte* allocate(U32 n) noexcept override{if(tick(12))return nullptr;allocations.push_back(std::make_unique<std::byte[]>(n));return allocations.back().get();}
    void release(std::byte*) noexcept override{tick(13);++releases;}
    Handle find_resource(const char*,const char*) noexcept override{return tick(14) || !resource_found?0:2;}
    Handle load_resource(Handle) noexcept override{return tick(15)?0:3;}
    U32 resource_size(Handle) noexcept override{return tick(16)?0:18;}
    const std::byte* lock_resource(Handle) noexcept override{return tick(17)?nullptr:format.data();}
};
struct Sound final:btb::original_wave7::SoundServices {
    using Handle=btb::original_wave7::Handle;
    std::array<std::byte,64> bytes{};std::array<std::byte,18> format{};std::vector<U32> trace;U32 failure{},status_bits{},restore_failures{};
    U32 create_primary(Handle,std::span<const U32,9> desc,Handle& out) override{trace.push_back(1);assert(desc[0]==36 && desc[1]==0x81);out=2;return failure==1?0x80004005:0;}
    U32 set_format(Handle,std::span<const std::byte,18> f) override{trace.push_back(2);std::copy(f.begin(),f.end(),format.begin());return failure==2?0x80004005:0;}
    void release(Handle) override{trace.push_back(3);}
    U32 status(Handle,U32* value) noexcept override{trace.push_back(4);*value=status_bits;return failure==4?0x80004005:0;}
    U32 restore(Handle) noexcept override{trace.push_back(5);if(restore_failures){--restore_failures;return 0x88780096;}return 0;}
    void sleep(U32 ms) noexcept override{trace.push_back(6);assert(ms>0);}
    U32 lock(Handle,U32 size,std::byte*& out,U32& count) override{trace.push_back(7);assert(size<=bytes.size());out=bytes.data();count=size;return failure==7?0x80004005:0;}
    U32 unlock(Handle,std::byte*,U32) override{trace.push_back(8);return 0x80004005;}
};
struct PrinterServices final:btb::original_printer7::Services {
    using Handle=btb::original_printer7::Handle;using Printer=btb::original_printer7::Printer;using Bitmap=btb::original_printer7::Bitmap;using DibSection=btb::original_printer7::DibSection;using Rect=btb::original_printer7::Rect;
    std::vector<U32> trace;std::vector<std::unique_ptr<std::byte[]>> allocations;
    std::vector<std::byte> pixels=std::vector<std::byte>(4096,std::byte{0x5a});
    I32 width{8},height{4},bitmap_result{24},section_result{84},dibits_result{4},stretch_result{1},caps{},doc_result{1},page_result{1};
    U32 bits{24},compression{},colors{};bool section{};
    I32 get_bitmap(Handle,Bitmap& out) override{trace.push_back(1);out={width,height,static_cast<std::uint16_t>(bits),section?pixels.data():nullptr};return bitmap_result;}
    I32 get_section(Handle,DibSection& out) override{trace.push_back(2);out.bitmap={width,height,static_cast<std::uint16_t>(bits),pixels.data()};std::array<U32,10> h{40,static_cast<U32>(width),static_cast<U32>(height),1U|(bits<<16),compression,0,0,0,colors,0};std::memcpy(out.header.data(),h.data(),40);return section_result;}
    std::byte* allocate(U32 bytes) override{trace.push_back(3);allocations.push_back(std::make_unique<std::byte[]>(bytes));return allocations.back().get();}
    void release(std::byte*) noexcept override{trace.push_back(4);}
    Handle screen_dc() override{trace.push_back(5);return 1;}
    void release_screen_dc(Handle) override{trace.push_back(6);}
    Handle select_palette(Handle,Handle,bool) override{trace.push_back(7);return 2;}
    void realize_palette(Handle) override{trace.push_back(8);}
    I32 get_dibits(Handle,Handle,U32 lines,std::byte* out,std::byte*) override{trace.push_back(9);const U32 size=lines*((static_cast<U32>(width)*24+31)>>3&~3U);std::memset(out,0x3c,size);return dibits_result;}
    Handle compatible_dc() override{trace.push_back(10);return 1;}
    Handle select_bitmap(Handle,Handle) override{trace.push_back(11);return 2;}
    void get_colors(Handle,U32 n,std::byte* out) override{trace.push_back(12);std::memset(out,0x77,n*4);}
    void delete_dc(Handle) override{trace.push_back(13);}
    I32 raster_caps(Handle) override{trace.push_back(14);return caps;}
    void query_escape(Handle,I32 n) override{trace.push_back(15);assert(n==3);}
    void query_version() override{trace.push_back(16);}
    void stretch_mode(Handle,I32 n) override{trace.push_back(17);assert(n==3);}
    I32 stretch_dib(Handle,const Rect&,I32 w,I32 h,const std::byte*,const std::byte*) override{trace.push_back(18);assert(w==width && h==height);return stretch_result;}
    void compute_target(Printer& p,Handle) override{trace.push_back(19);p.target={0,0,640,480};}
    I32 start_doc(Handle,const char*) override{trace.push_back(20);return doc_result;}
    I32 start_page(Handle) override{trace.push_back(21);return page_result;}
    void end_page(Handle) override{trace.push_back(22);}
    void end_doc(Handle) override{trace.push_back(23);}
};
// NLS backend deliberately unavailable; ordinary locale fast paths and original
// GetCPInfo-failure ASCII table construction need no mapping service.
struct Nls:btb::original_locale7::NlsServices {
    using U16=std::uint16_t;bool info{};
    I32 multibyte_to_wide(U32,U32,const char*,I32,U16*,I32) override{return 0;}
    I32 wide_to_multibyte(U32,U32,const U16*,I32,char*,I32) override{return 0;}
    I32 map_ansi(U32,U32,const char*,I32,char*,I32) override{return 0;}
    I32 map_wide(U32,U32,const U16*,I32,void*,I32) override{return 0;}
    bool type_ansi(U32,U32,const char*,I32,U16*) override{return false;}
    bool type_wide(U32,const U16*,I32,U16*) override{return false;}
    bool codepage_info(U32,btb::original_locale7::CodepageInfo&) override{return false;}
};
inline btb::original_locale7::State locale(){btb::original_locale7::State s;for(unsigned c=0;c<256;++c)s.classes[c+1]=(c>='0'&&c<='9'?4:0)|(c==' '||c=='\t'||c=='\n'||c=='\r'?8:0);return s;}
}

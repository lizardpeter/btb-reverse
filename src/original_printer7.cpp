#include "btb/original_printer7.hpp"
#include <bit>
#include <cstdlib>
#include <cstring>
#include <limits>
namespace btb::original_printer7 {
namespace {
U32 read32(const std::byte* p){U32 v{};for(unsigned i=0;i<4;++i)v|=std::to_integer<U32>(p[i])<<(i*8);return v;}
void put32(std::byte* p,U32 v){for(unsigned i=0;i<4;++i)p[i]=std::byte{static_cast<std::uint8_t>(v>>(i*8))};}
void replace_allocation(Printer& p,U32 bytes,Services& io){io.release(p.dib);p.dib=nullptr;p.bits=nullptr;p.dib=io.allocate(bytes);if(!p.dib)throw std::bad_alloc();p.allocation_size=bytes;}
void require_ready(const Printer& p,Handle dc){if(!p.dib)throw Error("No bitmap defined.");if(!dc)throw Error("Invalid printer DC.");}
}
Error::Error(const char* message):std::runtime_error(message) {
    auto allocate=[](std::size_t n)->void*{return std::malloc(n);};
    original_runtime16::construct_exception(object_.base,"",allocate);
    try {original_string8::assign_buffer(object_.text,message,static_cast<U32>(std::strlen(message)),heap_);}
    catch(...) {original_crt17::exception_destruct(object_.base,0x43b5bc,std::free);throw;}
    object_.base.vtable=0x43b328;
}
Error::Error(const Error& other):std::runtime_error(other){original_exception9::copy_runtime(object_,other.object_,heap_);}
Error::~Error() noexcept {original_exception9::destroy_runtime(object_,heap_);}
void set_bitmap(Printer& p,Handle bitmap,Handle palette,Services& io) {
    Bitmap info;
    if(io.get_bitmap(bitmap,info)!=24)throw Error("Invalid bitmap.");
    if(info.pixels)import_section(p,bitmap,io);else import_ddb(p,bitmap,palette,io);
}
void render_to_dc(Printer& p,Handle dc,Services& io) {
    require_ready(p,dc);io.compute_target(p,dc);stretch_dib(p,dc,io);
}
[[noreturn]] void rethrow_print_error(Handle dc,Services& io){io.end_page(dc);io.end_doc(dc);throw;}
void print_document(Printer& p,Handle dc,Services& io) {
    require_ready(p,dc);
    if(io.start_doc(dc,p.document.data())>0) {
        if(io.start_page(dc)>0) {
            try {render_to_dc(p,dc,io);}
            catch(...) {rethrow_print_error(dc,io);}
            io.end_page(dc);
        }
        io.end_doc(dc);
    }
}
void import_ddb(Printer& p,Handle bitmap,Handle palette,Services& io) {
    Bitmap info;if(io.get_bitmap(bitmap,info)!=24)throw Error("Error getting DDB info.");
    const U32 stride=((static_cast<U32>(info.width)*24U+31U)>>3)&~3U;
    const U32 bytes=static_cast<U32>(info.height)*stride+40U;
    if(bytes<40)throw std::overflow_error("original DDB allocation wraps");
    replace_allocation(p,bytes,io);p.bits=p.dib+40;std::memset(p.dib,0,bytes);
    put32(p.dib,40);put32(p.dib+4,static_cast<U32>(info.width));put32(p.dib+8,static_cast<U32>(info.height));
    p.dib[12]=std::byte{1};p.dib[14]=std::byte{24};
    const Handle dc=io.screen_dc();
    if(palette){palette=io.select_palette(dc,palette,true);io.realize_palette(dc);}
    const I32 result=io.get_dibits(dc,bitmap,static_cast<U32>(info.height),p.bits,p.dib);
    if(palette)io.select_palette(dc,palette,true);
    io.release_screen_dc(dc);
    if(!result)throw Error("Error getting DDB bits.");
}
void import_section(Printer& p,Handle bitmap,Services& io) {
    DibSection info;if(io.get_section(bitmap,info)!=84)throw Error("Error getting DIB section info.");
    U32 colors=read32(info.header.data()+32);
    const U32 compression=read32(info.header.data()+16);
    if(static_cast<std::uint16_t>(colors)==0 && info.bitmap.bits<=8)colors=1U<<(info.bitmap.bits&31);
    else if(compression!=0){import_ddb(p,bitmap,0,io);return;}
    const I32 signed_colors=static_cast<std::int16_t>(colors&65535);
    if(signed_colors<0)throw std::out_of_range("original negative color-table allocation");
    colors=static_cast<U32>(signed_colors);
    const I32 product=std::bit_cast<I32>(static_cast<U32>(info.bitmap.width)*info.bitmap.bits+31U);
    const U32 stride=static_cast<U32>(product>>3)&~3U;
    const U32 bytes=static_cast<U32>(info.bitmap.height)*stride;
    const U32 allocation=bytes+40U+colors*4U;
    if(allocation<bytes)throw std::overflow_error("original DIB section allocation wraps");
    replace_allocation(p,allocation,io);p.bits=p.dib+40+colors*4;
    std::memcpy(p.dib,info.header.data(),40);
    if(colors>0 && compression!=3) {
        const Handle dc=io.compatible_dc(),old=io.select_bitmap(dc,bitmap);
        io.get_colors(dc,colors,p.dib+40);io.select_bitmap(dc,old);io.delete_dc(dc);
    }
    if(bytes)std::memcpy(p.bits,info.bitmap.pixels,bytes);
}
void stretch_dib(Printer& p,Handle dc,Services& io) {
    if(p.target.right<=p.target.left || p.target.bottom<=p.target.top)throw Error("Invalid target rectangle.");
    // Retail rejects RC_DIBTODEV despite the name suggesting support.
    if(io.raster_caps(dc)&0x100)throw Error("Unsupported printer.");
    if(!p.dib || !p.bits)throw Error("Error getting DIB info.");
    const I32 width=std::bit_cast<I32>(read32(p.dib+4)),height=std::bit_cast<I32>(read32(p.dib+8));
    io.query_escape(dc,3);io.query_version();io.stretch_mode(dc,3);
    if(io.stretch_dib(dc,p.target,width,height,p.bits,p.dib)==-1)throw Error("Error printing DIB.");
}
}

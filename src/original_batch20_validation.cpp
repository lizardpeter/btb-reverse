#include "btb/original_wave7.hpp"
#include "btb/original_printer7.hpp"
#include "btb/original_runtime6.hpp"
namespace btb::original_wave7 {
bool verify_7_original_wave_functions(){
    // Valid source fast-path: no Windows services are consulted for memory read.
    struct Mmio final:MmioServices {
        Handle open(const char*,const std::byte*,U32,U32) noexcept override{return 0;}
        U32 close(Handle) noexcept override{return 0;}
        U32 descend(Handle,Chunk&,const Chunk*,U32) noexcept override{return 1;}
        U32 ascend(Handle,Chunk&) noexcept override{return 1;}
        I32 read(Handle,std::byte*,U32) noexcept override{return -1;}
        I32 write(Handle,const std::byte*,U32) noexcept override{return -1;}
        I32 seek(Handle,I32,U32) noexcept override{return -1;}
        U32 create_chunk(Handle,Chunk&,U32) noexcept override{return 1;}
        U32 get_info(Handle,Info&) noexcept override{return 1;}
        U32 set_info(Handle,const Info&) noexcept override{return 1;}
        U32 advance(Handle,Info&) noexcept override{return 1;}
        std::byte* allocate(U32) noexcept override{return nullptr;}
        void release(std::byte*) noexcept override{}
        Handle find_resource(const char*,const char*) noexcept override{return 0;}
        Handle load_resource(Handle) noexcept override{return 0;}
        U32 resource_size(Handle) noexcept override{return 0;}
        const std::byte* lock_resource(Handle) noexcept override{return nullptr;}
    } io;
    std::array<std::byte,3> bytes{std::byte{1},std::byte{2},std::byte{3}},out{};U32 count=0;
    Wave wave;wave.from_memory=true;wave.data=bytes.data();wave.current=bytes.data();wave.data_size=3;
    return read_wave(wave,out.data(),2,&count,io)==0 && count==2 && out[1]==std::byte{2} && wave.current==wave.data;
}
}
namespace btb::original_printer7 {
bool verify_7_original_printer_functions(){
    Error error("Invalid bitmap.");Error copy(error);
    return copy.retail_object().base.vtable==0x43b328 && copy.retail_object().text.length==15;
}
}
namespace btb::original_runtime6 {
bool verify_6_original_runtime_functions(){
    U32 cursor=0x12fffe;const std::array<std::byte,4> bytes{std::byte{0x34},std::byte{0x12},std::byte{0},std::byte{0}};
    return fetch_short_argument(bytes,0x12fffe,cursor)==0x131234 && cursor==0x130002;
}
}

#pragma once
#include "btb/dsutil.hpp"
#include <cstddef>
#include <cstdint>
#include <span>
namespace btb::original_wave7 {
using U32=std::uint32_t;using I32=std::int32_t;using Handle=std::uintptr_t;
using Chunk=dsutil::RetailMMCKINFO32;
struct Info {U32 flags{};char* next{};char* end_read{};std::array<std::byte,72> native{};};
struct Wave {
    std::byte* format{};Handle handle{};Chunk chunk{},riff{};U32 size{},flags{};
    Info output{};bool from_memory{};const std::byte* data{};const std::byte* current{};U32 data_size{};
};
class MmioServices {
public:
    virtual ~MmioServices()=default;
    virtual Handle open(const char*,const std::byte* memory,U32 bytes,U32 flags) noexcept=0;
    virtual U32 close(Handle) noexcept=0;
    virtual U32 descend(Handle,Chunk&,const Chunk*,U32) noexcept=0;
    virtual U32 ascend(Handle,Chunk&) noexcept=0;
    virtual I32 read(Handle,std::byte*,U32) noexcept=0;
    virtual I32 write(Handle,const std::byte*,U32) noexcept=0;
    virtual I32 seek(Handle,I32,U32) noexcept=0;
    virtual U32 create_chunk(Handle,Chunk&,U32) noexcept=0;
    virtual U32 get_info(Handle,Info&) noexcept=0;
    virtual U32 set_info(Handle,const Info&) noexcept=0;
    virtual U32 advance(Handle,Info&) noexcept=0;
    virtual std::byte* allocate(U32) noexcept=0;
    virtual void release(std::byte*) noexcept=0;
    virtual Handle find_resource(const char* name,const char* type) noexcept=0;
    virtual Handle load_resource(Handle) noexcept=0;
    virtual U32 resource_size(Handle) noexcept=0;
    virtual const std::byte* lock_resource(Handle) noexcept=0;
};
// 00404750 / 004049B0 / 00404B30 / 00404C60 / 00404560.
U32 read_format(Wave&,MmioServices&) noexcept;
U32 read_wave(Wave&,std::byte*,U32,U32*,MmioServices&) noexcept;
U32 close_wave(Wave&,MmioServices&) noexcept;
U32 write_format(Wave&,std::span<const std::byte>,MmioServices&);
U32 open_wave(Wave&,const char*,std::span<const std::byte>,U32,MmioServices&);
// Connect existing 004048F0 directly to real chunk/info services.
U32 reset_wave(Wave&,MmioServices&) noexcept;
class SoundServices {
public:
    virtual ~SoundServices()=default;
    virtual U32 create_primary(Handle,std::span<const U32,9>,Handle&)=0;
    virtual U32 set_format(Handle,std::span<const std::byte,18>)=0;
    virtual void release(Handle)=0;
    virtual U32 status(Handle,U32*) noexcept=0;
    virtual U32 restore(Handle) noexcept=0;
    virtual void sleep(U32) noexcept=0;
    virtual U32 lock(Handle,U32,std::byte*&,U32&)=0;
    virtual U32 unlock(Handle,std::byte*,U32)=0;
};
// 00403C40 / 00404100. Restore, reset and read dependencies are recovered bodies.
U32 set_primary_format(Handle,U32 channels,U32 frequency,U32 bits,SoundServices&);
U32 fill_sound(Handle,U32 buffer_size,Wave&,bool repeat,MmioServices&,SoundServices&);
bool verify_7_original_wave_functions();
}

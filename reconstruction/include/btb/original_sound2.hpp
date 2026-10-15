#pragma once
#include "btb/original_wave7.hpp"
namespace btb::original_sound2 {
using namespace original_wave7;
struct Sound {U32 vtable{};Handle* buffers{};U32 size{};Wave* wave{};U32 count{};};
struct BufferDescription {U32 size{36},flags{128},bytes{},reserved{};const std::byte* format{};std::array<U32,4> algorithm{};};
class CreationServices {
public:
 virtual ~CreationServices()=default;
 // Typed host storage corresponding to new(count*4), new(0x90), new(0x14).
 virtual Handle* allocate_buffers(U32 count)=0;
 virtual void free_buffers(Handle*)=0;
 virtual Wave* allocate_wave()=0;
 virtual void free_wave(Wave*)=0;
 virtual Sound* allocate_sound()=0;
 virtual U32 create_buffer(Handle,const BufferDescription&,Handle&)=0;
 virtual U32 duplicate_buffer(Handle,Handle,Handle&)=0;
 virtual U32 set_position(Handle,U32)=0;
};
Sound& construct_sound(Sound&,std::span<const Handle>,U32,Wave&,MmioServices&,SoundServices&,CreationServices&); //00403FF0
U32 create_sound(Handle,Sound**,const char*,U32 ignored_flags,const std::array<U32,4>& algorithm,U32 count,I32 cd_drive,MmioServices&,SoundServices&,CreationServices&); //00403D20
}

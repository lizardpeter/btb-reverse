#pragma once
#include "btb/original_startup11.hpp"
#include "btb/original_runtime6.hpp"
#include "btb/original_runtime15.hpp"
namespace btb::original_file7 {
using namespace original_runtime16;
using DescriptorTable=original_startup11::DescriptorTable;
using ErrorPair=original_runtime8::ErrorPair;
struct CreateRequest {U32 access{},share{},disposition{},attributes{};bool inherit{};};
class Services:public original_startup11::DescriptorServices {
public:
 virtual bool read_file(U32,std::byte*,U32,U32& actual)=0;
 virtual bool write_file(U32,const std::byte*,U32,U32& actual)=0;
 virtual U32 create_file(const char*,const CreateRequest&)=0;
 virtual bool end_of_file(U32)=0;
 virtual void set_standard_handle(U32,U32)=0;
 virtual bool flush_file(U32)=0;
 virtual Stream32* allocate_stream(std::size_t)=0;
 virtual U32 allocate_guest_bytes(U32)=0;
 virtual std::byte* guest_bytes(U32 address,U32 length)=0;
 virtual U32 guest_address(const std::byte*)=0;
};
struct Context {IoState& state;DescriptorTable& table;Services& services;std::span<const ErrorPair> errors;U32 default_mode{0x4000},permission_mask{},buffer_allocations{};std::uint8_t invalid_flags{};};
I32 read_descriptor(Context&,U32,std::byte*,U32); //00433961
I32 write_descriptor(Context&,U32,const std::byte*,U32); //0043407D
I32 resize_file(Context&,U32,I32 length); //004390CC
I32 open_descriptor(Context&,const char*,U32 flags,U32 share,U32 permissions); //00437BFA
U32 flush_byte(Context&,U32 byte,Stream32&,U32 guest_stream); //0043285C
U32 write_items(Context&,U32 guest_data,U32 size,U32 count,Stream32&,U32 guest_stream); //00430A52
U32 read_items(Context&,U32 guest_data,U32 size,U32 count,Stream32&,U32 guest_stream); //00430937
// Connects existing open/flush/commit/fclose/input code to the actual descriptor bodies.
class Link:public IoServices,public original_runtime6::OpenServices,public original_runtime15::InputServices {
public:
 explicit Link(Context& c):context_(c){}
 void set_standard_handle(U32,U32) override;
 bool flush_file(U32) override;
 U32 last_error() override;
 U32 write_descriptor(U32,U32,U32) override;
 Stream32* allocate_stream(std::size_t) override;
 bool is_terminal(U32) override;
 U32 allocate_guest_bytes(U32) override;
 I32 open_file(const char*,U32,U32,U32) override;
 void* allocate(std::size_t) override;
 I32 read_descriptor(U32,std::byte*,U32) override;
 std::uint8_t descriptor_flags(U32) override;
private:Context& context_;
};
}

#pragma once
#include "btb/original_runtime16.hpp"
#include "btb/original_runtime8.hpp"
#include <functional>
#include <memory>
#include <string>
#include <vector>
namespace btb::original_startup11 {
using namespace btb::original_runtime16;
using ErrorPair=btb::original_runtime8::ErrorPair;
struct ParsedArguments {std::vector<std::string> values;U32 pointer_count{},byte_count{};};
// 00435A9D. Lead-byte flags are the original DAT_0051C7E0 table, +1 bias.
ParsedArguments parse_command_line(const char*,std::span<const std::uint8_t,257>,bool count_only=false);
class ProcessServices {
public:
    virtual ~ProcessServices()=default;
    virtual void initialize_multibyte()=0;
    virtual std::string module_file_name(U32 maximum)=0;
    virtual void* allocate(U32 bytes)=0;
    virtual void release(void*) noexcept=0;
    [[noreturn]] virtual void fatal_runtime_error(U32)=0;
    virtual void terminate_process(U32)=0;
    virtual void exit_process(U32)=0;
};
struct Arguments {void* allocation{};std::vector<char*> argv;I32 argc{};std::string program;};
// 00435A04, real two-pass contiguous argv construction (native pointer width).
void initialize_arguments(Arguments&,const char*,bool multibyte_initialized,
    std::span<const std::uint8_t,257>,ProcessServices&);
struct Environment {char* raw{};char** values{};bool initialized{};};
// 0043594B. Discard hidden '=C:' entries, free the original environment block.
void initialize_environment(Environment&,bool multibyte_initialized,ProcessServices&);
struct ExitState {bool already_exiting{},cleanup_started{};std::uint8_t return_to_caller{};};
// 00430884. Reverse onexit callbacks, then both forward initializer tables.
void process_exit(ExitState&,U32 code,bool quick,U32 return_to_caller,
    std::span<const Initializer> on_exit,std::span<const Initializer> pre,
    std::span<const Initializer> final,ProcessServices&);
// 00433640, original strcat pointer return.
char* concatenate(char*,const char*) noexcept;
class GuestMemory {public:virtual ~GuestMemory()=default;virtual U32 read32(U32)=0;};
// 0043259C, PMD base/member/virtual displacement adjustment.
U32 adjust_pointer(U32 object,const std::array<I32,3>& pmd,GuestMemory&);
class AllocationServices {
public:
    virtual ~AllocationServices()=default;
    virtual void* small_heap3(U32)=0;
    virtual void* small_heap2(U32 units)=0;
    virtual void* zero_heap(U32)=0;
    virtual bool new_handler(U32)=0;
};
// 004382FB: original wrapped multiplication, size rounding and retry loop.
void* allocate_zero(U32 count,U32 size,U32 heap_mode,U32 small_limit,
    bool use_new_handler,AllocationServices&);
using DescriptorBlock=std::array<Descriptor32,32>;
class DescriptorServices {
public:
    virtual ~DescriptorServices()=default;
    virtual DescriptorBlock* allocate_block()=0;
    virtual U32 standard_handle(U32 which)=0;
    virtual U32 file_type(U32 handle)=0;
    virtual void set_handle_count(U32)=0;
    virtual bool close_handle(U32)=0;
    virtual U32 seek_handle(U32,I32,U32)=0;
    virtual U32 last_error()=0;
};
struct DescriptorTable {std::array<DescriptorBlock*,64> blocks{};U32 limit{};};
// 0043619C / 00435D83. Startup reserved bytes contain count, flags, handles.
I32 allocate_descriptor(DescriptorTable&,DescriptorServices&);
void initialize_descriptors(DescriptorTable&,std::span<const std::byte> reserved,DescriptorServices&);
// 00431129 / 004372D7, connected original handle-release and errno mapping.
I32 close_descriptor(IoState&,U32,IoServices&,DescriptorServices&,std::span<const ErrorPair>);
U32 seek_descriptor(IoState&,U32,I32,U32,DescriptorServices&,std::span<const ErrorPair>);
// Direct 0042FB62 fclose integration: existing flush/commit and new close.
btb::original_crt10::Io connect_close(BufferedIoLink&,IoState&,IoServices&,
    DescriptorServices&,std::span<const ErrorPair>);
bool verify_11_original_startup_functions();
}

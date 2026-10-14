#pragma once
#include "btb/original_crt17.hpp"
#include "btb/original_crt10.hpp"
#include <array>
#include <span>
#include <cstdint>
#include <string_view>
#include <functional>
#include <utility>
namespace btb::original_runtime16 {
using U32=std::uint32_t;using I32=std::int32_t;using U64=std::uint64_t;
// Fields retain the original 32-byte FILE and eight-byte descriptor layout.
struct Stream32 { std::array<U32,8> words{}; };
struct Descriptor32 { U32 handle{0xffffffffU};std::uint8_t flags{};std::array<std::uint8_t,3> reserved{}; };
static_assert(sizeof(Stream32)==32 && sizeof(Descriptor32)==8);
struct IoState { std::span<Descriptor32> descriptors;U32 dos_error{};I32 error{};bool console_application{}; };
class IoServices {
public:
    virtual ~IoServices()=default;
    virtual void set_standard_handle(U32 which,U32 handle)=0;
    virtual bool flush_file(U32 handle)=0;
    virtual U32 last_error()=0;
    // 0043407D dependency owns original descriptor/text-mode translation.
    virtual U32 write_descriptor(U32 descriptor,U32 guest_buffer,U32 count)=0;
    virtual Stream32* allocate_stream(std::size_t bytes)=0;
    virtual bool is_terminal(U32 descriptor)=0;
    virtual U32 allocate_guest_bytes(U32 bytes)=0;
};
// 00436231 / 004362A8 / 0043635F / 00439627
I32 assign_os_handle(IoState&,U32 descriptor,U32 handle,IoServices&);
I32 release_os_handle(IoState&,U32 descriptor,IoServices&);
I32 commit_os_handle(IoState&,U32 descriptor,IoServices&);
I32 set_descriptor_mode(IoState&,U32 descriptor,I32 mode);
// 00434005 / 00431242 / 004356A5
Stream32* acquire_stream(std::span<Stream32*>,IoServices&);
I32 flush_stream(Stream32&,IoServices&);
enum class StandardStream { Other,Out,Error };
I32 begin_standard_buffering(Stream32&,U32 guest_stream_address,
    StandardStream,std::array<U32,2>& buffers,U32& attempts,IoServices&);
// 00435630 / 004380F0
U64 unsigned_remainder(U64 numerator,U64 denominator);
char* find_character(char*,int) noexcept;
// 0043867A: lookup by the second word of each three-word record.
struct ActionRecord { U32 code{};I32 signal{};U32 action{}; };
const ActionRecord* find_action_record(std::span<const ActionRecord>,I32) noexcept;
// 0043422A: read major/minor linker bytes, not OS/subsystem version.
U32 read_linker_version(std::span<const std::byte> module);
// 004320AF: decorated type equality and exception CV/property gates.
struct Type { const char* decorated_name{}; };
struct CatchType {std::uint8_t properties{};const Type* type{};};
bool catch_type_matches(const CatchType& thrown,const CatchType& caught,U32 qualifiers);
// 00437A60: decimal carry, padding and source decimal-point mutation.
void round_decimal_digits(std::span<char> output,I32 digits,std::string_view source,I32& decimal_point);
// 004303BA / 004303F7, reuse the ONE exception state and destructor type.
using Allocate=void*(*)(std::size_t);
btb::original_crt17::Exception& construct_exception(
    btb::original_crt17::Exception&,const char*,Allocate);
btb::original_crt17::Exception& copy_exception(
    btb::original_crt17::Exception&,const btb::original_crt17::Exception&,Allocate);
// 00430835 calls optional hook, then 0043091D on the two original tables.
using Initializer=void(*)();
void run_runtime_initializers(Initializer hook,std::span<const Initializer> first,
    std::span<const Initializer> second);
// Real linkage for 00431207 / 004312A7 / 0042FB62: use the recovered
// 00431242 flush and 0043635F commit instead of a caller-supplied stand-in.
class BufferedIoLink {
public:
    BufferedIoLink(IoState& state,IoServices& services,btb::original_crt10::Io upstream)
        : state_(state),services_(services),upstream_(upstream) {}
    BufferedIoLink(const BufferedIoLink&)=delete;
    BufferedIoLink& operator=(const BufferedIoLink&)=delete;
    btb::original_crt10::Io callbacks();
    void set_descriptor_close(std::function<I32(U32)> close){close_=std::move(close);}
private:
    IoState& state_;IoServices& services_;btb::original_crt10::Io upstream_;
    std::function<I32(U32)> close_;
};
bool verify_16_original_runtime_functions();
} // namespace btb::original_runtime16

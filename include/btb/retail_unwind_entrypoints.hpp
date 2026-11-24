#pragma once
// All original MSVC EH cleanup landing pads recovered from the PE32 Ghidra
// output. Source functions take an explicit frame pointer in place of EBP.
// This models the cleanup target, not the x86 EH runtime calling convention.
#include <array>
#include <cstddef>
#include <cstdint>
#include <cstring>

namespace btb::retail_unwind {
enum class Kind { String, Exception, IndirectFree, IndirectException, Printer };
using Cleanup = void (*)(void*) noexcept;
struct Hooks { Cleanup string_tidy{}; Cleanup exception_destruct{}; Cleanup free_memory{}; Cleanup printer_destruct{}; };
struct Entry {
    std::uint32_t address{};
    Kind kind{};
    std::size_t offset{};
    void (*invoke)(std::byte*, Hooks) noexcept{};
};
inline void invoke_unwind(Kind kind,std::size_t offset,std::byte* frame,Hooks h) noexcept {
    auto* slot=frame-offset;
    void* object=slot;
    if(kind==Kind::IndirectFree || kind==Kind::IndirectException)
        std::memcpy(&object,slot,sizeof(object));
    switch(kind) {
      case Kind::String: if(h.string_tidy)h.string_tidy(object);break;
      case Kind::Exception:
      case Kind::IndirectException:
        if(h.exception_destruct)h.exception_destruct(object);break;
      case Kind::IndirectFree: if(h.free_memory)h.free_memory(object);break;
      case Kind::Printer: if(h.printer_destruct)h.printer_destruct(object);break;
    }
}
// Original 0x0043A360: indirect_free at EBP - 0x10.
inline void unwind_0043A360(std::byte* frame,Hooks hooks) noexcept {
    invoke_unwind(Kind::IndirectFree,0x10,frame,hooks);
}
// Original 0x0043A380: indirect_free at EBP - 0x9c.
inline void unwind_0043A380(std::byte* frame,Hooks hooks) noexcept {
    invoke_unwind(Kind::IndirectFree,0x9c,frame,hooks);
}
// Original 0x0043A38E: indirect_free at EBP - 0xa4.
inline void unwind_0043A38E(std::byte* frame,Hooks hooks) noexcept {
    invoke_unwind(Kind::IndirectFree,0xa4,frame,hooks);
}
// Original 0x0043A3B0: string at EBP - 0x50.
inline void unwind_0043A3B0(std::byte* frame,Hooks hooks) noexcept {
    invoke_unwind(Kind::String,0x50,frame,hooks);
}
// Original 0x0043A3B8: exception at EBP - 0x28.
inline void unwind_0043A3B8(std::byte* frame,Hooks hooks) noexcept {
    invoke_unwind(Kind::Exception,0x28,frame,hooks);
}
// Original 0x0043A3D0: string at EBP - 0x54.
inline void unwind_0043A3D0(std::byte* frame,Hooks hooks) noexcept {
    invoke_unwind(Kind::String,0x54,frame,hooks);
}
// Original 0x0043A3D8: exception at EBP - 0x44.
inline void unwind_0043A3D8(std::byte* frame,Hooks hooks) noexcept {
    invoke_unwind(Kind::Exception,0x44,frame,hooks);
}
// Original 0x0043A3E0: string at EBP - 0x64.
inline void unwind_0043A3E0(std::byte* frame,Hooks hooks) noexcept {
    invoke_unwind(Kind::String,0x64,frame,hooks);
}
// Original 0x0043A3E8: exception at EBP - 0x28.
inline void unwind_0043A3E8(std::byte* frame,Hooks hooks) noexcept {
    invoke_unwind(Kind::Exception,0x28,frame,hooks);
}
// Original 0x0043A400: string at EBP - 0x30.
inline void unwind_0043A400(std::byte* frame,Hooks hooks) noexcept {
    invoke_unwind(Kind::String,0x30,frame,hooks);
}
// Original 0x0043A408: exception at EBP - 0x60.
inline void unwind_0043A408(std::byte* frame,Hooks hooks) noexcept {
    invoke_unwind(Kind::Exception,0x60,frame,hooks);
}
// Original 0x0043A410: string at EBP - 0x20.
inline void unwind_0043A410(std::byte* frame,Hooks hooks) noexcept {
    invoke_unwind(Kind::String,0x20,frame,hooks);
}
// Original 0x0043A418: exception at EBP - 0x7c.
inline void unwind_0043A418(std::byte* frame,Hooks hooks) noexcept {
    invoke_unwind(Kind::Exception,0x7c,frame,hooks);
}
// Original 0x0043A430: string at EBP - 0x38.
inline void unwind_0043A430(std::byte* frame,Hooks hooks) noexcept {
    invoke_unwind(Kind::String,0x38,frame,hooks);
}
// Original 0x0043A438: exception at EBP - 0x28.
inline void unwind_0043A438(std::byte* frame,Hooks hooks) noexcept {
    invoke_unwind(Kind::Exception,0x28,frame,hooks);
}
// Original 0x0043A450: string at EBP - 0xfc.
inline void unwind_0043A450(std::byte* frame,Hooks hooks) noexcept {
    invoke_unwind(Kind::String,0xfc,frame,hooks);
}
// Original 0x0043A45B: exception at EBP - 0xe8.
inline void unwind_0043A45B(std::byte* frame,Hooks hooks) noexcept {
    invoke_unwind(Kind::Exception,0xe8,frame,hooks);
}
// Original 0x0043A466: string at EBP - 0xfc.
inline void unwind_0043A466(std::byte* frame,Hooks hooks) noexcept {
    invoke_unwind(Kind::String,0xfc,frame,hooks);
}
// Original 0x0043A471: exception at EBP - 0xe8.
inline void unwind_0043A471(std::byte* frame,Hooks hooks) noexcept {
    invoke_unwind(Kind::Exception,0xe8,frame,hooks);
}
// Original 0x0043A47C: string at EBP - 0xfc.
inline void unwind_0043A47C(std::byte* frame,Hooks hooks) noexcept {
    invoke_unwind(Kind::String,0xfc,frame,hooks);
}
// Original 0x0043A487: exception at EBP - 0xe8.
inline void unwind_0043A487(std::byte* frame,Hooks hooks) noexcept {
    invoke_unwind(Kind::Exception,0xe8,frame,hooks);
}
// Original 0x0043A492: string at EBP - 0xcc.
inline void unwind_0043A492(std::byte* frame,Hooks hooks) noexcept {
    invoke_unwind(Kind::String,0xcc,frame,hooks);
}
// Original 0x0043A49D: exception at EBP - 0xbc.
inline void unwind_0043A49D(std::byte* frame,Hooks hooks) noexcept {
    invoke_unwind(Kind::Exception,0xbc,frame,hooks);
}
// Original 0x0043A4C0: string at EBP - 0x6c.
inline void unwind_0043A4C0(std::byte* frame,Hooks hooks) noexcept {
    invoke_unwind(Kind::String,0x6c,frame,hooks);
}
// Original 0x0043A4C8: exception at EBP - 0x44.
inline void unwind_0043A4C8(std::byte* frame,Hooks hooks) noexcept {
    invoke_unwind(Kind::Exception,0x44,frame,hooks);
}
// Original 0x0043A4D0: string at EBP - 0x7c.
inline void unwind_0043A4D0(std::byte* frame,Hooks hooks) noexcept {
    invoke_unwind(Kind::String,0x7c,frame,hooks);
}
// Original 0x0043A4D8: exception at EBP - 0x28.
inline void unwind_0043A4D8(std::byte* frame,Hooks hooks) noexcept {
    invoke_unwind(Kind::Exception,0x28,frame,hooks);
}
// Original 0x0043A4F0: string at EBP - 0x90.
inline void unwind_0043A4F0(std::byte* frame,Hooks hooks) noexcept {
    invoke_unwind(Kind::String,0x90,frame,hooks);
}
// Original 0x0043A4FB: exception at EBP - 0x7c.
inline void unwind_0043A4FB(std::byte* frame,Hooks hooks) noexcept {
    invoke_unwind(Kind::Exception,0x7c,frame,hooks);
}
// Original 0x0043A510: indirect_exception at EBP - 0x10.
inline void unwind_0043A510(std::byte* frame,Hooks hooks) noexcept {
    invoke_unwind(Kind::IndirectException,0x10,frame,hooks);
}
// Original 0x0043A540: indirect_free at EBP - 0x10.
inline void unwind_0043A540(std::byte* frame,Hooks hooks) noexcept {
    invoke_unwind(Kind::IndirectFree,0x10,frame,hooks);
}
// Original 0x0043A560: printer at EBP - 0x530.
inline void unwind_0043A560(std::byte* frame,Hooks hooks) noexcept {
    invoke_unwind(Kind::Printer,0x530,frame,hooks);
}
// Original 0x0043A578: string at EBP - 0x20.
inline void unwind_0043A578(std::byte* frame,Hooks hooks) noexcept {
    invoke_unwind(Kind::String,0x20,frame,hooks);
}
// Original 0x0043A58C: indirect_exception at EBP - 0x14.
inline void unwind_0043A58C(std::byte* frame,Hooks hooks) noexcept {
    invoke_unwind(Kind::IndirectException,0x14,frame,hooks);
}
// Original 0x0043A5A0: indirect_exception at EBP - 0x10.
inline void unwind_0043A5A0(std::byte* frame,Hooks hooks) noexcept {
    invoke_unwind(Kind::IndirectException,0x10,frame,hooks);
}
// Original 0x0043A5B4: indirect_exception at EBP - 0x10.
inline void unwind_0043A5B4(std::byte* frame,Hooks hooks) noexcept {
    invoke_unwind(Kind::IndirectException,0x10,frame,hooks);
}
// Original 0x0043A5C8: indirect_exception at EBP - 0x10.
inline void unwind_0043A5C8(std::byte* frame,Hooks hooks) noexcept {
    invoke_unwind(Kind::IndirectException,0x10,frame,hooks);
}
// Original 0x0043A5DC: string at EBP - 0x20.
inline void unwind_0043A5DC(std::byte* frame,Hooks hooks) noexcept {
    invoke_unwind(Kind::String,0x20,frame,hooks);
}
// Original 0x0043A5F0: indirect_exception at EBP - 0x10.
inline void unwind_0043A5F0(std::byte* frame,Hooks hooks) noexcept {
    invoke_unwind(Kind::IndirectException,0x10,frame,hooks);
}
inline constexpr std::array<Entry,39> kEntries{{
    {0x0043A360U,Kind::IndirectFree,0x10,&unwind_0043A360},
    {0x0043A380U,Kind::IndirectFree,0x9c,&unwind_0043A380},
    {0x0043A38EU,Kind::IndirectFree,0xa4,&unwind_0043A38E},
    {0x0043A3B0U,Kind::String,0x50,&unwind_0043A3B0},
    {0x0043A3B8U,Kind::Exception,0x28,&unwind_0043A3B8},
    {0x0043A3D0U,Kind::String,0x54,&unwind_0043A3D0},
    {0x0043A3D8U,Kind::Exception,0x44,&unwind_0043A3D8},
    {0x0043A3E0U,Kind::String,0x64,&unwind_0043A3E0},
    {0x0043A3E8U,Kind::Exception,0x28,&unwind_0043A3E8},
    {0x0043A400U,Kind::String,0x30,&unwind_0043A400},
    {0x0043A408U,Kind::Exception,0x60,&unwind_0043A408},
    {0x0043A410U,Kind::String,0x20,&unwind_0043A410},
    {0x0043A418U,Kind::Exception,0x7c,&unwind_0043A418},
    {0x0043A430U,Kind::String,0x38,&unwind_0043A430},
    {0x0043A438U,Kind::Exception,0x28,&unwind_0043A438},
    {0x0043A450U,Kind::String,0xfc,&unwind_0043A450},
    {0x0043A45BU,Kind::Exception,0xe8,&unwind_0043A45B},
    {0x0043A466U,Kind::String,0xfc,&unwind_0043A466},
    {0x0043A471U,Kind::Exception,0xe8,&unwind_0043A471},
    {0x0043A47CU,Kind::String,0xfc,&unwind_0043A47C},
    {0x0043A487U,Kind::Exception,0xe8,&unwind_0043A487},
    {0x0043A492U,Kind::String,0xcc,&unwind_0043A492},
    {0x0043A49DU,Kind::Exception,0xbc,&unwind_0043A49D},
    {0x0043A4C0U,Kind::String,0x6c,&unwind_0043A4C0},
    {0x0043A4C8U,Kind::Exception,0x44,&unwind_0043A4C8},
    {0x0043A4D0U,Kind::String,0x7c,&unwind_0043A4D0},
    {0x0043A4D8U,Kind::Exception,0x28,&unwind_0043A4D8},
    {0x0043A4F0U,Kind::String,0x90,&unwind_0043A4F0},
    {0x0043A4FBU,Kind::Exception,0x7c,&unwind_0043A4FB},
    {0x0043A510U,Kind::IndirectException,0x10,&unwind_0043A510},
    {0x0043A540U,Kind::IndirectFree,0x10,&unwind_0043A540},
    {0x0043A560U,Kind::Printer,0x530,&unwind_0043A560},
    {0x0043A578U,Kind::String,0x20,&unwind_0043A578},
    {0x0043A58CU,Kind::IndirectException,0x14,&unwind_0043A58C},
    {0x0043A5A0U,Kind::IndirectException,0x10,&unwind_0043A5A0},
    {0x0043A5B4U,Kind::IndirectException,0x10,&unwind_0043A5B4},
    {0x0043A5C8U,Kind::IndirectException,0x10,&unwind_0043A5C8},
    {0x0043A5DCU,Kind::String,0x20,&unwind_0043A5DC},
    {0x0043A5F0U,Kind::IndirectException,0x10,&unwind_0043A5F0},
}};
} // namespace btb::retail_unwind

#include "btb/retail_unwind_entrypoints.hpp"
#include <array>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <cstring>
using namespace btb::retail_unwind;
namespace {
void* latest{};
Kind latest_kind{};
void string_tidy(void* ptr) noexcept {latest=ptr; latest_kind=Kind::String;}
void except(void* ptr) noexcept {latest=ptr; latest_kind=Kind::Exception;}
void free_mem(void* ptr) noexcept {latest=ptr; latest_kind=Kind::IndirectFree;}
void printer(void* ptr) noexcept {latest=ptr; latest_kind=Kind::Printer;}
}
int main() {
    static_assert(kEntries.size()==39);
    static_assert(kEntries[0].address==0x0043A360U);
    static_assert(kEntries[38].address==0x0043A5F0U);
    alignas(void*) std::array<std::byte,4096> stack{};
    auto* frame=stack.data()+2048;
    int sentinel=19;
    Hooks h{&string_tidy,&except,&free_mem,&printer};
    for(std::size_t i=0;i<kEntries.size();++i) {
        const auto& e=kEntries[i];
        if(i)assert(kEntries[i-1].address<e.address);
        assert(e.offset>0&&e.offset<=0x530);
        auto* location=frame-e.offset;
        void* expected=location;
        if(e.kind==Kind::IndirectException||e.kind==Kind::IndirectFree) {
            expected=&sentinel;
            std::memcpy(location,&expected,sizeof(expected));
        }
        latest=nullptr;
        e.invoke(frame,h);
        assert(latest==expected);
        const Kind expected_kind=
           e.kind==Kind::IndirectException?Kind::Exception:e.kind;
        assert(latest_kind==expected_kind);
    }
}

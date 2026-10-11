#include "btb/original_architecture_615.hpp"

#include <cassert>
#include <cstddef>
#include <cstdint>

namespace {
constexpr bool unique_address_inventory() {
    constexpr auto& a=btb::evidence::kOriginalArchitecture615;
    for (std::size_t i=0;i<a.size();++i)
        for (std::size_t j=i+1;j<a.size();++j)
            if(a[i].address==a[j].address)return false;
    return true;
}
constexpr std::size_t source_written_count() {
    std::size_t value=0;
    for(const auto& f:btb::evidence::kOriginalArchitecture615)
        if(f.has_source_mapping)++value;
    return value;
}
}
int main() {
    static_assert(btb::evidence::kOriginalArchitecture615.size()==615);
    static_assert(unique_address_inventory());
    static_assert(source_written_count()==191);
    std::size_t runtime_functions=0;
    std::size_t prototypes_with_thiscall=0;
    for(const auto& f:btb::evidence::kOriginalArchitecture615) {
        assert(f.address>=0x00400000U);
        assert(!f.original_name.empty());
        assert(!f.module_candidate.empty());
        assert(!f.owner_candidate.empty());
        if(f.module_candidate=="static_runtime")++runtime_functions;
        if(f.original_thiscall)++prototypes_with_thiscall;
    }
    assert(runtime_functions>=224);
    assert(prototypes_with_thiscall>0);
}

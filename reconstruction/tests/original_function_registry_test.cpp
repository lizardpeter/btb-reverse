#include "btb/original_function_registry.hpp"

#include <cassert>
#include <cstddef>
#include <cstdint>
#include <string_view>

using namespace btb::reconstruction;

int main() {
    // This test verifies the inventory, not behavior of any retail function.
    static_assert(kOriginalFunctions.size() == 615);
    static_assert(find_original_function(0x00401000U) != nullptr);
    static_assert(find_original_function(0x0040A440U) != nullptr);
    static_assert(find_original_function(0x0043A5F0U) != nullptr);
    static_assert(find_original_function(0x003FFFFFU) == nullptr);
    static_assert(find_original_function(0x00400000U) == nullptr);

    const auto* dinosaur = find_original_function(0x0040A440U);
    assert(dinosaur != nullptr);
    assert(dinosaur->name == std::string_view{"LoadDinoLevelData"});
    assert(dinosaur->subsystem == OriginalSubsystem::Dinosaur);
    assert(dinosaur->stage == ReconstructionStage::Unreviewed);

    std::size_t reviewed = 0;
    for (std::size_t i = 0; i < kOriginalFunctions.size(); ++i) {
        const auto& function = kOriginalFunctions[i];
        assert(function.address >= 0x00401000U);
        assert(!function.name.empty());
        assert(function.body_bytes > 0);
        assert(find_original_function(function.address) == &function);
        if (i > 0) {
            assert(kOriginalFunctions[i - 1].address < function.address);
        }
        if (function.stage != ReconstructionStage::Unreviewed) {
            ++reviewed;
        }
    }
    // Reviewed coverage is counted from explicit evidence-backed status
    // overrides, never inferred from compilation of the metadata library.
    (void)reviewed;
}

#include "btb/park_designer_runtime.hpp"

namespace btb::park_designer {

bool has_any_placed_object(const SaveData& data) noexcept {
    for (const auto& object : data.objects) {
        if (object.object_code != -1) {
            return true;
        }
    }
    return false;
}

} // namespace btb::park_designer

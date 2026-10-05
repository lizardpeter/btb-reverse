#include "btb/park_designer_runtime.hpp"

#include <cassert>

using namespace btb::park_designer;

int main() {
    auto step = completion_step(0, false, 0);
    assert(step.stage == 1);
    assert(step.action == CompletionAction::PlayClosingLine);
    assert(step.sound_id == 246);

    step = completion_step(0, false, 2);
    assert(step.sound_id == 248);

    step = completion_step(1, true, 0);
    assert(step.stage == 1);
    assert(step.action == CompletionAction::None);

    step = completion_step(1, false, 0);
    assert(step.stage == 2);
    assert(step.action == CompletionAction::None);

    step = completion_step(2, false, 0);
    assert(step.action == CompletionAction::SaveUnloadAndExit);

    SaveData data{};
    for (auto& object : data.objects) {
        object.object_code = -1;
    }
    assert(!has_any_placed_object(data));
    data.objects[399].object_code = 7;
    assert(has_any_placed_object(data));
}

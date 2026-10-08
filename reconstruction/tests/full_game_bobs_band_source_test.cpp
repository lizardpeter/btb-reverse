#include "btb/full_game_bobs_band.hpp"

#include <cassert>
#include <filesystem>
#include <fstream>
#include <iterator>
#include <memory>
#include <string>

int main() {
    using namespace btb::full_game;
    namespace fs = std::filesystem;

    const auto root = fs::temp_directory_path() / "btb_full_game_band_fixture";
    const auto originals = root / "Data" / "subgameopen";
    const auto saved = root / "profile";
    fs::create_directories(originals);
    fs::create_directories(saved);

    // The original installed game's numeric machinedata.txt records, not
    // synthetic machine sprite dimensions or UI-editor hitboxes.
    {
        std::ofstream out(originals / "machinedata.txt");
        out << R"(0 138
0 140
98 117
100 120
219 1
228 25
349 113
352 123
410 70
406 81

168 157
216 166
174 128
194 144
142 216
169 191
73 95
63 78
105 136
121 139

525 141
519 145
524 142

56 71
56 68
52 72

59 59 64

259 264 319 290
206 246 258 289
323 248 378 276
264 224 318 262
353 219 397 247
320 196 351 245
413 193 445 232
354 183 409 217
493 200 525 248
448 193 491 252

static machine animation rectangles
)";
    }

    GameRoot game;
    assert(game.select_profile(0));
    auto band = std::make_unique<BobsBandDriver>(
        originals, saved, btb::bobs_band::Conductor::Bob);
    auto* installed = band.get();
    game.install(ActivityId::BobsBand, std::move(band));
    game.set_outer_state(btb::game_flow::State::BobsBandInit);

    assert(game.advance({}).kind == FrameKind::ActivityInitialized);
    assert(installed->activity());
    assert(installed->activity()->active_sequence_filename() ==
           "musicbob1.txt");

    ActivityFrameInput select_roleys_short{};
    select_roleys_short.pointer_x = 245;
    select_roleys_short.pointer_y = 295;
    select_roleys_short.click_pulse = true;
    auto palette = game.advance(select_roleys_short);
    assert(palette.kind == FrameKind::ActivityUpdated);
    assert(!palette.effects.audio.empty());

    ActivityFrameInput place{};
    place.pointer_x = 45;
    place.pointer_y = 311;
    place.click_pulse = true;
    auto placed = game.advance(place);
    assert(placed.kind == FrameKind::ActivityUpdated);
    assert(installed->activity()->editor().composition().at({0,0}) == 0);

    ActivityFrameInput request_play{};
    request_play.pointer_x = 350;
    request_play.pointer_y = 441;
    request_play.click_pulse = true;
    auto pressed = game.advance(request_play);
    assert(pressed.kind == FrameKind::ActivityUpdated);
    assert(!pressed.effects.audio.empty());
    assert(installed->activity()->editor().play_pending());

    ActivityFrameInput waiting{};
    waiting.managed_sound_playing = true;
    static_cast<void>(game.advance(waiting));
    assert(installed->activity()->editor().play_pending());

    const auto started = game.advance({});
    assert(started.effects.progress_writes.size() == 1);
    assert(game.globals().player_progress[0].get(
        btb::progress::Slot::BobsBandBob) == 1);
    assert(game.globals().shared_completion_code == 6);
    assert(started.effects.audio.size() == 1);
    assert(started.effects.audio[0].source_asset.find("bobmt.wav") !=
           std::string::npos);

    const auto sec0 = game.advance({});
    assert(!sec0.effects.draws.empty());
    bool found_machine_note = false;
    for (const auto& audio : sec0.effects.audio) {
        if (audio.source_asset.find("roley1_5.wav") != std::string::npos) {
            found_machine_note = true;
        }
    }
    assert(found_machine_note);

    // Exit drives BOTH original persistent file writes before switching
    // global game state to Play Again 0x3C.
    auto* runtime = installed->activity();
    assert(runtime);
    const_cast<btb::bobs_band::Activity*>(runtime)->leave_to_play_again();
    const auto leave = game.advance({});
    assert(leave.effects.save_and_unload);
    assert(leave.kind == FrameKind::ActivityUpdated);
    assert(leave.replay_preparation);
    assert(leave.replay_preparation->voice_sound_id == 573);
    assert(game.globals().menu.replay_active_latch);
    assert(leave.effects.audio.size() >= 2);
    assert(leave.effects.audio[leave.effects.audio.size()-2].operation ==
           AudioOperation::StopManagedSounds);
    assert(leave.effects.audio.back().sound_id == 573);
    assert(leave.effects.audio.back().input_interruptible);
    assert(game.globals().dispatcher.current_state == 0x3C);
    assert(game.globals().ui_context == 0x2E);
    assert(installed->activity() == nullptr);
    assert(fs::file_size(saved / "musicbob1.txt") == 480);
    assert(fs::file_size(saved / "last.txt") == 20);
    assert(game.globals().dispatcher.saved_state == 0x2E);
    assert(game.globals().menu.replay_class == 1);

    // A different original Music chooser action now reaches the same
    // installed Band driver, not a hard-coded Bob conductor. -3 means
    // Wendy and -4 Farmer Pickles in retail 0x42C4F4 / 0x42C50A.
    game.globals().menu.subgame_selection_origin =
        btb::game_flow::State::MusicChooserUpdate;
    game.globals().menu.selected_subgame = 1;
    game.set_outer_state(btb::game_flow::State::BobsBandInit);
    assert(game.advance({}).kind == FrameKind::ActivityInitialized);
    assert(installed->activity()->conductor() ==
           btb::bobs_band::Conductor::Wendy);
    assert(installed->activity()->active_sequence_filename() ==
           "musicwendy1.txt");
    std::string error;
    assert(game.unload_current(error));

    game.globals().menu.selected_subgame = 2;
    game.set_outer_state(btb::game_flow::State::BobsBandInit);
    assert(game.advance({}).kind == FrameKind::ActivityInitialized);
    assert(installed->activity()->conductor() ==
           btb::bobs_band::Conductor::FarmerPickles);
    assert(installed->activity()->active_sequence_filename() ==
           "musicfarmer1.txt");
    assert(game.unload_current(error));

    fs::remove_all(root);
}

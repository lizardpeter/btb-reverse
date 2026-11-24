#include "btb/full_game_resources.hpp"

#include <cassert>
#include <filesystem>
#include <fstream>
#include <vector>

int main() {
    using namespace btb::full_game;
    namespace fs = std::filesystem;

    const auto root = fs::temp_directory_path() /
                      "btb_full_game_resource_resolution";
    fs::remove_all(root);
    fs::create_directories(root / "Data" / "SubGameOpen");
    fs::create_directories(root / "Data" / "sound");
    std::ofstream(root / "Data" / "SubGameOpen" /
                  "music_01.bmp", std::ios::binary) << "bitmap";
    std::ofstream(root / "Data" / "sound" /
                  "MP_BOB_01.wav", std::ios::binary) << "RIFF";

    OriginalAssetResolver resolver(root);
    std::vector<Draw> draws{
        {"data\\subgameopen\\music_01.BMP",0,0,std::nullopt,false},
        {"Data\\SubGameOpen\\missing.bmp",42,30,
            Rect{0,0,16,16},true},
    };

    AudioFramePlan plan;
    plan.operations = {
        {SoundEffectKind::LoadManagedWav,0,510,
            "Data/sound/MP_BOB_01.wav"},
        {SoundEffectKind::PlayManagedSlot,0,510},
        {SoundEffectKind::StartBackingWav,-1,-1,
            "Data/SubGameOpen/missing.wav"},
        {SoundEffectKind::StopBackingWav},
    };

    const auto frame = resolve_game_frame_assets(
        resolver, draws, plan);
    assert(frame.draws.size() == 2);
    assert(frame.sound.size() == 4);
    assert(frame.unresolved_draws == 1);
    assert(frame.unresolved_audio_assets == 1);
    assert(!frame.ready_for_submission());
    assert(frame.draws[0].source.found());
    assert(frame.draws[1].original.color_keyed);
    assert(frame.draws[1].original.source_rectangle->right == 16);
    assert(frame.sound[0].source &&
           frame.sound[0].source->found());
    assert(!frame.sound[1].source);
    assert(frame.sound[2].source &&
           !frame.sound[2].source->found());
    assert(!frame.sound[3].source);
    fs::remove_all(root);
}

#include "btb/full_game_effect_pipeline.hpp"

#include <cassert>
#include <filesystem>
#include <fstream>
#include <sstream>

int main() {
    using namespace btb::full_game;
    namespace fs = std::filesystem;

    const auto root = fs::temp_directory_path() /
                      "btb_game_effect_pipeline_source_test";
    fs::remove_all(root);
    fs::create_directories(root / "Data" / "sound");
    fs::create_directories(root / "Data" / "SubGameOpen");

    std::ofstream(root / "Data" / "sound" / "MP_BOB_01.wav",
                  std::ios::binary) << "original";
    std::ofstream(root / "Data" / "SubGameOpen" / "music_01.bmp",
                  std::ios::binary) << "original";

    GameEffectPipeline pipeline{OriginalAssetResolver{root}};
    std::istringstream list("MP_BOB_01.wav 510\n");
    const auto catalog = pipeline.load_sound_catalog(list);
    assert(catalog.success);
    assert(pipeline.audio().manager().filename_by_sound_id[510]
               .bytes[0] == 'M');

    GameFrame source{};
    source.kind = FrameKind::ActivityUpdated;
    source.effects.draws.push_back({
        "data\\subgameopen\\MUSIC_01.BMP",0,0,std::nullopt,false
    });
    source.effects.audio.push_back({
        AudioOperation::ManagedSoundId,{},510,50,2
    });
    auto first = pipeline.prepare(source,{},false,false);
    assert(first.game.kind == FrameKind::ActivityUpdated);
    assert(first.resources.ready_for_submission());
    assert(!first.missing_catalog_ids);
    assert(first.resources.draws.size() == 1);
    assert(first.resources.draws.front().source.found());
    assert(first.resources.sound.size() == 3);
    assert(first.resources.sound[0].source);
    assert(first.resources.sound[0].source->found());
    assert(first.resources.sound[1].source == std::nullopt);
    assert(first.audio_policy.managed_results[0].status == 1);

    // On the next frame the physical backend reports that the buffer has
    // finished. Retail reaps and rewinds BEFORE the activity requests it
    // again, permitting one cached replay without a new WAV allocation.
    GameFrame replay{};
    replay.effects.audio.push_back({
        AudioOperation::ManagedSoundId,{},510,50,2
    });
    auto second = pipeline.prepare(replay,{},false,false);
    assert(second.audio_policy.operations.size() == 2);
    assert(second.audio_policy.operations[0].kind ==
           SoundEffectKind::StopRewindManagedSlot);
    assert(second.audio_policy.operations[1].kind ==
           SoundEffectKind::PlayManagedSlot);
    assert(second.audio_policy.managed_results[0].status == 1);

    GameFrame missing{};
    missing.effects.draws.push_back({
        "Data/SubGameOpen/missing.bmp",0,0,std::nullopt,true
    });
    missing.effects.audio.push_back({
        AudioOperation::ManagedSoundId,{},599,50,2
    });
    auto third = pipeline.prepare(missing,{},false,false);
    assert(third.resources.unresolved_draws == 1);
    assert(third.missing_catalog_ids);
    assert(third.audio_policy.missing_catalog_sound_ids[0] == 599);

    fs::remove_all(root);
}

#include "btb/full_game_audio.hpp"
#include "btb/full_game_replay_transition.hpp"

#include <cassert>
#include <sstream>
#include <string>
#include <vector>

using namespace btb::full_game;

int main() {
    AudioEffectPlanner planner;
    std::istringstream original_like_catalog(
        "MP_BOB_01.wav 510\r\n"
        "MP_BOB_02.wav\t511\r\n"
        "MP_WEN_01.wav 523\n"
        "MP_PIC_01.wav 536\n"
        "PA_BOB_01.wav 573\n");
    auto parsed = planner.load_catalog(original_like_catalog);
    assert(parsed.success);
    assert(parsed.entries == 5);
    assert(planner.catalog().find(510));
    assert(planner.catalog().find(510)->filename == "MP_BOB_01.wav");
    assert(planner.catalog().find(509) == nullptr);

    std::istringstream duplicate("test.wav 510\nother.wav 510\n");
    const auto bad = planner.load_catalog(duplicate);
    assert(!bad.success);
    assert(bad.error_line == 2);
    // Reject damaged replacements without losing the installed catalog.
    assert(planner.catalog().size() == 5);
    assert(planner.catalog().find(510)->filename == "MP_BOB_01.wav");

    // Original managed Play acquisition plays a newly loaded buffer twice.
    std::vector<Audio> first{{
        AudioOperation::ManagedSoundId, {}, 510, 50, 2
    }};
    const auto initial = planner.plan(first, {});
    assert(initial.invalid_requests == 0);
    assert(initial.managed_results.size() == 1);
    assert(initial.managed_results[0].status == 1);
    assert(initial.operations.size() == 3);
    assert(initial.operations[0].kind == SoundEffectKind::LoadManagedWav);
    assert(initial.operations[0].filename ==
           "Data/sound/MP_BOB_01.wav");
    assert(initial.operations[0].slot == 0);
    assert(initial.operations[1].kind == SoundEffectKind::PlayManagedSlot);
    assert(initial.operations[2].kind == SoundEffectKind::PlayManagedSlot);

    ManagedSoundObservation playing{};
    playing.playing[0] = true;
    const auto duplicate_active = planner.plan(first, playing);
    assert(duplicate_active.managed_results[0].status == 2);
    assert(duplicate_active.operations.empty());

    // An incoming class-1 voice preempts an existing class-2 voice.
    std::vector<Audio> second{{
        AudioOperation::ManagedSoundId, {}, 511, 50, 1
    }};
    const auto preempt = planner.plan(second, playing);
    assert(preempt.managed_results[0].preempted_count == 1);
    assert(preempt.operations.size() == 4);
    assert(preempt.operations[0].kind ==
           SoundEffectKind::StopRewindManagedSlot);
    assert(preempt.operations[0].slot == 0);
    assert(preempt.operations[1].kind == SoundEffectKind::LoadManagedWav);
    assert(preempt.operations[1].slot == 1);

    ManagedSoundObservation exclusive{};
    exclusive.playing[1] = true;
    const auto refused = planner.plan(first, exclusive);
    assert(refused.managed_results.size() == 1);
    assert(refused.managed_results[0].status == 0);
    assert(refused.managed_results[0].rejected_by_exclusive_blocker);
    assert(refused.operations.empty());

    // 0x00402C90 must only stop *playing* groups. Cached stopped groups
    // remain untouched. Here slot 1 plays while slot 0 is allocated/stopped.
    const auto stop = planner.plan({{
        AudioOperation::StopManagedSounds
    }}, exclusive);
    assert(stop.operations.size() == 1);
    assert(stop.operations[0].kind ==
           SoundEffectKind::StopRewindManagedSlot);
    assert(stop.operations[0].slot == 1);
    assert(planner.manager().buffer_group_ptr32[0] != 0);
    assert(planner.manager().buffer_group_ptr32[1] != 0);
    assert(planner.manager().state(1) == btb::sound::SlotState::Stopped);
    // Repeating StopAll against nonplaying buffers must emit no extra stop.
    const auto stopped_again = planner.plan({{
        AudioOperation::StopManagedSounds
    }}, {});
    assert(stopped_again.operations.empty());
    const auto replay = planner.plan(first, {});
    assert(replay.managed_results[0].status == 1);
    assert(replay.operations.size() == 1);
    assert(replay.operations[0].kind == SoundEffectKind::PlayManagedSlot);

    const auto missing = planner.plan({{
        AudioOperation::ManagedSoundId, {}, 599, 90, 1
    }}, {});
    assert(missing.operations.empty());
    assert(missing.missing_catalog_sound_ids.size() == 1);
    assert(missing.missing_catalog_sound_ids[0] == 599);

    const auto direct = planner.plan({
        {AudioOperation::StartBackingTrack, "Data/SubGameOpen/bobmt.wav"},
        {AudioOperation::PlaySampleFile, "Data/SubGameOpen/roley1_5.wav"},
        {AudioOperation::StopBackingTrack},
    }, {});
    assert(direct.operations.size() == 3);
    assert(direct.operations[0].kind == SoundEffectKind::StartBackingWav);
    assert(direct.operations[1].kind == SoundEffectKind::PlayDirectSample);
    assert(direct.operations[2].kind == SoundEffectKind::StopBackingWav);

    planner.reset();
    assert(planner.manager().mapped_slot(510) == -1);

    const auto replay_plan = prepare_retail_play_again_transition();
    assert(replay_plan.voice_sound_id == 573);
    assert(replay_plan.voice_priority == 50);
    assert(replay_plan.voice_arbitration_class == 1);
    assert(replay_plan.voice_input_interruptible);
    assert(replay_plan.stop_all_managed_sounds);

    const auto voice = planner.plan({{
        AudioOperation::ManagedSoundId,{},573,50,1,true
    }},{});
    assert(voice.managed_results.size() == 1);
    assert(voice.managed_results[0].slot == 0);
    assert(planner.manager().input_interruptible[0] == 1);
    assert(voice.operations[0].filename ==
           "Data/sound/PA_BOB_01.wav");

    ManagedSoundObservation replay_playing{};
    replay_playing.playing[0] = true;
    const auto interrupted = planner.reap(
        replay_playing,true,false);
    assert(interrupted.operations.size() == 1);
    assert(interrupted.operations[0].kind ==
           SoundEffectKind::StopRewindManagedSlot);
    assert(interrupted.operations[0].slot == 0);

    // 0x00401000 uses priority 50 / arbitration class 2, with an optional
    // StopAllManagedSounds BEFORE trying the new voice.
    AudioEffectPlanner help;
    std::istringstream help_catalog(
        "voice0.wav 510\n"
        "voice1.wav 511\n");
    assert(help.load_catalog(help_catalog).success);
    const auto base_voice = help.plan_help_voice(510,false,{});
    assert(base_voice.managed_results.size()==1);
    assert(base_voice.managed_results[0].status==1);
    assert(base_voice.operations.size()==3);
    assert(base_voice.operations[0].kind==SoundEffectKind::LoadManagedWav);
    assert(base_voice.operations[0].sound_id==510);
    ManagedSoundObservation observed_voice{};
    observed_voice.playing[0]=true;
    const auto replace_voice = help.plan_help_voice(
        511,true,observed_voice);
    assert(replace_voice.operations.size()==4);
    assert(replace_voice.operations[0].kind==
           SoundEffectKind::StopRewindManagedSlot);
    assert(replace_voice.operations[0].slot==0);
    assert(replace_voice.operations[1].kind==
           SoundEffectKind::LoadManagedWav);
    assert(replace_voice.operations[1].sound_id==511);
    assert(replace_voice.managed_results[0].status==1);
    assert(help.manager().state(0)==btb::sound::SlotState::Stopped);
    // 00402F60 reap keeps an active group running until it really stops,
    // then stops and rewinds it. Source models are wired to planner.reap.
    AudioEffectPlanner reaper;
    std::istringstream reap_catalog("voice.wav 510\n");
    assert(reaper.load_catalog(reap_catalog).success);
    assert(reaper.plan_help_voice(510,false,{}).managed_results[0].status==1);
    ManagedSoundObservation still_playing{};
    still_playing.playing[0]=true;
    assert(reaper.reap(still_playing,false,false).operations.empty());
    ManagedSoundObservation ended{};
    const auto finish=reaper.reap(ended,false,false);
    assert(finish.operations.size()==1);
    assert(finish.operations[0].kind==SoundEffectKind::StopRewindManagedSlot);
    assert(finish.operations[0].slot==0);
    assert(reaper.manager().state(0)==btb::sound::SlotState::Stopped);
}

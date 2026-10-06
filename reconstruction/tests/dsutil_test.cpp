#include "btb/dsutil.hpp"

#include <cassert>
#include <cstddef>

using namespace btb::dsutil;

int main() {
    static_assert(sizeof(RetailCSoundManager32) == 0x04);
    static_assert(sizeof(RetailCSound32) == 0x14);
    static_assert(sizeof(RetailMMCKINFO32) == 0x14);
    static_assert(sizeof(RetailCWaveFile32) == 0x90);

    static_assert(offsetof(RetailCSound32, m_apDSBuffer) == 0x04);
    static_assert(offsetof(RetailCSound32, m_dwDSBufferSize) == 0x08);
    static_assert(offsetof(RetailCSound32, m_pWaveFile) == 0x0C);
    static_assert(offsetof(RetailCSound32, m_dwNumBuffers) == 0x10);

    static_assert(offsetof(RetailCWaveFile32, m_ck) == 0x08);
    static_assert(offsetof(RetailCWaveFile32, m_ckRiff) == 0x1C);
    static_assert(offsetof(RetailCWaveFile32, m_dwSize) == 0x30);
    static_assert(offsetof(RetailCWaveFile32, m_mmioinfoOut) == 0x34);
    static_assert(offsetof(RetailCWaveFile32, m_dwFlags) == 0x7C);
    static_assert(offsetof(RetailCWaveFile32, m_bIsReadingFromMemory) == 0x80);
    static_assert(offsetof(RetailCWaveFile32, m_pbData) == 0x84);
    static_assert(offsetof(RetailCWaveFile32, m_pbDataCur) == 0x88);
    static_assert(offsetof(RetailCWaveFile32, m_ulDataSize) == 0x8C);

    RetailCSound32 sound{};
    sound.m_dwNumBuffers = 3;
    assert(sound.m_dwNumBuffers == 3);

    RetailCWaveFile32 wave{};
    wave.m_dwFlags = 1;
    wave.m_bIsReadingFromMemory = 0;
    assert(wave.m_dwFlags == 1);
    assert(wave.m_bIsReadingFromMemory == 0);

    static_assert(kPrimaryBufferDescriptorSize == 0x24);
    static_assert(kPrimaryBufferFlags == 0x81);
    static_assert(kWaveFormatPcm == 1);

    constexpr auto pcm =
        pcm_format_plan(2, 22050, 16);
    static_assert(pcm.format_tag == 1);
    static_assert(pcm.channels == 2);
    static_assert(pcm.samples_per_sec == 22050);
    static_assert(pcm.block_align == 4);
    static_assert(pcm.avg_bytes_per_sec == 88200);
    static_assert(pcm.bits_per_sample == 16);
    static_assert(pcm.extra_size == 0);

    constexpr auto primary =
        primary_buffer_format_plan(2, 22050, 16);
    static_assert(primary.descriptor_size == 0x24);
    static_assert(primary.descriptor_flags == 0x81);
    static_assert(primary.create_primary_buffer);
    static_assert(primary.set_primary_format);
    static_assert(primary.release_primary_buffer);
    static_assert(primary.format.block_align == 4);

    constexpr auto initialize =
        sound_manager_initialize_plan(2, 22050, 16);
    static_assert(initialize.release_existing_direct_sound);
    static_assert(initialize.call_direct_sound_create8);
    static_assert(initialize.set_cooperative_level);
    static_assert(initialize.channels == 2);
    static_assert(initialize.sample_rate == 22050);
    static_assert(initialize.bits_per_sample == 16);
    static_assert(initialize.call_primary_format_setup);
    static_assert(initialize.ignores_primary_format_hresult);

    constexpr auto invalid_restore =
        restore_buffer_step(false, 0, false);
    static_assert(
        invalid_restore.outcome == RestoreBufferOutcome::Error);
    static_assert(invalid_restore.return_value == kCoENotInitialized);
    static_assert(!invalid_restore.query_status);

    constexpr auto status_error =
        restore_buffer_step(true, kEFail, false);
    static_assert(
        status_error.outcome == RestoreBufferOutcome::Error);
    static_assert(status_error.return_value == kEFail);
    static_assert(status_error.query_status);

    // Exact retail oddity: not-lost returns 1, while successful restoration
    // returns 0 and sets the caller's restored flag.
    constexpr auto not_lost =
        restore_buffer_step(true, 0, false);
    static_assert(
        not_lost.outcome == RestoreBufferOutcome::NotLost);
    static_assert(not_lost.return_value == 1);
    static_assert(!not_lost.loop_restore_until_success);
    static_assert(!not_lost.set_restored_flag);

    constexpr auto restored =
        restore_buffer_step(true, 0, true);
    static_assert(
        restored.outcome == RestoreBufferOutcome::Restored);
    static_assert(restored.return_value == 0);
    static_assert(restored.loop_restore_until_success);
    static_assert(restored.sleep_10ms_on_buffer_lost);
    static_assert(restored.set_restored_flag);

    constexpr DuplicateBufferStatus duplicates[] = {
        {true,true},
        {true,false},
        {true,true},
    };
    static_assert(
        select_free_buffer_index(duplicates, 3, 99) == 1);

    constexpr DuplicateBufferStatus all_playing[] = {
        {true,true},
        {true,true},
        {true,true},
    };
    static_assert(
        select_free_buffer_index(all_playing, 3, 5) == 2);

    constexpr DuplicateBufferStatus with_null[] = {
        {false,false},
        {true,true},
        {true,false},
    };
    // Null entries are skipped; the first real nonplaying duplicate wins.
    static_assert(
        select_free_buffer_index(with_null, 3, 0) == 2);
    static_assert(
        select_free_buffer_index(nullptr, 0, 0) == -1);

    static_assert(silence_byte_for_bits(8) == 0x80);
    static_assert(silence_byte_for_bits(16) == 0x00);

    constexpr auto fill_once =
        fill_buffer_plan(16, false);
    static_assert(fill_once.restore_before_lock);
    static_assert(fill_once.lock_entire_buffer);
    static_assert(fill_once.reset_wave_before_read);
    static_assert(!fill_once.repeat_wave_to_fill_tail);
    static_assert(fill_once.silence_byte == 0);
    static_assert(fill_once.unlock_after_fill);

    constexpr auto fill_repeat =
        fill_buffer_plan(8, true);
    static_assert(fill_repeat.repeat_wave_to_fill_tail);
    static_assert(fill_repeat.silence_byte == 0x80);

    constexpr auto play =
        play_buffer_plan(50, 1);
    static_assert(play.require_buffer_array);
    static_assert(play.select_free_buffer);
    static_assert(play.restore_selected_buffer);
    static_assert(play.refill_when_restored);
    static_assert(play.rewind_group_when_restored);
    static_assert(play.reserved_play_arg == 0);
    static_assert(play.priority == 50);
    static_assert(play.flags == 1);
    static_assert(play.set_shared_game_volume_after_play);

    constexpr std::uint32_t stopped_status[] = {0,0,0};
    constexpr std::uint32_t playing_status[] = {0,1,0};
    static_assert(
        !any_buffer_playing_from_status_bits(stopped_status, 3));
    static_assert(
        any_buffer_playing_from_status_bits(playing_status, 3));
    static_assert(!any_buffer_playing_from_status_bits(nullptr, 0));

    static_assert(kStopGroupPlan.requires_buffer_array);
    static_assert(kStopGroupPlan.iterate_all_buffers);
    static_assert(kStopGroupPlan.or_hresult_results);
    static_assert(kRewindGroupPlan.requires_buffer_array);
}

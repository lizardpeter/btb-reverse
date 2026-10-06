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

    static_assert(kFourCcRiff == 0x46464952U);
    static_assert(kFourCcWave == 0x45564157U);
    static_assert(kFourCcFmt == 0x20746D66U);
    static_assert(kFourCcData == 0x61746164U);
    static_assert(kFourCcFact == 0x74636166U);
    static_assert(kMmioReadOpenFlags == 0x00010000U);
    static_assert(kMmioWriteOpenFlags == 0x00011002U);

    constexpr auto pcm_fmt = wave_format_read_plan(
        kFourCcRiff, kFourCcWave, 16, 1);
    static_assert(pcm_fmt.valid_riff);
    static_assert(pcm_fmt.valid_wave);
    static_assert(pcm_fmt.valid_fmt_chunk);
    static_assert(pcm_fmt.pcm);
    static_assert(pcm_fmt.allocation_bytes == 18);
    static_assert(!pcm_fmt.read_extra_size_word);
    static_assert(pcm_fmt.extra_size == 0);

    constexpr auto compressed_fmt = wave_format_read_plan(
        kFourCcRiff, kFourCcWave, 18, 0x55, 12);
    static_assert(!compressed_fmt.pcm);
    static_assert(compressed_fmt.read_extra_size_word);
    static_assert(compressed_fmt.extra_size == 12);
    static_assert(compressed_fmt.allocation_bytes == 30);

    constexpr auto bad_riff = wave_format_read_plan(
        0, kFourCcWave, 16, 1);
    static_assert(!bad_riff.valid_riff);
    static_assert(bad_riff.allocation_bytes == 0);

    constexpr auto short_fmt = wave_format_read_plan(
        kFourCcRiff, kFourCcWave, 15, 1);
    static_assert(!short_fmt.valid_fmt_chunk);
    static_assert(short_fmt.allocation_bytes == 0);

    constexpr auto open_read =
        wave_open_plan(WaveOpenMode::Read);
    static_assert(open_read.require_filename);
    static_assert(open_read.mmio_open_flags == 0x10000);
    static_assert(open_read.free_existing_owned_format);
    static_assert(open_read.read_riff_format);
    static_assert(!open_read.write_riff_format);
    static_assert(open_read.reset_after_open);
    static_assert(open_read.cache_data_chunk_size_after_reset);

    constexpr auto open_write =
        wave_open_plan(WaveOpenMode::Write);
    static_assert(open_write.mmio_open_flags == 0x11002);
    static_assert(!open_write.free_existing_owned_format);
    static_assert(!open_write.read_riff_format);
    static_assert(open_write.write_riff_format);
    static_assert(open_write.reset_after_open);
    static_assert(!open_write.cache_data_chunk_size_after_reset);

    constexpr auto memory_reset =
        wave_reset_plan(true, WaveOpenMode::Read);
    static_assert(memory_reset.reset_memory_cursor);
    static_assert(!memory_reset.require_mmio_handle);

    constexpr auto file_read_reset =
        wave_reset_plan(false, WaveOpenMode::Read);
    static_assert(file_read_reset.require_mmio_handle);
    static_assert(file_read_reset.seek_to_riff_data_area);
    static_assert(file_read_reset.descend_data_chunk);
    static_assert(!file_read_reset.create_data_chunk);

    constexpr auto file_write_reset =
        wave_reset_plan(false, WaveOpenMode::Write);
    static_assert(file_write_reset.require_mmio_handle);
    static_assert(file_write_reset.create_data_chunk);
    static_assert(file_write_reset.get_mmio_info);
    static_assert(file_write_reset.zero_remaining_chunk_bytes);

    constexpr auto memory_read =
        memory_wave_read_step(1000, 900, 200, true);
    static_assert(memory_read.initialized);
    static_assert(memory_read.bytes_to_copy == 100);
    static_assert(memory_read.next_cursor_offset == 1000);

    constexpr auto memory_read2 =
        memory_wave_read_step(1000, 100, 200, true);
    static_assert(memory_read2.bytes_to_copy == 200);
    static_assert(memory_read2.next_cursor_offset == 300);

    constexpr auto memory_bad =
        memory_wave_read_step(1000, 100, 200, false);
    static_assert(!memory_bad.initialized);

    static_assert(kFileWaveReadPlan.require_mmio_handle);
    static_assert(kFileWaveReadPlan.require_destination);
    static_assert(kFileWaveReadPlan.require_bytes_read_output);
    static_assert(kFileWaveReadPlan.zero_bytes_read_first);
    static_assert(kFileWaveReadPlan.get_mmio_info);
    static_assert(kFileWaveReadPlan.clamp_to_remaining_data_chunk);
    static_assert(kFileWaveReadPlan.advance_mmio_when_buffer_exhausted);
    static_assert(kFileWaveReadPlan.set_mmio_info_after_copy);

    constexpr auto close_read =
        wave_close_plan(false, WaveOpenMode::Read, true);
    static_assert(close_read.close_mmio);
    static_assert(!close_read.ascend_data_chunk_before_close);
    static_assert(!close_read.patch_riff_size_for_write);
    static_assert(close_read.clear_mmio_handle);

    constexpr auto close_write =
        wave_close_plan(false, WaveOpenMode::Write, true);
    static_assert(close_write.close_mmio);
    static_assert(close_write.ascend_data_chunk_before_close);
    static_assert(close_write.patch_riff_size_for_write);

    constexpr auto close_memory =
        wave_close_plan(true, WaveOpenMode::Read, true);
    static_assert(!close_memory.close_mmio);
}

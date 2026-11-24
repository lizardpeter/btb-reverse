#pragma once

#include <array>
#include <cstddef>
#include <cstdint>

namespace btb::dsutil {

// The executable contains a lightly customized DirectX SDK DSUtil family.
// Pointer-shaped Win32/COM members remain uint32_t so these layouts are exact
// regardless of the host architecture used to build the reconstruction.

struct RetailCSoundManager32 {
    std::uint32_t m_pDS{}; // IDirectSound8*
};
static_assert(sizeof(RetailCSoundManager32) == 0x04);

struct RetailCSound32 {
    std::uint32_t vtable_ptr32{};   // +0x00
    std::uint32_t m_apDSBuffer{};   // +0x04 IDirectSoundBuffer**
    std::uint32_t m_dwDSBufferSize{}; // +0x08
    std::uint32_t m_pWaveFile{};    // +0x0C CWaveFile*
    std::uint32_t m_dwNumBuffers{}; // +0x10
};

static_assert(offsetof(RetailCSound32, m_apDSBuffer) == 0x04);
static_assert(offsetof(RetailCSound32, m_dwDSBufferSize) == 0x08);
static_assert(offsetof(RetailCSound32, m_pWaveFile) == 0x0C);
static_assert(offsetof(RetailCSound32, m_dwNumBuffers) == 0x10);
static_assert(sizeof(RetailCSound32) == 0x14);

struct RetailMMCKINFO32 {
    std::uint32_t ckid{};
    std::uint32_t cksize{};
    std::uint32_t fccType{};
    std::uint32_t dwDataOffset{};
    std::uint32_t dwFlags{};
};
static_assert(sizeof(RetailMMCKINFO32) == 0x14);

struct RetailCWaveFile32 {
    std::uint32_t m_pwfx{};          // +0x00 WAVEFORMATEX*
    std::uint32_t m_hmmio{};         // +0x04 HMMIO
    RetailMMCKINFO32 m_ck{};         // +0x08
    RetailMMCKINFO32 m_ckRiff{};     // +0x1C
    std::uint32_t m_dwSize{};        // +0x30
    std::array<std::byte, 0x48> m_mmioinfoOut{}; // +0x34 MMIOINFO
    std::uint32_t m_dwFlags{};       // +0x7C
    std::int32_t m_bIsReadingFromMemory{}; // +0x80 BOOL
    std::uint32_t m_pbData{};        // +0x84 BYTE*
    std::uint32_t m_pbDataCur{};     // +0x88 BYTE*
    std::uint32_t m_ulDataSize{};    // +0x8C ULONG
};

static_assert(offsetof(RetailCWaveFile32, m_pwfx) == 0x00);
static_assert(offsetof(RetailCWaveFile32, m_hmmio) == 0x04);
static_assert(offsetof(RetailCWaveFile32, m_ck) == 0x08);
static_assert(offsetof(RetailCWaveFile32, m_ckRiff) == 0x1C);
static_assert(offsetof(RetailCWaveFile32, m_dwSize) == 0x30);
static_assert(offsetof(RetailCWaveFile32, m_mmioinfoOut) == 0x34);
static_assert(offsetof(RetailCWaveFile32, m_dwFlags) == 0x7C);
static_assert(offsetof(RetailCWaveFile32, m_bIsReadingFromMemory) == 0x80);
static_assert(offsetof(RetailCWaveFile32, m_pbData) == 0x84);
static_assert(offsetof(RetailCWaveFile32, m_pbDataCur) == 0x88);
static_assert(offsetof(RetailCWaveFile32, m_ulDataSize) == 0x8C);
static_assert(sizeof(RetailCWaveFile32) == 0x90);

inline constexpr std::int32_t kCoENotInitialized =
    static_cast<std::int32_t>(0x800401F0U);
inline constexpr std::int32_t kEFail =
    static_cast<std::int32_t>(0x80004005U);
inline constexpr std::int32_t kEOutOfMemory =
    static_cast<std::int32_t>(0x8007000EU);
inline constexpr std::int32_t kDsErrBufferLost =
    static_cast<std::int32_t>(0x88780096U);

inline constexpr std::uint32_t kPrimaryBufferDescriptorSize = 0x24;
inline constexpr std::uint32_t kPrimaryBufferFlags = 0x81;
inline constexpr std::uint16_t kWaveFormatPcm = 1;

struct PcmFormatPlan {
    std::uint16_t format_tag{kWaveFormatPcm};
    std::uint16_t channels{};
    std::uint32_t samples_per_sec{};
    std::uint32_t avg_bytes_per_sec{};
    std::uint16_t block_align{};
    std::uint16_t bits_per_sample{};
    std::uint16_t extra_size{};
};

[[nodiscard]] constexpr PcmFormatPlan pcm_format_plan(
    std::uint16_t channels,
    std::uint32_t sample_rate,
    std::uint16_t bits_per_sample) noexcept {

    const auto block_align = static_cast<std::uint16_t>(
        channels * static_cast<std::uint16_t>(bits_per_sample / 8U));

    return {
        kWaveFormatPcm,
        channels,
        sample_rate,
        static_cast<std::uint32_t>(block_align) * sample_rate,
        block_align,
        bits_per_sample,
        0,
    };
}

struct PrimaryBufferFormatPlan {
    std::uint32_t descriptor_size{kPrimaryBufferDescriptorSize};
    std::uint32_t descriptor_flags{kPrimaryBufferFlags};
    bool create_primary_buffer{true};
    PcmFormatPlan format{};
    bool set_primary_format{true};
    bool release_primary_buffer{true};
};

[[nodiscard]] constexpr PrimaryBufferFormatPlan primary_buffer_format_plan(
    std::uint16_t channels,
    std::uint32_t sample_rate,
    std::uint16_t bits_per_sample) noexcept {
    return {
        kPrimaryBufferDescriptorSize,
        kPrimaryBufferFlags,
        true,
        pcm_format_plan(channels, sample_rate, bits_per_sample),
        true,
        true,
    };
}

struct SoundManagerInitializePlan {
    bool release_existing_direct_sound{true};
    bool call_direct_sound_create8{true};
    bool set_cooperative_level{true};
    std::uint16_t channels{};
    std::uint32_t sample_rate{};
    std::uint16_t bits_per_sample{};
    bool call_primary_format_setup{true};
    // Retail calls SetPrimarySoundBufferFormat but returns S_OK regardless of
    // that helper's HRESULT once creation and cooperative level succeeded.
    bool ignores_primary_format_hresult{true};
};

[[nodiscard]] constexpr SoundManagerInitializePlan
sound_manager_initialize_plan(
    std::uint16_t channels,
    std::uint32_t sample_rate,
    std::uint16_t bits_per_sample) noexcept {
    return {
        true,
        true,
        true,
        channels,
        sample_rate,
        bits_per_sample,
        true,
        true,
    };
}

enum class RestoreBufferOutcome {
    Error,
    NotLost,
    Restored,
};

struct RestoreBufferStep {
    RestoreBufferOutcome outcome{RestoreBufferOutcome::Error};
    std::int32_t return_value{};
    bool query_status{};
    bool loop_restore_until_success{};
    bool sleep_10ms_on_buffer_lost{};
    bool set_restored_flag{};
};

[[nodiscard]] constexpr RestoreBufferStep restore_buffer_step(
    bool valid_buffer,
    std::int32_t get_status_hresult,
    bool status_buffer_lost) noexcept {

    if (!valid_buffer) {
        return {
            RestoreBufferOutcome::Error,
            kCoENotInitialized,
            false,false,false,false,
        };
    }

    if (get_status_hresult < 0) {
        return {
            RestoreBufferOutcome::Error,
            get_status_hresult,
            true,false,false,false,
        };
    }

    if (!status_buffer_lost) {
        return {
            RestoreBufferOutcome::NotLost,
            1,
            true,false,false,false,
        };
    }

    return {
        RestoreBufferOutcome::Restored,
        0,
        true,true,true,true,
    };
}

struct DuplicateBufferStatus {
    bool pointer_nonnull{};
    bool playing{};
};

[[nodiscard]] constexpr std::int32_t select_free_buffer_index(
    const DuplicateBufferStatus* status,
    std::size_t count,
    std::uint32_t random_value) noexcept {

    if (status == nullptr || count == 0) {
        return -1;
    }

    for (std::size_t i = 0; i < count; ++i) {
        if (status[i].pointer_nonnull && !status[i].playing) {
            return static_cast<std::int32_t>(i);
        }
    }

    return static_cast<std::int32_t>(
        random_value % static_cast<std::uint32_t>(count));
}

[[nodiscard]] constexpr std::uint8_t silence_byte_for_bits(
    std::uint16_t bits_per_sample) noexcept {
    return bits_per_sample == 8 ? 0x80U : 0x00U;
}

struct FillBufferPlan {
    bool restore_before_lock{true};
    bool lock_entire_buffer{true};
    bool reset_wave_before_read{true};
    bool repeat_wave_to_fill_tail{};
    std::uint8_t silence_byte{};
    bool unlock_after_fill{true};
};

[[nodiscard]] constexpr FillBufferPlan fill_buffer_plan(
    std::uint16_t bits_per_sample,
    bool repeat_wave_to_fill_tail) noexcept {
    return {
        true,
        true,
        true,
        repeat_wave_to_fill_tail,
        silence_byte_for_bits(bits_per_sample),
        true,
    };
}

struct PlayBufferPlan {
    bool require_buffer_array{true};
    bool select_free_buffer{true};
    bool restore_selected_buffer{true};
    bool refill_when_restored{true};
    bool rewind_group_when_restored{true};
    std::uint32_t reserved_play_arg{};
    std::uint32_t priority{};
    std::uint32_t flags{};
    bool set_shared_game_volume_after_play{true};
};

[[nodiscard]] constexpr PlayBufferPlan play_buffer_plan(
    std::uint32_t priority,
    std::uint32_t flags) noexcept {
    return {
        true,true,true,true,true,
        0,
        priority,
        flags,
        true,
    };
}

struct GroupOperationPlan {
    bool requires_buffer_array{};
    bool iterate_all_buffers{};
    bool or_hresult_results{};
};

inline constexpr GroupOperationPlan kSetVolumeGroupPlan{
    false,true,true};
inline constexpr GroupOperationPlan kStopGroupPlan{
    true,true,true};
inline constexpr GroupOperationPlan kRewindGroupPlan{
    true,true,true};

[[nodiscard]] constexpr bool any_buffer_playing_from_status_bits(
    const std::uint32_t* status_bits,
    std::size_t count) noexcept {
    if (status_bits == nullptr) {
        return false;
    }

    std::uint32_t playing = 0;
    for (std::size_t i = 0; i < count; ++i) {
        playing |= status_bits[i] & 1U;
    }
    return playing != 0;
}

inline constexpr std::uint32_t kFourCcRiff = 0x46464952U;
inline constexpr std::uint32_t kFourCcWave = 0x45564157U;
inline constexpr std::uint32_t kFourCcFmt  = 0x20746D66U;
inline constexpr std::uint32_t kFourCcData = 0x61746164U;
inline constexpr std::uint32_t kFourCcFact = 0x74636166U;

inline constexpr std::uint32_t kMmioReadOpenFlags = 0x00010000U;
inline constexpr std::uint32_t kMmioWriteOpenFlags = 0x00011002U;
inline constexpr std::uint32_t kWaveMemoryMmioFlag = 0x204D454DU; // "MEM "

enum class WaveOpenMode : std::uint32_t {
    Read = 1,
    Write = 2,
};

struct WaveConstructorState {
    std::uint32_t format_ptr32{};
    std::uint32_t mmio_handle32{};
    std::uint32_t data_size{};
    std::int32_t reading_from_memory{};
};

inline constexpr WaveConstructorState kWaveConstructorState{};

struct WaveFormatReadPlan {
    bool valid_riff{};
    bool valid_wave{};
    bool valid_fmt_chunk{};
    bool pcm{};
    std::uint32_t allocation_bytes{};
    bool read_extra_size_word{};
    std::uint16_t extra_size{};
};

[[nodiscard]] constexpr WaveFormatReadPlan wave_format_read_plan(
    std::uint32_t riff_id,
    std::uint32_t riff_type,
    std::uint32_t fmt_chunk_size,
    std::uint16_t format_tag,
    std::uint16_t extra_size = 0) noexcept {

    const bool valid_riff = riff_id == kFourCcRiff;
    const bool valid_wave = riff_type == kFourCcWave;
    const bool valid_fmt = fmt_chunk_size >= 16;

    if (!valid_riff || !valid_wave || !valid_fmt) {
        return {valid_riff,valid_wave,valid_fmt,false,0,false,0};
    }

    if (format_tag == kWaveFormatPcm) {
        return {
            true,true,true,true,
            18,
            false,
            0,
        };
    }

    return {
        true,true,true,false,
        static_cast<std::uint32_t>(18U + extra_size),
        true,
        extra_size,
    };
}

struct WaveOpenPlan {
    WaveOpenMode mode{WaveOpenMode::Read};
    bool clear_memory_read_flag{true};
    bool require_filename{};
    std::uint32_t mmio_open_flags{};
    bool free_existing_owned_format{};
    bool read_riff_format{};
    bool write_riff_format{};
    bool reset_after_open{true};
    bool cache_data_chunk_size_after_reset{};
};

[[nodiscard]] constexpr WaveOpenPlan wave_open_plan(
    WaveOpenMode mode) noexcept {

    if (mode == WaveOpenMode::Read) {
        return {
            mode,
            true,
            true,
            kMmioReadOpenFlags,
            true,
            true,
            false,
            true,
            true,
        };
    }

    return {
        mode,
        true,
        true,
        kMmioWriteOpenFlags,
        false,
        false,
        true,
        true,
        false,
    };
}

struct WaveResetPlan {
    bool reset_memory_cursor{};
    bool require_mmio_handle{};
    bool seek_to_riff_data_area{};
    bool descend_data_chunk{};
    bool create_data_chunk{};
    bool get_mmio_info{};
    bool zero_remaining_chunk_bytes{};
};

[[nodiscard]] constexpr WaveResetPlan wave_reset_plan(
    bool reading_from_memory,
    WaveOpenMode mode) noexcept {

    if (reading_from_memory) {
        return {true,false,false,false,false,false,false};
    }

    if (mode == WaveOpenMode::Read) {
        return {false,true,true,true,false,false,false};
    }

    return {false,true,false,false,true,true,true};
}

struct MemoryWaveReadStep {
    bool initialized{};
    std::uint32_t bytes_to_copy{};
    std::uint32_t next_cursor_offset{};
};

[[nodiscard]] constexpr MemoryWaveReadStep memory_wave_read_step(
    std::uint32_t data_size,
    std::uint32_t cursor_offset,
    std::uint32_t requested_bytes,
    bool has_current_pointer) noexcept {

    if (!has_current_pointer || cursor_offset > data_size) {
        return {};
    }

    const auto remaining = data_size - cursor_offset;
    const auto count = requested_bytes < remaining
        ? requested_bytes
        : remaining;

    return {
        true,
        count,
        cursor_offset + count,
    };
}

struct FileWaveReadPlan {
    bool require_mmio_handle{true};
    bool require_destination{true};
    bool require_bytes_read_output{true};
    bool zero_bytes_read_first{true};
    bool get_mmio_info{true};
    bool clamp_to_remaining_data_chunk{true};
    bool advance_mmio_when_buffer_exhausted{true};
    bool set_mmio_info_after_copy{true};
};

inline constexpr FileWaveReadPlan kFileWaveReadPlan{};

struct WaveClosePlan {
    bool close_mmio{};
    bool ascend_data_chunk_before_close{};
    bool patch_riff_size_for_write{};
    bool clear_mmio_handle{true};
};

[[nodiscard]] constexpr WaveClosePlan wave_close_plan(
    bool reading_from_memory,
    WaveOpenMode mode,
    bool has_mmio_handle) noexcept {

    if (reading_from_memory || !has_mmio_handle) {
        return {};
    }

    if (mode == WaveOpenMode::Write) {
        return {true,true,true,true};
    }

    return {true,false,false,true};
}

// This retail build is a smaller/customized DSUtil variant:
// - CSound has no m_dwCreationFlags member.
// - CSound::Play takes only priority + flags; volume is applied from the
//   game's global volume immediately after starting the DirectSound buffer.
// - CSoundManager::Initialize additionally receives channels/frequency/bits
//   and calls SetPrimaryBufferFormat internally.

} // namespace btb::dsutil

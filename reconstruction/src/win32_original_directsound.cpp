#include "btb/win32_original_directsound.hpp"

#if defined(_WIN32)
#include <algorithm>
#include <cstdint>
#include <cstring>
#include <fstream>
#include <limits>
#include <string>
#include <utility>

namespace btb::full_game {
namespace {

std::uint32_t le32(const unsigned char* p) noexcept {
    return static_cast<std::uint32_t>(p[0]) |
           (static_cast<std::uint32_t>(p[1])<<8) |
           (static_cast<std::uint32_t>(p[2])<<16) |
           (static_cast<std::uint32_t>(p[3])<<24);
}
std::string hr_error(const char* context,HRESULT hr) {
    return std::string(context)+" failed (HRESULT "+
           std::to_string(static_cast<std::uint32_t>(hr))+")";
}

struct OriginalWave {
    std::vector<std::uint32_t> aligned_format_words{};
    std::vector<unsigned char> samples{};
};

bool read_original_wave(
    const std::filesystem::path& path,
    OriginalWave& out,std::string& error) {

    std::ifstream file(path,std::ios::binary);
    if (!file) {
        error="cannot open original WAV file: "+path.generic_string();
        return false;
    }
    unsigned char header[12]{};
    if (!file.read(reinterpret_cast<char*>(header),12) ||
        std::memcmp(header,"RIFF",4)!=0 ||
        std::memcmp(header+8,"WAVE",4)!=0) {
        error="original WAV missing RIFF/WAVE header";
        return false;
    }

    // Keep the original WAVEFORMATEX and extra codec bytes intact for
    // DirectSound. Never reinterpret compressed original audio as PCM.
    std::vector<unsigned char> format{};
    std::vector<unsigned char> samples{};
    constexpr std::uint32_t kMaxWaveBytes=128U*1024U*1024U;
    constexpr std::uint32_t kMaxFormatBytes=4096;
    for (int chunks=0;chunks<4096 && file;++chunks) {
        unsigned char chunk[8]{};
        if (!file.read(reinterpret_cast<char*>(chunk),8)) {
            if (file.eof()) break;
            error="damaged original WAV chunk header";
            return false;
        }
        const auto count=le32(chunk+4);
        if (count>kMaxWaveBytes) {
            error="original WAV chunk exceeds supported DirectSound size";
            return false;
        }
        if (std::memcmp(chunk,"fmt ",4)==0) {
            if (count<16 || count>kMaxFormatBytes || !format.empty()) {
                error="invalid or duplicate original WAV fmt chunk";
                return false;
            }
            format.resize(count);
            if (!file.read(reinterpret_cast<char*>(format.data()),count)) {
                error="truncated original WAV fmt data";
                return false;
            }
        } else if (std::memcmp(chunk,"data",4)==0) {
            if (count==0 || !samples.empty()) {
                error="empty or duplicate original WAV sample chunk";
                return false;
            }
            samples.resize(count);
            if (!file.read(reinterpret_cast<char*>(samples.data()),count)) {
                error="truncated original WAV sample data";
                return false;
            }
        } else {
            file.seekg(count,std::ios::cur);
            if (!file) {
                error="truncated optional original WAV chunk";
                return false;
            }
        }
        if ((count&1U)!=0U) {
            file.seekg(1,std::ios::cur); // RIFF chunks are word padded
        }
    }

    if (format.empty() || samples.empty()) {
        error="original WAV is missing a format or sample-data chunk";
        return false;
    }
    if (format.size()==17 ||
        (format.size()>=18 &&
         (static_cast<std::size_t>(format[16]) |
          (static_cast<std::size_t>(format[17])<<8)) >
             format.size()-18)) {
        error="original WAV fmt extra-byte count exceeds its RIFF chunk";
        return false;
    }

    const auto format_bytes=std::max<std::size_t>(
        format.size(),sizeof(WAVEFORMATEX));
    out.aligned_format_words.assign((format_bytes+3)/4,0);
    std::memcpy(
        out.aligned_format_words.data(),format.data(),format.size());
    // A 16-byte standard PCM fmt chunk does not contain cbSize. We padded
    // the aligned WAVEFORMATEX storage with zero so cbSize remains 0.
    out.samples=std::move(samples);
    error.clear();
    return true;
}

} // namespace

Win32OriginalDirectSound::~Win32OriginalDirectSound() {
    shutdown();
}

void Win32OriginalDirectSound::release_buffer(
    IDirectSoundBuffer*& buffer) noexcept {
    if (buffer) {
        buffer->Stop();
        buffer->Release();
        buffer=nullptr;
    }
}

void Win32OriginalDirectSound::shutdown() noexcept {
    for (auto& slot : slots_) {
        release_buffer(slot);
    }
    release_buffer(backing_);
    for (auto*& sample : samples_) {
        release_buffer(sample);
    }
    samples_.clear();
    if (sound_) {
        sound_->Release();
        sound_=nullptr;
    }
}

bool Win32OriginalDirectSound::initialize(
    HWND hwnd,std::string& error) {

    shutdown();
    if (!hwnd || !IsWindow(hwnd)) {
        error="DirectSound8 needs a real original-game HWND";
        return false;
    }
    auto hr=DirectSoundCreate8(nullptr,&sound_,nullptr);
    if (FAILED(hr)) {
        error=hr_error("DirectSoundCreate8",hr);
        return false;
    }
    hr=sound_->SetCooperativeLevel(hwnd,DSSCL_PRIORITY);
    if (FAILED(hr)) {
        error=hr_error("DirectSound cooperative level",hr);
        shutdown();
        return false;
    }
    error.clear();
    return true;
}

bool Win32OriginalDirectSound::create_wave_buffer(
    const std::filesystem::path& source,
    IDirectSoundBuffer*& out,std::string& error) {

    out=nullptr;
    if (!sound_) {
        error="DirectSound8 device is not initialized";
        return false;
    }
    OriginalWave wave;
    if (!read_original_wave(source,wave,error)) {
        return false;
    }
    DSBUFFERDESC desc{};
    desc.dwSize=sizeof(desc);
    desc.dwFlags=DSBCAPS_CTRLVOLUME|DSBCAPS_GETCURRENTPOSITION2;
    desc.dwBufferBytes=static_cast<DWORD>(wave.samples.size());
    desc.lpwfxFormat=reinterpret_cast<WAVEFORMATEX*>(
        wave.aligned_format_words.data());

    auto hr=sound_->CreateSoundBuffer(&desc,&out,nullptr);
    if (FAILED(hr)) {
        error=hr_error("CreateSoundBuffer original WAV",hr);
        return false;
    }

    LPVOID first=nullptr,second=nullptr;
    DWORD first_bytes=0,second_bytes=0;
    hr=out->Lock(0,desc.dwBufferBytes,&first,&first_bytes,
                 &second,&second_bytes,0);
    if (FAILED(hr)) {
        error=hr_error("Lock original WAV sound buffer",hr);
        release_buffer(out);
        return false;
    }
    std::memcpy(first,wave.samples.data(),first_bytes);
    if (second_bytes && second) {
        std::memcpy(second,wave.samples.data()+first_bytes,second_bytes);
    }
    hr=out->Unlock(first,first_bytes,second,second_bytes);
    if (FAILED(hr)) {
        error=hr_error("Unlock original WAV sound buffer",hr);
        release_buffer(out);
        return false;
    }
    error.clear();
    return true;
}

bool Win32OriginalDirectSound::play_once(
    IDirectSoundBuffer* buffer,bool loop,
    std::string& error) {

    if (!buffer) {
        error="the source WAV was not loaded into a DirectSound buffer";
        return false;
    }
    const auto hr=buffer->Play(0,0,loop ? DSBPLAY_LOOPING : 0);
    if (FAILED(hr)) {
        error=hr_error("Play original DirectSound WAV",hr);
        return false;
    }
    error.clear();
    return true;
}

void Win32OriginalDirectSound::reap_finished_samples() noexcept {
    for (auto it=samples_.begin();it!=samples_.end();) {
        DWORD status=0;
        if (FAILED((*it)->GetStatus(&status)) ||
            (status&DSBSTATUS_PLAYING)==0) {
            release_buffer(*it);
            it=samples_.erase(it);
        } else {
            ++it;
        }
    }
}

bool Win32OriginalDirectSound::observe(
    ManagedSoundObservation& managed,
    bool& any_managed_voice_playing,
    std::string& error) {

    if (!sound_) {
        error="DirectSound8 observation requested before initialization";
        return false;
    }
    reap_finished_samples();
    managed={};
    any_managed_voice_playing=false;
    for (std::size_t i=0;i<slots_.size();++i) {
        auto* const buffer=slots_[i];
        if (!buffer) {
            continue;
        }
        DWORD status=0;
        const auto hr=buffer->GetStatus(&status);
        if (FAILED(hr)) {
            error=hr_error("GetStatus original managed sound",hr);
            return false;
        }
        managed.playing[i]=(status&DSBSTATUS_PLAYING)!=0;
        any_managed_voice_playing|=
            managed.playing[i];
    }
    error.clear();
    return true;
}

bool Win32OriginalDirectSound::apply(
    const ResolvedSoundEffect& effect,std::string& error) {

    if (!sound_) {
        error="DirectSound8 must be initialized before audio submission";
        return false;
    }

    const auto kind=effect.original.kind;
    const auto slot=effect.original.slot;
    const bool managed=
        slot>=0 && slot<static_cast<int>(slots_.size());
    const bool needs_slot =
        kind==SoundEffectKind::StopRewindManagedSlot ||
        kind==SoundEffectKind::ReleaseManagedSlot ||
        kind==SoundEffectKind::LoadManagedWav ||
        kind==SoundEffectKind::PlayManagedSlot;
    if (needs_slot && !managed) {
        error="native managed sound operation specifies invalid slot";
        return false;
    }
    if (kind==SoundEffectKind::ReleaseManagedSlot) {
        release_buffer(slots_[static_cast<std::size_t>(slot)]);
        error.clear();
        return true;
    }
    if (kind==SoundEffectKind::StopRewindManagedSlot) {
        auto* buffer=slots_[static_cast<std::size_t>(slot)];
        if (!buffer) {
            error="cannot stop an unallocated managed sound buffer";
            return false;
        }
        auto hr=buffer->Stop();
        if (SUCCEEDED(hr)) hr=buffer->SetCurrentPosition(0);
        if (FAILED(hr)) {
            error=hr_error("Stop and rewind managed WAV",hr);
            return false;
        }
        error.clear();
        return true;
    }
    if (kind==SoundEffectKind::PlayManagedSlot) {
        return play_once(slots_[static_cast<std::size_t>(slot)],false,error);
    }
    if (kind==SoundEffectKind::StopBackingWav) {
        release_buffer(backing_);
        error.clear();
        return true;
    }

    // These three operations require actual resolved WAV bytes; never
    // replace missing game assets with an artificial beep or silence.
    if (!effect.source || !effect.source->found()) {
        error="original WAV file missing for DirectSound load/play";
        return false;
    }
    if (kind==SoundEffectKind::LoadManagedWav) {
        auto& buffer=slots_[static_cast<std::size_t>(slot)];
        release_buffer(buffer);
        return create_wave_buffer(
            effect.source->absolute_path,buffer,error);
    }
    if (kind==SoundEffectKind::StartBackingWav) {
        release_buffer(backing_);
        if (!create_wave_buffer(effect.source->absolute_path,backing_,error)) {
            return false;
        }
        return play_once(backing_,true,error);
    }
    if (kind==SoundEffectKind::PlayDirectSample) {
        IDirectSoundBuffer* sample=nullptr;
        if (!create_wave_buffer(effect.source->absolute_path,sample,error)) {
            return false;
        }
        if (!play_once(sample,false,error)) {
            release_buffer(sample);
            return false;
        }
        samples_.push_back(sample);
        return true;
    }
    error="unknown recovered DirectSound effect kind";
    return false;
}

} // namespace btb::full_game
#endif

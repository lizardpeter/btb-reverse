#include "btb/win32_native_bink_pcm_voice.hpp"
#if defined(_WIN32)
#include "btb/bink32_audio_pcm.hpp"
#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <limits>
#include <span>
#include <string>
#include <vector>

namespace btb::full_game {
namespace {
[[nodiscard]] std::string ds_error(
    const char* name,HRESULT hr) {
    return std::string(name)+" failed (HRESULT "+
        std::to_string(static_cast<std::uint32_t>(hr))+")";
}
}

Win32NativeBinkPcmVoice::Win32NativeBinkPcmVoice(
    IDirectSound8* sound) noexcept :device_(sound) {
    if(device_)device_->AddRef();
}
Win32NativeBinkPcmVoice::~Win32NativeBinkPcmVoice() {
    close();
    if(device_) {
        device_->Release();
        device_=nullptr;
    }
}
void Win32NativeBinkPcmVoice::close() noexcept {
    if(buffer_) {
        buffer_->Stop();
        buffer_->Release();
        buffer_=nullptr;
    }
    rate_=bytes_per_frame_=0;
    sample_frames_=0;
    started_=ever_played_=false;
}
bool Win32NativeBinkPcmVoice::load(
    const btb::bink32_recovered::Bink1MoviePcm16& audio,
    std::string& error) {
    close();
    if(!device_) {
        error="original DirectSound8 game device unavailable";
        return false;
    }
    if(!audio.sample_rate || audio.channels<1||audio.channels>2 ||
       audio.samples.empty()||audio.samples.size()%audio.channels) {
        error="Bink native PCM16 rate, channels or frame count invalid";
        return false;
    }
    const auto data=
        btb::bink32_recovered::serialize_bink1_pcm16_le(audio.samples);
    constexpr std::size_t kMaxBuffer=64ULL*1024ULL*1024ULL;
    if(data.size()<8||data.size()>kMaxBuffer ||
       data.size()>std::numeric_limits<DWORD>::max()) {
        error="Bink PCM buffer outside original DirectSound limits";
        return false;
    }
    WAVEFORMATEX format{};
    format.wFormatTag=WAVE_FORMAT_PCM;
    format.nChannels=audio.channels;
    format.nSamplesPerSec=audio.sample_rate;
    format.wBitsPerSample=16;
    format.nBlockAlign=static_cast<WORD>(audio.channels*2);
    format.nAvgBytesPerSec=audio.sample_rate*format.nBlockAlign;
    format.cbSize=0;

    DSBUFFERDESC desc{};
    desc.dwSize=sizeof(desc);
    desc.dwFlags=DSBCAPS_CTRLVOLUME|DSBCAPS_GETCURRENTPOSITION2;
    desc.dwBufferBytes=static_cast<DWORD>(data.size());
    desc.lpwfxFormat=&format;
    HRESULT hr=device_->CreateSoundBuffer(&desc,&buffer_,nullptr);
    if(FAILED(hr)||!buffer_) {
        buffer_=nullptr;
        error=ds_error("DirectSound Bink PCM16 CreateSoundBuffer",hr);
        return false;
    }
    LPVOID first=nullptr,second=nullptr;
    DWORD first_size=0,second_size=0;
    hr=buffer_->Lock(0,desc.dwBufferBytes,&first,&first_size,
                     &second,&second_size,0);
    if(FAILED(hr)||std::size_t(first_size)+second_size!=data.size() ||
       (first_size&&!first)||(second_size&&!second)) {
        error=ds_error("DirectSound Bink PCM16 Lock",hr);
        close();
        return false;
    }
    if(first_size)std::memcpy(first,data.data(),first_size);
    if(second_size)std::memcpy(
        second,data.data()+first_size,second_size);
    hr=buffer_->Unlock(first,first_size,second,second_size);
    if(FAILED(hr)) {
        error=ds_error("DirectSound Bink PCM16 Unlock",hr);
        close();
        return false;
    }
    rate_=audio.sample_rate;
    bytes_per_frame_=format.nBlockAlign;
    sample_frames_=audio.sample_frames();
    error.clear();
    return true;
}

bool Win32NativeBinkPcmVoice::start(std::string& error) {
    if(!buffer_||!sample_frames_) {
        error="native Bink PCM sound buffer not loaded";
        return false;
    }
    const auto hr=buffer_->Play(0,0,0); // one-shot, not endless looping
    if(FAILED(hr)) {
        error=ds_error("DirectSound Bink PCM16 Play",hr);
        return false;
    }
    started_=true;
    error.clear();
    return true;
}
bool Win32NativeBinkPcmVoice::stop(std::string& error) noexcept {
    if(!buffer_) {
        error.clear();
        return true;
    }
    const auto hr=buffer_->Stop();
    if(FAILED(hr)) {
        error=ds_error("DirectSound Bink PCM16 Stop",hr);
        return false;
    }
    started_=false;
    error.clear();
    return true;
}
bool Win32NativeBinkPcmVoice::seek_frame(
    std::uint64_t frame,std::string& error) {
    if(!buffer_||frame>=sample_frames_) {
        error="Bink native PCM seek outside decoded samples";
        return false;
    }
    if(!stop(error))return false;
    const auto offset=frame*bytes_per_frame_;
    if(offset>std::numeric_limits<DWORD>::max()) {
        error="Bink native PCM seek cannot fit DirectSound cursor";
        return false;
    }
    const auto hr=buffer_->SetCurrentPosition(static_cast<DWORD>(offset));
    if(FAILED(hr)) {
        error=ds_error("DirectSound Bink PCM16 Seek",hr);
        return false;
    }
    ever_played_=false;
    error.clear();
    return true;
}
std::optional<Win32NativeBinkPcmVoice::Playhead>
Win32NativeBinkPcmVoice::observe(std::string& error) noexcept {
    if(!buffer_||!bytes_per_frame_) {
        error="native Bink PCM playback was not initialized";
        return std::nullopt;
    }
    DWORD status=0;
    auto hr=buffer_->GetStatus(&status);
    if(FAILED(hr)) {
        error=ds_error("DirectSound Bink PCM16 GetStatus",hr);
        return std::nullopt;
    }
    DWORD play_cursor=0,write_cursor=0;
    hr=buffer_->GetCurrentPosition(&play_cursor,&write_cursor);
    if(FAILED(hr)) {
        error=ds_error("DirectSound Bink PCM16 GetCurrentPosition",hr);
        return std::nullopt;
    }
    const bool playing=(status&DSBSTATUS_PLAYING)!=0;
    if(playing)ever_played_=true;
    const std::uint64_t position=std::min(
        std::uint64_t(play_cursor/bytes_per_frame_),sample_frames_);
    // Do not report completion until DirectSound has been observed
    // actively playing and then has actually reached a stopped state.
    const bool complete=started_ && ever_played_ && !playing;
    error.clear();
    return Playhead{complete?sample_frames_:position,playing,complete};
}
} // namespace btb::full_game
#endif

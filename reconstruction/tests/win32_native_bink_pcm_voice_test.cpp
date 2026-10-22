#include "btb/win32_native_bink_pcm_voice.hpp"
#if defined(_WIN32)
#include <cmath>
#include <cstdint>
#include <iostream>
#include <vector>

namespace {
struct DeviceOwner {
    IDirectSound8* value{};
    ~DeviceOwner() { if(value)value->Release(); }
};
}

int main() {
    DeviceOwner device;
    HRESULT hr=DirectSoundCreate8(nullptr,&device.value,nullptr);
    if(FAILED(hr)||!device.value) {
        std::cout<<"SKIP: DirectSound8 playback device unavailable (HRESULT "
                 <<static_cast<std::uint32_t>(hr)<<")\n";
        return 0;
    }
    hr=device.value->SetCooperativeLevel(GetDesktopWindow(),DSSCL_NORMAL);
    if(FAILED(hr)) {
        std::cout<<"SKIP: DirectSound8 cooperative level unavailable (HRESULT "
                 <<static_cast<std::uint32_t>(hr)<<")\n";
        return 0;
    }

    // Use digital silence so this runtime smoke test never makes audible sound.
    // The cursor still has to advance through a real one-shot DirectSound buffer.
    constexpr std::uint32_t rate=44100;
    constexpr std::uint8_t channels=2;
    constexpr std::uint64_t frames=rate/2;
    btb::bink32_recovered::Bink1MoviePcm16 audio{};
    audio.sample_rate=rate;
    audio.channels=channels;
    audio.track_id=0;
    audio.samples.assign(static_cast<std::size_t>(frames)*channels,0);
    audio.audio_frame_boundaries={0,frames};

    btb::full_game::Win32NativeBinkPcmVoice voice(device.value);
    std::string error;
    if(!voice.load(audio,error)) {
        std::cerr<<"FAIL: native PCM voice load: "<<error<<"\n";
        return 1;
    }
    if(!voice.start(error)) {
        std::cerr<<"FAIL: native PCM voice start: "<<error<<"\n";
        return 1;
    }

    bool saw_playing=false,saw_progress=false,saw_complete=false;
    std::uint64_t last_frame=0;
    const ULONGLONG started_at=GetTickCount64();
    const ULONGLONG deadline=started_at+3000;
    while(GetTickCount64()<deadline) {
        const auto head=voice.observe(error);
        if(!head) {
            std::cerr<<"FAIL: native PCM voice observe: "<<error<<"\n";
            return 1;
        }
        saw_playing=saw_playing||head->playing;
        saw_progress=saw_progress||head->decoded_sample_frame>last_frame;
        last_frame=head->decoded_sample_frame;
        if(head->complete) {
            saw_complete=true;
            break;
        }
        if(!saw_playing && GetTickCount64()-started_at>250) {
            std::cout<<"SKIP: headless DirectSound backend exposes no playing state\n";
            return 0;
        }
        Sleep(5);
    }
    if(!saw_playing) {
        std::cout<<"SKIP: DirectSound playback state unavailable on this host\n";
        return 0;
    }
    if(!saw_progress) {
        std::cerr<<"FAIL: DirectSound play cursor did not advance\n";
        return 1;
    }
    if(!saw_complete||last_frame!=frames) {
        std::cerr<<"FAIL: DirectSound did not report complete at the decoded end; "
                 <<"last frame="<<last_frame<<" expected="<<frames<<"\n";
        return 1;
    }

    std::cout<<"PASS: DirectSound8 one-shot PCM voice advanced and completed "
             <<frames<<" silent stereo sample frames at "<<rate<<" Hz\n";
    return 0;
}
#else
int main() { return 0; }
#endif

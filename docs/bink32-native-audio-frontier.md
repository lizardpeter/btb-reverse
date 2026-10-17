# Original Bink1 native audio — reconstruction and experimental playback

**Status:** The native SJPi Bink1 decoder reconstructs PCM16, but original
retail BIK audio is **not yet certified**. Production gameplay continues
to use the original x86 binkw32.dll for audio-bearing movies. Native
silent-video playback works without that DLL.

## Implemented

- bink32_audio_packets: source track metadata, codec flags, bounds and
  original per-frame length-prefixed audio.
- bink32_audio_spectrum: compressed 29-bit signed DC floats, quantization
  bands and variable-width/RLE signed AC coefficients.
- bink32_audio_inverse: FFT-backed inverse DCT-III and inverse real DFT,
  without an FFmpeg/FFTW runtime dependency.
- bink32_audio_overlap: source 1/16-block overlap history and channel-aware
  blending. Transactional packet rollback protects that history.
- bink32_audio_stream: multiple compressed audio blocks per BIK video frame.
- bink32_audio_pcm: deterministic, bounded PCM16 output, mono/stereo
  interleaving, ties-to-even rounding, clipping and exact packet-byte checks.
- bink32_audio_movie: read-only BIK-to-memory PCM reconstruction, maximum
  64 MiB, with audio sample position indexed by original video frame.
- win32_native_bink_pcm_voice: an actual DirectSound8 sample buffer, with
  start/stop/seek and playback-cursor observation.
- win32_native_silent_bink_provider: developer-only A/V path that uses
  the actual DirectSound playback position as video master clock.
  Full audio mode is not selected unless explicitly requested.

## Validated

The previous Linux reconstruction CI run passed all 192/192 tests. All
eight independent synthetic original-format SJPi Bink audio comparisons
against FFmpeg passed, including loud and near-silent mono/stereo
DCT/RDFT fixtures. Candidate native PCM16 differed from FFmpeg by at most
one 16-bit sample level (seven of eight matched exactly). Peak candidate
float deviation was 3.3e-6 on a louder stereo DCT fixture.

This is **synthetic independent differential parity**, not evidence that
all original 2002 retail movies are correct. The complete in-memory movie
assembler, DirectSound8 voice and opt-in A/V integration need their
latest Windows CI and actual retail playback verification.

## Developer-only opt-in

For a locally installed, unmodified original-game movie, set in
PowerShell before launching the native original-game executable:

    $env:BTB_EXPERIMENTAL_NATIVE_BINK_AUDIO = "1"

By default, the hybrid provider continues to select the original
binkw32.dll for files carrying compressed audio. The preview accepts
exactly one decoded source audio track, the original 640x480 global
video geometry, and a working game-owned DirectSound8 device. It neither
silently drops extra tracks nor writes decoded media to disk. Unsupported
movies fall back to the original x86 DLL when available. Instruction
walkthroughs continue to use the separate original help WAV behavior.

## Release blockers

1. Verify packet coverage, output gain, timing and PCM from actual
   unmodified retail BIKs against an independent reference.
2. Exercise DirectSound8 playback and DirectDraw7 game video on real
   Windows devices (x86 and x64), including final frames and long A/V drift.
3. Measure in-memory buffer limits/startup latency and add backpressured
   streaming for longer movies if needed.
4. Reconcile the hundreds-of-commits-old decoder branch with modern
   main without overwriting gameplay changes.
5. Only then consider making native audio the default.

Codec references: https://ffmpeg.org/doxygen/trunk/binkaudio_8c_source.html
and https://www.ffmpeg.org/doxygen/7.1/wma__freqs_8c.html .
The game links neither reference decoder.

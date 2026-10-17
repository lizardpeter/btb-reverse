# Original 2002 Bink1 audio: native decoding frontier

This is **not yet a playable native audio decoder**. The original BTB
Bink video path has independently verified YUV frame parity, but compressed
audio, overlap/add, PCM conversion and DirectSound synchronization remain
separate tasks. Do not mark the game DLL-free on account of this work.

## Implemented in C++ (source-only, no external codecs)

- \`bink32_audio_packets\`: parse the 12-byte per-track descriptors
  ([maximum uncompressed bytes, 16-bit sample rate plus 16-bit flags,
  track identifier]); preserve DCT/RDFT, mono/stereo and output preference.
  Distinguish zero-length track packets from a nonempty packet with a
  declared uncompressed length prefix.
- \`bink32_audio_spectrum\`: parse the compressed Bink1 (SJPi, **not** Bink
  version 'b' or Bink Audio 2) spectral block into frequency-domain floats:
  29-bit signed exponent/mantissa DC values, 8-bit quantizer selectors,
  25 critical-frequency boundaries, 8 or extended RLE coefficient groups,
  zero width groups, signed variable-width coefficients, and stereo DCT
  versus interleaved RDFT topology. Return the exact bit cursor for a
  caller to process a subsequent DWORD-aligned audio block.
- Synthetic release/CI tests for low/middle/high sample rates, 2048-point
  stereo DCT, 4096-point stereo RDFT, nonzero signed coefficients, zero
  spectra, mismatched format and truncated bitstreams.

Band derivation and compressed-format structure were cross-checked against
independent open FFmpeg Bink audio format documentation:
https://ffmpeg.org/doxygen/trunk/binkaudio_8c_source.html
and https://www.ffmpeg.org/doxygen/7.1/wma__freqs_8c.html .
These references are **not** native runtime dependencies. Native
compressed-audio output has not been differentially certified against
FFmpeg or the 2002 DLL.

## What must be finished before audio can replace the original DLL

1. Audit original track metadata and all compressed blocks in the retail
   movies; verify quantization/bit cursor and count blocks when a frame
   contains more than one compressed audio transform.
2. Implement and validate inverse DCT and inverse real DFT with the
   correct 2002 Bink1 normalization and channel conventions.
3. Retain and mix the 1/16-block history across blocks and frame
   boundaries. Handle empty packet frames without inventing PCM.
4. Generate native PCM with exact sample count, gain and clipping,
   and feed it to the actual DirectSound provider.
5. Synchronize against the movie clock, seek and pause lifecycle;
   certify original x86 and native x64 game playback against the
   retail BIK assets with independent reference samples.

\`Win32HybridBinkProvider\` keeps the real original-DLL path for
audio-bearing files. Never substitute a silent video for a movie that has
an audio track. Native video may be used directly only for explicitly
audio-free files; x86 still requires the original user-supplied DLL for
compressed-audio playback, and x64 must fail visibly for those movies.

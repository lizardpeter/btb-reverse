# Native Bink1 codec on current game main

The portable original-format SJPi Bink1 decoder is compiled and tested here **without replacing the modern Windows movie host**. The independent video, RGB surface conversion, audio packet/FFT, PCM16, sample clocks and original BIK diagnostic utilities live in the reconstruction library, not in a game-specific shell. No proprietary game assets or original DLL are embedded in this change.

Real-file video validation against the connected original archive covers 38 SJPi movies (11,266 frames) across global cinematics, walkthroughs, and the five Spud Skate clips. Every Y, U, V, and alpha plane matched FFmpeg byte-for-byte for every frame. The correct reference normalization changes only the four-byte signature in a temporary copy from SJPi to BIKi; BIKf is not a valid video reference for these assets. The original media files remain unchanged.

The native audio decoder and PCM16 serializer were tested on all 22 available original audio-bearing tracks against FFmpeg. All sample counts matched and the maximum signed-16-bit sample difference was one LSB. The source-declared PCM byte prefix is treated as a bounded hint rather than an exact per-frame sample count, consistent with the full-stream FFmpeg output. Original-DLL PCM parity and a real DirectSound device/synchronization test remain outstanding, so native A/V playback stays opt-in. The 23 Fireworks show clips are not present in the connected asset archive and have not received original-file parity testing.

To reproduce the local checks after building the portable tools:

```sh
cmake -S reconstruction -B build/reconstruction
cmake --build build/reconstruction --parallel --target btb_bink32_native_audio_audit btb_bink32_retail_video_audit
python tools/bink32_real_audio_differential.py --native build/reconstruction/btb_bink32_native_audio_audit --asset-dir /path/to/original/BTB-BBP
python tools/bink32_real_video_differential.py --native build/reconstruction/btb_bink32_retail_video_audit --asset-dir /path/to/original/BTB-BBP
```

The scripts create temporary reference copies only; they never rewrite or commit the supplied retail media. The video coverage-only tool is available when a reference decoder is not available.

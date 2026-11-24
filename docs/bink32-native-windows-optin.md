# Native reconstructed Bink1 playback on Windows

The Windows x64 game now selects the source-reconstructed Bink1 decoder for global movies (including single-track PCM audio), walkthroughs, Spud Skate's five streams, and the Fireworks movie bank by default. No environment switches are required. It does not fall back to the original 2002 x86 Bink DLL if media is missing, unsupported, or decoding fails. Such failures are explicit. This is a native-only integration candidate, not a claim that all retail movies or whole-game playback have been certified.

The Windows x86 build retains the retail DLL by default as a comparison path. Native x86 trials can use the existing environment flags:

```powershell
$env:BTB_EXPERIMENTAL_NATIVE_BINK_VIDEO = "1"
$env:BTB_EXPERIMENTAL_NATIVE_BINK_AUDIO = "1"
$env:BTB_EXPERIMENTAL_NATIVE_BINK_SKATE = "1"
$env:BTB_EXPERIMENTAL_NATIVE_BINK_FIREWORKS = "1"
```

The AUDIO flag enables the native global movie path for supported 640x480 movies with exactly one compressed audio track; video-only opt-in requires no compressed audio. The x86 comparison path may fall back to original playback. Walkthrough loops remain source 282x179 with separate original WAV narration. Native specialized activity streams retain their own source-backed rendering and timing constraints.

Original assets remain read-only. Compressed samples and decoded frames are held in memory; the experimental full-movie PCM buffer is bounded to 64 MiB and uses the game-owned DirectSound8 device with audio cursor pacing and monotonic EOS handling.

The original Win32 platform-link test now checks all four x64 native-only missing-media error paths with opt-ins disabled. This verifies fail-closed routing, not retail movie fidelity. Real-device playback, differential validation and a complete all-activity playthrough remain required.

# Opt-in reconstructed Bink1 playback on current main

The original 2002 x86 Bink DLL remains the default for the real modern game host; no ordinary player-visible behavior is replaced. This adapter reuses the portable native Bink1 decoder with real DirectDraw7 upload and DirectSound8 audio. Independent FFmpeg PCM16 comparisons passed on 22 original audio tracks (maximum one-LSB difference), and 38 available original movies matched FFmpeg video planes byte-for-byte across 11,266 frames. Original-DLL PCM parity and a real DirectSound device/synchronization test remain outstanding.

In PowerShell set the optional switches before launching the current game:

    $env:BTB_EXPERIMENTAL_NATIVE_BINK_VIDEO = "1"
    $env:BTB_EXPERIMENTAL_NATIVE_BINK_AUDIO = "1"

Setting the AUDIO flag implies attempting native video as well, only for supported source 640x480 movies. Video-only opt-in requires a movie with no compressed audio track; audio-bearing movies fall back to the original x86 DLL. Walkthrough loops remain source 282x179 with separate original WAV narration. Unsupported formats and open-time failures fall back to original playback where available. A failure after native playback has begun is reported rather than silently switching backends mid-movie. The x64 original DLL cannot be loaded.

All assets remain read-only, compressed samples and decoded video are kept in memory (no extracted game media files). The experimental full-movie PCM buffer is bounded to 64 MiB and uses the game-owned DirectSound8 device, with audio cursor pacing and monotonic EOS handling.

The original game still relies on its independent specialized Bink players for Spud Skate and Fireworks; this change does not replace them. Native PCM playback is not retail-certified, and Windows automated build/link tests cannot substitute for a real device test.

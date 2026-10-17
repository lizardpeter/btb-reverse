# Native Bink1 codec on current game main

The portable original-format SJPi Bink1 decoder is compiled and tested here **without replacing the modern Windows movie host**. The independent video, RGB surface conversion, audio packet/FFT, PCM16, sample clocks and original BIK diagnostic utilities live in the reconstruction library, not in a game-specific shell. No proprietary game assets or original DLL are embedded in this change.

Prior source-branch CI verified 33 retail movie video streams (8,505 frames) against FFmpeg, and eight synthetic audio formats (DCT/RDFT, mono/stereo, quiet/loud) within one PCM16 LSB. Main CI must revalidate the library in this new integration context. The production game continues to use the original Bink DLL for audio-bearing movies. Experimental native DirectSound movie playback remains in PR #133 and is not part of this PR.

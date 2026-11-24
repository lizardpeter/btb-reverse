# Native Bink1 codec and whole-installation audit

The portable reconstructed Bink1 decoder is integrated with the x64 native
Windows movie host for global movies, walkthroughs, the five Spud Skate
streams and Fireworks movie banks. See
[native provider selection and limits](bink32-native-windows-optin.md).
No original game media or original DLL is embedded. Real-device playback
certification remains outstanding.

Prior source-branch CI verified 33 retail video streams (8,505 frames) against
FFmpeg and eight synthetic audio formats within one PCM16 LSB. Those earlier
results do not certify every movie in the installed game or end-to-end
native playback. FFmpeg is a separate diagnostic reference only, never a
runtime decoder dependency.

## Audit every installed movie

Build the two portable original-format audit executables:

```sh
cmake --build build/reconstruction --target btb_bink32_retail_video_audit btb_bink32_native_audio_audit --parallel
python tools/audit_retail_bink.py /path/to/BTB-BBP \
  --video-tool build/reconstruction/btb_bink32_retail_video_audit \
  --audio-tool build/reconstruction/btb_bink32_native_audio_audit \
  --require-complete \
  --out analysis/generated/native-bink-audit.json
```

On Windows use the equivalent Release executables with `.exe` suffix.
The diagnostic workflow publishes the Linux tools as well. The Windows x64
repo-checks job publishes both Release audit executables and the Python
runner as `btb-native-bink-win64-tools`, separately from the game EXE.
`--require-complete` makes partial decode reports return a failing exit
code; `--expected-count N` additionally checks the number of discovered
movie paths against a separately established installation inventory. A count
match alone does not establish asset identity. The report records validated
frame/track totals and SHA-256 of each supplied native tool.

The script recursively discovers case-insensitive .bik paths, validates
metadata and frame offsets, reports identical-file aliases, decodes every
video frame and every declared audio track, and records each native tool's
exit status and output tail. Missing tools and limited-frame runs are
explicitly partial; any rejected header, failed decode or timeout fails the
audit. The report must be outside the original directory.

Use `--max-frames 20` for a video prefix probe or omit both decoder tools
for inventory only. Neither is a full decode result. The script writes no
game files and requires no FFmpeg or RAD library to decode. An independently
obtained raw YUVA reference can be supplied directly to the video audit
executable with `--reference-yuva` for pixel comparison.

A successful complete audit establishes native file/stream decoding, not
identical PCM or pixels, real-device rendering, synchronization or end-to-end
gameplay. Those are separate certification gates.

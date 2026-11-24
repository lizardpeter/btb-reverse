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

## Original disc media evidence (2026-11-24)

The 61 identified originals across Data/movies (33), Data/subgamespudskate
(5), and Data/SubGameFirework/graph (23) were decoded with the native
video/audio audit tools from main checkpoint 6c6fcf2 (diagnostics artifact
14899730650). All 12,732 video frames matched independent FFmpeg YUVA/YUV
references exactly; all 44 declared audio tracks had matching sample counts
and maximum PCM16 error at most one LSB. The 22 Fireworks audio tracks decoded
to exact zero, including all overlap samples; the remaining Fireworks clip
has no track. The native Fireworks bank now validates decoded silence rather
than rejecting a declared track. Audible or corrupt replacements fail closed.
The disc's uppercase OK.bik also revealed a case-sensitive Spud Skate name
check; both native and x86 comparison providers now follow retail Windows
case-insensitive naming.

Reference FFmpeg 7.1.5 received a temporary copy with SJPi magic translated
to BIKi, never modified original assets. The native video executable was
unmodified; the audio audit executable required a local ELF compatibility
adjustment to its sqrtf GLIBC symbol version (2.43 to 2.2.5), not a change to
decoder instructions. These results are codec differential evidence on
Linux, not original RAD DLL parity or Windows device playback certification.

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

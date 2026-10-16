# Native Bink1 retail-frame audit

This tool measures **retail pixel correctness** separately from
synthetic-unit-test coverage. It reads an original Bink1/SJPi `.bik`
without loading `binkw32.dll`, decodes complete alpha/Y/V/U picture
planes to canonical Y/U/V/A, and optionally compares **every decoded
byte** with raw `yuva420p` video produced by an independent decoder.

The input movie and independent reference are always read-only. No
proprietary file is checked into the public repository. Movie playback
and native audio are not implemented by this audit command.

## Build

From the repository root, with a C++26 toolchain and CMake:

```sh
cmake -S reconstruction -B build/bink32 -DCMAKE_BUILD_TYPE=Release
cmake --build build/bink32 --config Release --target btb_bink32_retail_video_audit
```

The executable will be `build/bink32/btb_bink32_retail_video_audit`
on single-configuration toolchains, or under
`build/bink32/Release/` on multi-configuration toolchains.

## Generate an independent reference

With an original *legally possessed* retail movie, generate raw
reference bytes in **Y, U, V, A** order. FFmpeg is one possible
independent decoder; it is **not** used or linked by the native tool.

```sh
ffmpeg -i designstartup.bik -map 0:v:0 -fps_mode passthrough -pix_fmt yuva420p -f rawvideo designstartup-reference.yuva
```

Keep the original file and reference separate. `yuva420p` contains
`width * height * 5 / 2` bytes per frame (full Y and A, quarter U
and V). For 640×480, that is **768,000 bytes per frame**. It is
important not to compare the native planar YUVA buffers with RGBA
or YUV420 output that has lost the alpha plane.

## Decode, localize divergence, and export

```sh
btb_bink32_retail_video_audit designstartup.bik --max-frames 12 --reference-yuva designstartup-reference.yuva
btb_bink32_retail_video_audit designstartup.bik --reference-yuva designstartup-reference.yuva --output-yuva native-designstartup.yuva
```

The command prints the actual frames decoded, or stops at the
first failed packet. With a reference, each mismatching frame
includes per-plane mismatch byte counts, the first `(x,y)`
coordinate, native/reference sample values, and maximum absolute
difference. The final summary includes totals by Y/U/V/A plane.

- **Exit 0:** all requested frames decoded and, if provided, match
  the reference byte-for-byte.
- **Exit 1:** invalid arguments, assets, output paths, or raw reference
  sizes.
- **Exit 2:** a BIK packet could not be decoded as a complete frame.
- **Exit 3:** all requested frames decoded but at least one byte
  differed from the independent reference.

The optional `--output-yuva` file includes only successfully decoded
frames, and neither retail input nor reference can be overwritten
through the output option. It is a diagnostic output, not a
certification of original-DLL parity by itself.

## Known reference evidence and limits

The independent decode of the untouched 227-frame 640×480
`designstartup.bik` has source SHA-256
`577c9340d779bf2afeca5797fd365793b236a35c5d26dd60cc54e1ceecffc5a9`.
Original reference frame-1 Y and frame-12 Y hashes are recorded in
`uregraph` under
`urn:ure:btb:bink32:evidence:designstartup-yuva-reference-pixels-20261016`.

These reference hashes **do not** mean the native decoder is
already bit-exact. Run the audit against the original file to obtain
native-vs-reference outcomes. This is currently restricted to alpha
SJPi with 16-aligned dimensions. Native audio decoding, broader
Bink revisions, video playback integration, and source-accurate
movie timing are separate remaining work.

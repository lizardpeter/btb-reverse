# Original BTB Bink1 video — byte-exact retail parity

Validated October 16, 2026 against the original unmodified BIK files
available in the BTB-BBP assets folder. Proprietary movies and the
independently decoded raw images are **not** committed to this repository.

The native C++26 `btb_bink32_retail_video_audit` executable decoded
**all 3,671 frames** across **14 separate retail movies**, with
**zero differences** against the independently decoded FFmpeg
`binkvideo` YUV/YUVA planar reference. For each frame, every visible
Y, U, V and (when encoded) A byte was compared, not just hashes or
selected screenshots.

| Retail filename | Dimensions | Source planes | Frames bit-exact |
|---|---:|---|---:|
| designstartup.bik | 640×480 | YUVA | 227/227 |
| designcomplete.bik | 640×480 | YUVA | 139/139 |
| dinostartup.bik | 640×480 | YUVA | 290/290 |
| adventstartup.bik | 640×480 | YUVA | 207/207 |
| introbig1.bik | 640×480 | YUVA | 779/779 |
| herdstartup.bik | 640×480 | YUVA | 263/263 |
| fireworkstartup.bik | 640×480 | YUVA | 178/178 |
| musicstartup.bik | 640×480 | YUVA | 109/109 |
| thq.bik | 640×480 | YUV (opaque) | 120/120 |
| squirelcomplete.bik | 640×480 | YUVA | 78/78 |
| fireworkcomplete.bik | 640×480 | YUVA | 239/239 |
| designwalthrough.bik | 282×179 | YUV (opaque) | 445/445 |
| advent2walthrough.bik | 282×179 | YUV (opaque) | 231/231 |
| skate2walthrough.bik | 282×179 | YUV (opaque) | 366/366 |
| **Total** | | | **3,671/3,671** |

## Format closure and original evidence

- Original `SJPi` bitstreams support both alpha sections and non-alpha
  YUV-only movie packets. Native plane order is `A,Y,V,U` or `Y,V,U`;
  exported frame order is `Y,U,V,A`, with synthetic opaque A only
  for non-alpha **presentation**.
- Visible odd-sized planes are cropped from padded 8×8 codec
  surfaces. Chroma visible dimensions use
  `ceil(width/2) × ceil(height/2)`; the padded blocks persist
  unmodified across reference frames for motion compensation.
- A genuine original-frame failure at designstartup frame 24 was
  caused by a 6-bit Huffman table lookup requiring more *lookahead*
  than a valid 2-bit code symbol consumed at a packet tail. The native
  reader now permits zero-padded table lookup while refusing to
  consume nonexistent bits. A regression prevents recurrence.
- No original RAD DLL or FFmpeg runtime dependency is used in the
  native video decoder. FFmpeg is used only to produce the **separate**
  reference for differential verification.
- Independently decoded video is **not** a completed Bink codec
  replacement: native compressed audio, presentation surface
  conversions, playback timing, game callback/ABI integration,
  and end-to-end game cinematics still require verification.

## Reproduce with your legally possessed original BIK

```sh
cmake -S reconstruction -B build/bink32 -DCMAKE_BUILD_TYPE=Release
cmake --build build/bink32 --target btb_bink32_retail_video_audit
# The Bink demuxer may need -f bink when the file is named *.bik.bin
ffmpeg -f bink -i designstartup.bik -map 0:v:0 -fps_mode passthrough -pix_fmt yuva420p -f rawvideo designstartup.yuva
build/bink32/btb_bink32_retail_video_audit designstartup.bik --reference-yuva designstartup.yuva
# Walkthroughs contain no encoded alpha; compare yuv420p instead:
ffmpeg -f bink -i designwalthrough.bik -map 0:v:0 -fps_mode passthrough -pix_fmt yuv420p -f rawvideo designwalthrough.yuv
build/bink32/btb_bink32_retail_video_audit designwalthrough.bik --reference-yuva designwalthrough.yuv
```

The tool refuses incomplete frames and returns a nonzero exit status
for any mismatched native pixel.

## Original file fingerprints for reproducibility

SHA-256 of original unmodified retail BIK containers:

```text
designstartup.bik       577c9340d779bf2afeca5797fd365793b236a35c5d26dd60cc54e1ceecffc5a9
designcomplete.bik      f106e5fc57b4cf85d50359a00db4ea5db7d6f987d5a89a0b5fe5cf76fa825a96
dinostartup.bik         7e3f163c53afa1e940a09f39a15ffd1bade1a69e760ea8c8e83de90d02e1b686
adventstartup.bik       a6dbb4294813180372d98d5c0d6bbe3013dad58a0de82c91426f991664c1d506
introbig1.bik           fd18c5e3b8f198c3106532f0a1ae3c392af6e7c88bdfd687a3409e236eea2569
herdstartup.bik         0894b0b8bf38e2fcc88d9c6737e635c2a608cdc72a13fb2714fa285289341201
fireworkstartup.bik     3092611e3c9a3ee65e0fef77bedaecd58cd8e1e898da487fc9db3be34d14d4af
musicstartup.bik        0e0cf826263015812b971e363ef6b272085bbc95854d5e5e7f8c02e3bc6b47e6
thq.bik                 e7ae3315f57b5e42cd491519899056035c85fe71089a327d4eec77d19119e761
squirelcomplete.bik     39923ab39f7df27f8988f90d8239632d0c36859b557c1c8062a384686ea73417
fireworkcomplete.bik    79a24c993bb4afcf2ff1f850a612cf7069df571b9e318fa6a29d9d07452c43a3
designwalthrough.bik    61f8466ff7845cdc64769159e4d80b2a58c5f99cd20a9309e7ed2a4f35631573
advent2walthrough.bik  b42a56b34cbe5f9ba44b9ddd624ee5f4f3921638cc75f860f3e6e4c8f2e7fdb6
skate2walthrough.bik   2605cff19c6d98672a63b99fa462b45cf88d41f86795310bd7ad1e2cadf11272
```

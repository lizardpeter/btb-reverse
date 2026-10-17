# Original BTB Bink1 video — byte-exact retail parity

Validated October 16, 2026 against the original unmodified BIK files
available in the BTB-BBP assets folder. Proprietary movies and the
independently decoded raw images are **not** committed to this repository.

The native C++26 `btb_bink32_retail_video_audit` executable decoded
**all 8,505 frames** across **all 33 original retail movies**, with
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
| BBC.bik | 640×480 | YUV (opaque) | 132/132 |
| BBCSTING.bik | 640×480 | YUV (opaque) | 132/132 |
| Hitsting.bik | 640×480 | YUV (opaque) | 119/119 |
| Hotsting.bik | 640×480 | YUV (opaque) | 49/49 |
| advent1walthrough.bik | 282×179 | YUV (opaque) | 435/435 |
| adventcomplete.bik | 640×480 | YUVA | 166/166 |
| dinocomplete.bik | 640×480 | YUVA | 146/146 |
| dinowalthrough.bik | 282×179 | YUV (opaque) | 208/208 |
| fireworkwalthrough.bik | 282×179 | YUV (opaque) | 399/399 |
| herdcomplete.bik | 640×480 | YUVA | 227/227 |
| herdwalthrough.bik | 282×179 | YUV (opaque) | 641/641 |
| musiccomplete.bik | 640×480 | YUVA | 166/166 |
| musicwalthrough.bik | 282×179 | YUV (opaque) | 317/317 |
| sceneset.bik | 640×480 | YUVA | 261/261 |
| skate1walthrough.bik | 282×179 | YUV (opaque) | 503/503 |
| skatecomplete.bik | 640×480 | YUVA | 167/167 |
| skatestartup.bik | 640×480 | YUVA | 214/214 |
| squirelstartup.bik | 640×480 | YUVA | 149/149 |
| squirelwalthrough.bik | 282×179 | YUV (opaque) | 403/403 |
| **Total** | | | **8,505/8,505** |

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
BBC.bik                  30607818fd27f34ab2f046e1528691b1e981262e4d94bee9f7da8e0ee01ae72d
BBCSTING.bik             eb372b22456b078d7170c37ed24550a29c7a8d23afd86e1ab9071048cfec94a9
Hitsting.bik             2013f913734e7960134f505d3833b67f2e0cb8bff61814387a98c9765e8c0176
Hotsting.bik             22297a34627a2ae2fc464b1fee3113327ee1f386121e57b4e5bf6097da98787d
advent1walthrough.bik    19153046924ec45f8922ea29d7bff6a44b3f3f4afb8cc5a48ec9c8c040c88fe1
adventcomplete.bik       ddfe84e0bd878c46db32f0d6f074c7d3c9f0e74dbbff4932c3a7ee3de586101e
dinocomplete.bik         c7e6c51dda703e0df1107e91ea4f8e4f3002b919d0ba843f70ae69f02a76d0c7
dinowalthrough.bik       0b8ccb80fae1de9cfaeac17d7a454f5b7a7c27fb889569f825fdf2dac106d1ca
fireworkwalthrough.bik   c5cf1b0f88886e572bca04477a2cef84dfc31f397235b2b1a41c5d2487f3ec1d
herdcomplete.bik         32d58860d0a62ddfd451039184876654bcd321f6c9c51a7d6a8f662cf6a9e4ba
herdwalthrough.bik       5f582039353a2d791ed7beea6879a301d6cfd3e6f4c74d141f9086f1627275ba
musiccomplete.bik        2dc3e0b3f78c755ebfe8fb7bb1857e3fa85adb3a730d392dbf2ff720e7ee5601
musicwalthrough.bik      9ee4d083e8ad983c37fe408837e1fc8c53c785ef6952d934636c111084f4866c
sceneset.bik             796bfd737c41b540e48203b7dae13b7e59886e42df0957a04e953e74c7f3487d
skate1walthrough.bik     c6807f67103efceecddd791edd47aaa9a9e1ea9a6bbaf95b40a538374d816a8e
skatecomplete.bik        540f738f7375ae9f6addbaaf1eaa4f5d8e734a985cd9020f9349c3b6fc6a44c5
skatestartup.bik         9ff53e8f24290513d220bd91ea2439a1a4ed4bd16660cb88fa76bbcedbbc5b82
squirelstartup.bik       85f5ebc5e466ff3dcbe871ec8b112b8a7e0844ac28f60832142afccdfca3f305
squirelwalthrough.bik    976ca90666777734e2513c4c8a18e2f12cfe7d29cbf70b857a27afd964c4ea71
```

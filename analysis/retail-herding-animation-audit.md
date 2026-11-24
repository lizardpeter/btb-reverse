# Retail Pets Corner sprite/animation audit

Source: Bob the Builder - Bob Builds a Park.exe, 311296 bytes, SHA-256 `c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05`. Read-only inspection; no game binary or asset included.

## Sprite sheet addressing

DrawHerdingActivity (0x416370) explicitly uses `(direction * 12 + frame) * 53` for type 3 (duck); original DUCK_01.bmp is 5088 x 50, eight banks of twelve frames. Sheep: 7840 x 103, eight banks of ten 98px frames; rabbit: 5840 x 69, eight banks of ten 73px frames. The old duck stride of six addressed the wrong direction and home-route cells.

## Separate update and draw animation rules

Raw signed little-endian 32-bit tables: 0x443AF0 = {6,0,3}; 0x443AFC = {10,6,7}. UpdateHerdingAnimal indexes these by type minus one (sheep, rabbit, duck), including 0x417C62/0x417C78 and 0x416D13/0x416D37. Update wraps only at the exclusive upper limit. Duck home motion is a distinct branch (0x416CDF..0x416D11), clamping to frames 9..11, without the normal first-stage timer reload. DrawHerdingActivity has its own explicit rabbit 3..6 and duck 0..5 wrap rules: **do not use those to substitute for the AI update tables**. Both phases must be retained independently.

Portable floating-point math and full live retail differential playback remain separate validation work.

# Spud Maze retail repair interaction and mouse input

Original SHA256 c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05.
00421360 uses each loaded sprite's width and height, the anchor
(x + width/2, y + height + 20), integer-truncated Bob coordinates,
distance strictly less than 40 (float 0043B4B0), and horizontal
distance strictly less than 15. Previously the reconstruction used
a fixed height and half-height offset, the left edge, and radius 20.
The actual 32 shipped BMP headers were inspected: widths 36..96,
heights 34..79. Native initialization now reads those headers;
missing or invalid retail dimensions are a hard error.
004211A0 mouse input uses y+50, a 26-unit dead zone, sin threshold
0.1 and cos threshold 0.4 (0043B4A8/0043B400/0043B4A0).
Unlike Maze it does not debounce. It is now wired into the production
Spud activity. Tests use metadata fixtures and physical input;
interactive differential validation remains outstanding.

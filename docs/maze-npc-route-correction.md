# Maze Spud NPC route corrections

0041CCB0 state 2 advances DAT_005107A0 to the actually reached
route node. The earlier native coordinator retained the spawn node,
so carrying paths could be calculated from the opposite side of
the map. Native paths now strip the separate start node, update the
current node at every waypoint and track outgoing direction.
State 4 uses Bob's current node and all four adjacency entries
when choosing a carrying route. State 5 retains the actual
truncated waypoint distance and consumes the original optional
waypoint voice RNG before the collision voice RNG. State 6 consumes
a random edge only on the middle screen. 0041B140 resets the
offscreen delay on crossing a portal, now mirrored by the host.
Source table checks: 00444204 regular speed 1/1/2, 00444210
carrying speed 1/1/2, 004441F8 wait 3000/2000/800.
These are source-logic fixes; exact animation timing and graphical
playthroughs still need validation.

## Native sprite-sheet crash correction

0041CCB0 sets the animation frame to zero at spawn, not seven. The
package pickup adds seven to the current walking frame; starting at
seven previously produced column 14, outside the original 1792px
(14 x 128) sheet. The driver now renders before the NPC update as in
0041D4D0, advances visible NPC frames using the 0041C110 walking/carrying
ranges, and applies the verified speed-table half-counter values on
route setup. The carrying flag is set by phase 4, not phase 2. Sprite
rectangle direction 4 maps to row 3 (not animation column 4); this
also fixes the skipped player pickup frame and out-of-bounds direction.
The collision distance uses four truncated coordinates as in 0041CCB0.
Regression covers walking wrap, maximum legal pickup frame, carrying
wrap, and direction fallback. Physical playthrough remains required.

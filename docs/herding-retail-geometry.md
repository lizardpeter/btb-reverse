# Original Pets Corner geometry recovered directly from retail PE32

Binary: `Exe/Bob the Builder - Bob Builds a Park.exe`, 311,296 bytes,
SHA-256
`c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05`.
Original file was retrieved from the connected `BTB-BBP/Exe` folder,
not reconstructed from a screenshot, inferred from asset names, or a
modern generic collision library.

Source implementations:
- `reconstruction/include/btb/retail_point_in_polygon.hpp`
- `reconstruction/include/btb/herding_navigation_boundary.hpp`
- `reconstruction/include/btb/herding_retail_rect_contact.hpp`
- `reconstruction/src/full_game_herding_event_simulation.cpp`
- Their matching source-only test files.

## 0x00428520 — shared PointInPolygon

Inputs: `Vec2i* vertices`, signed `count`, signed query `x` and `y`.
The helper walks edges from first vertex to each following vertex,
including the final edge back to vertex 0 (loop counter 1..count;
an `IDIV` yields the modulo index).

It initializes its crossing count to zero. For every edge:

1. Test **`queryY > min(y0,y1)` and `queryY <= max(y0,y1)`**.
2. Test **`queryX <= max(x0,x1)`**.
3. Reject horizontal edges (`y0 == y1`).
4. On a vertical edge (`x0 == x1`), count a crossing directly.
5. Otherwise compute `x_cross = x0 + signed32((x1-x0)*(queryY-y0)) /
   signed32(y1-y0)`. Native `SUB/IMUL` wrap at 32 bits; native signed
   `IDIV` truncates toward zero. The disassembly then uses x87 `FILD`,
   `FCOMPP` and `FNSTSW` to count a crossing when
   **`queryX <= x_cross`**.

**Unusual return convention:** even parity -> **1** (outside);
odd parity -> **0** (inside). This is exactly the result of
`AND 0x80000001; NEG; SBB EAX,EAX; INC EAX` at 0x4285D8..0x4285EF.
This is not a Boolean `true = inside` API. The C++ implementation
provides both `original_point_in_polygon_status` (0/1 source form)
and `original_polygon_contains` (explicit Boolean).

Boundaries are asymmetric because of the strict lower Y bound and
inclusive upper Y/crossing X bounds. The integer intersection matters
when a diagonal's real-valued x-intersection lies between integers.

**Callsites:** `UpdateHerdingActivity` at
0x418C54, 0x418C87, 0x418CC0; free-roaming animals at 0x419167;
`UpdateHerdingAnimal` at 0x4170EC and 0x41720E.

## 0x00418C2D–0x00418CE4 — Farmer Pickles navigation correction

After all Herding entity updates, retail converts Farmer Pickles'
new floating X/Y to signed integer queries by calling
`0x4304D0`. The latter explicitly changes the x87 rounding control
to `RC=11`, executes `FISTP QWORD`, then restores the control word.
This is truncation toward zero, *not floor*.

The main navigation polygon is `herd.txt` group 0 after
the verified constructor transformation `x -= 64, y -= 100`.

1. If `PointInPolygon(currentX,currentY) == 0` (inside), retain
   original floating position.
2. If outside, the source checks the frame-local count of active
   directional axes at `0x418C65` and the original mouse-navigation
   mode global `0x00443A9C`. It permits per-axis recovery exactly
   when **directional_axes > 1 OR mouse_navigation_mode != 0**.
   The count is incremented by original keyboard branches at
   `0x4184F4` / `0x418545`; the mouse-mode decision is made
   in the earlier `0x418334..0x41846B` input dispatch.
3. If axis recovery is permitted, test **(previousX,currentY)** first:
   if inside, restore only floating X using original previous integer X.
4. Otherwise test **(currentX,previousY)**: if inside, restore only
   floating Y using original previous integer Y.
5. Otherwise restore **both** floating coordinates from the previous
   integer position.

This is original axis-slide priority, not a generic nearest-point-on-
polygon or rectangle clamp. It needs two or three polygon evaluations
only when the attempt leaves the navigation area.

`retail_herding_axis_recovery_enabled` and
`retail_herding_navigation_boundary_step` together encode
the complete confirmed branch order and its source-level guard.
`HerdingPicklesBoundaryEvidence` carries the raw candidate,
the actual input-axis count and the original `0x443A9C` mode
global, not a guessed Boolean. The simulation checks finalized
Pickles floats against the independent binary-derived result
before committing animal state. Contradictory frames fail.
The movement constant at original `0x43B444` was also verified
as IEEE-754 float **1.5**.

## 0x004169A0–0x00416A2D — original rectangle collision

Inputs are pointers to two signed integer rectangles, each
`{left,top,right,bottom}`.

The helper tests the following **four corners of the first rectangle**
for inclusive containment inside the **second** rectangle, in order:

1. `(first.left, first.top)`
2. `(first.right, first.top)`
3. `(first.left, first.bottom)`
4. `(first.right, first.bottom)`

It returns **1** as soon as any corner is inside and **0** otherwise.

The operation is **one-way**. If the second rectangle is fully inside
the first, but no corner of the first lies in the second, the result
is zero despite geometric intersection. Cross-shaped overlaps can
also return zero. A standard symmetric AABB test would not preserve
the 2002 retail behavior.

The routine is called by Herding's collision code, including the
Farmer Pickles actor-contact paths around 0x418A7F/0x418ADC/
0x418B71/0x418C04. The exact order of first/second arguments
must be preserved at each individual callsite.

Further audit of `UpdateHerdingAnimal` at
`0x4179BF..0x417A2E` established the actual argument order:
**the current animal's integer sprite rectangle is first; Scruffty's
integer sprite rectangle is second**. Each is computed as the entity's
world `(x,y)` plus `(source_right-source_left,` 
`source_bottom-source_top)` for the bottom/right corner.
`original_herding_entity_rect` and
`original_herding_animal_hits_scruffty` now reconstruct those values
directly from 0x64-byte entity records. `HerdingEventSimulation`
derives and validates contact independently; extra caller-provided
rectangle evidence is optional and, when present, must exactly match
the derived ordered rectangles. A guessed symmetric overlap cannot
remove an animal from the follower list.

Source regression scenarios now include a Medium-level original entity
population with Scruffty and a confirmed animal-first contact, plus
an overlapping nested-rectangle configuration that must **not**
trigger the original routine.

## What remains unrecovered

These findings close the polygon and one-way rectangle *kernels*,
but do not complete all animal motion:

- source code producing the initial per-entity movement vectors and
  exactly allocating the entity starting positions;
- the complete steering speed, angle update, and target-selection calculations
  for free roam, follower placement, Scruffty patrol, escape, and home;
- exact source rectangles for each individual collision callsite;
- native comparison runs on difficult boundary/corner frames.

The surrounding `HerdingEventSimulation` is transactional at the
recovered game-state level. It still requires a genuinely reconstructed
`OriginalHerdingMotionSource` to supply physical motion and collision
candidates. No fake movement or fabricated sound is inserted.

**Compilation and execution remain deferred by project instruction.**
The committed tests are source fixtures, not passing build results.

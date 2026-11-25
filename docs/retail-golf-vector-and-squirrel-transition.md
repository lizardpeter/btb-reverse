# Retail Golf numerical and Squirrel level-boundary follow-up

Verified original executable SHA-256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05.

* 00414A30 loads binary64 0x3f91df5691cb5231 from 0043B388, multiplies by integer degrees, then fcos/fsin and integer magnitude, and only then stores each output as float32. The former pi/180 constant and intermediate float32 rounding differed. A local x86-64 x87-instruction replay of the function's arithmetic compared all 91 playable/marker angles (-2..88) and 141 magnitudes (0..140): 12,831 vector comparisons, 0 mismatches for the revised formula, 12,600 for the previous formula. Regression checks preserve exact representative bits. Original PE32 execution was not required.
* 00415080 raw coordinator: distinct target-hit and landing-mark counters, ascent/descent state, zero-height target gate, saturation rather than wrap of swing cells, raw state increments including simultaneous ground stop/hit, exact feedback thresholds 24/50/50 and reversed middle/high target index. The typed full-game round retains its explicit ResolveLanding normalization for overlapping artificial targets rather than exposing raw invalid phase values.
* Squirrel 004273F0 only writes the pending transition latch after scene update. 004279F0 executes 00425780 on the following update, before the new level scene, skipping the conveyor and placement logic on that update. The integration test observes section 7 and the latch before the level index advances. This corrects the failing level-boundary regression in run 45546199371 (252/253 Linux CTests passed).

The optional GNU/Clang x86-64 comparison is reproducible with:

```sh
g++ -std=c++20 -O2 -Iinclude tools/verify_retail_golf_x87.cpp src/golf_runtime.cpp -o /tmp/btb-golf-x87
/tmp/btb-golf-x87
```

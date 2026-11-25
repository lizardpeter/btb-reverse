# Live Squirrel display mode

Original 00427320 checks process-global 0044DE0C: zero is fullscreen (5 pixels every update); nonzero is windowed (50 pixels every second update). The old native driver always passed false even though the Win32 host initializes a windowed display. The actual current Win32 window mode now reaches the activity input, including on mode changes. The source-only fixture default remains fullscreen.

The window counter 0051511C advances even while the conveyor is stationary, and original 00425160 does not reset it on activity entry. The live driver calls the source kernel on every normal update and preserves that counter across unload/reinitialization. A native fixture drives 36 complete windowed games (all difficulties, 12 seeds each), checking conveyor position, cadence, animation cell, counter, regeneration and completion. It also checks counter persistence and switching display mode. The existing fullscreen regression remains in place. The typed three-piece level bound remains deliberate; the dormant fourth raw slot is not exposed through the live driver.

No graphics assets or executable are added. Physical display parity is still separate from these driver regressions. CI pending at commit time.

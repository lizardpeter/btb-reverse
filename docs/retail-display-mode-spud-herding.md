# Remaining display-dependent gameplay branches

- 00422400: Bob's Spud Maze animation delay is only enforced for DAT_0044DE0C == 0. Windowed mode advances every update for walking, idle, repair and outcome narration. The final repair wrap sets countdown 4 and must not invoke a second animation update in that same frame. Regression tests cover both cadence branches, windowed walking/narration and a complete 30-update repair (versus 120 fullscreen updates).
- 00419600: Pets Corner initialization explicitly sets DAT_0051C284 to zero if DAT_0044DE0C == 1. The root supplies the display mode to initializers after menu selection; the faithful Herding driver therefore uses Easy in windowed mode. This is original behavior rather than an enhancement or a user-selected difficulty override. Subsequent display changes do not reinitialize the activity.

These are reconstructed instruction branches, not physical interactive parity claims. The source fixture defaults to fullscreen unless windowed mode is explicitly supplied.

# 0x00402C90 retail StopAllManagedSounds fidelity fix

The Ghidra export of original StopAllManagedSounds at address \`0x00402C90\`
proves that the function iterates all 80 **sound ID by slot** entries, calls
\`IsSoundIdPlaying\` for each ID, and calls \`StopSoundSlot(slot)\` only if it
reports true.

The previous source-only audio effect planner stopped **every allocated**
managed voice, including stopped/cached groups. This diverged from the
original executable and could introduce unnecessary DirectSound operations.

\`reconstruction/src/full_game_audio.cpp\` now observes the sound group
corresponding to the reverse sound-ID mapping and emits a stop/rewind
command only when that group is actually playing. Its invalid host metadata
branch is explicitly a memory-safety guard rather than an original x86
behavior.

\`reconstruction/tests/full_game_audio_source_test.cpp\` verifies an
allocated stopped group stays cached and untouched, a playing group is
stopped, repeated stop calls are no-ops, and the stopped slot can be reused.

This source-level planner and its tests are now actual CMake build targets,
rather than uncompiled files. The application-level native session and
real DirectSound provider remain separate integration gates.

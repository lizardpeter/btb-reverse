#pragma once
namespace btb::full_game {
// Exercise the actual GameRoot, original activity driver vtables and the
// shared 80-slot sound manager from one final linked executable image.
// This is a linker/integration smoke test, NOT retail behavior verification.
[[nodiscard]] bool verify_linked_recompiled_core();
// Integration fixture covers real GameRoot driver initialize/update/unload
// and original save formats in the final compiled game, not retail parity.
[[nodiscard]] bool verify_integrated_activity_fixtures();
}

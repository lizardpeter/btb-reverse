#include "btb/win32_original_platform_link.hpp"
#include <cassert>
int main() {
#if defined(_WIN32)
    assert(btb::full_game::verify_linked_win32_providers());
#endif
    return 0;
}

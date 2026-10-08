#include "btb/game_session.hpp"

#ifndef _WIN32
#include <iostream>
#include <string_view>

// Linux CI builds the exact same integrated session and gameplay libraries.
// The actual window host is Win32; no substitute graphical host is claimed.
int main(int argc, char** argv) {
    btb::host::Session app;
    if (argc == 1 || (argc == 2 && std::string_view(argv[1]) == "--smoke")) {
        const auto dispatch = app.dispatcher();
        if (!dispatch.dispatch ||
            dispatch.dispatch->state !=
                btb::game_flow::State::ActivitySelectUpdate) {
            return 1;
        }
        std::cout << "btb_game: integrated C++26 host ready; "
                     "Win32 is the graphical platform.\n";
        std::cout << "To load original Dinosaur data here: "
                     "btb_game --dino-level /path/to/dino.txt\n";
        return 0;
    }

    if (argc == 3 && std::string_view(argv[1]) == "--dino-level") {
        if (!app.load_dinosaur(
                argv[2],
                btb::dino::Species::Raptor,
                btb::dino::Difficulty::Easy)) {
            std::cerr << "Cannot open level: " << app.error() << '\n';
            return 2;
        }
        std::cout << "Parsed " << app.dinosaur()->pieces().size()
                  << " original Dino pieces, entered recovered dispatcher "
                  << app.retail_state() << ", composed "
                  << app.dino_frame().commands.size() << " draw commands.\n";
        return 0;
    }

    std::cerr << "Usage: btb_game [--smoke | --dino-level dino.txt]\n";
    return 2;
}
#endif

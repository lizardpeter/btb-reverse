#include "btb/game_session.hpp"

#include <iostream>
#include <filesystem>
#include <fstream>

#define CHECK(test) do { \
    if (!(test)) { \
        std::cerr << "game_session check failed at line " << __LINE__ \
                  << ": " #test << '\\n'; \
        return 1; \
    } \
} while (false)

int main() {
    using namespace btb;
    using namespace btb::host;

    Session app;
    CHECK(app.screen() == Screen::ActivitySelect);
    CHECK(app.dispatcher().dispatch);
    CHECK(app.dispatcher().dispatch->state ==
           game_flow::State::ActivitySelectUpdate);

    // An unavailable retail level is a recoverable UI error.
    CHECK(!app.load_dinosaur(
        "__missing_dino_data__.txt",
        dino::Species::Raptor,
        dino::Difficulty::Easy));
    CHECK(app.screen() == Screen::ActivitySelect);
    CHECK(!app.error().empty());

    const auto path = std::filesystem::temp_directory_path() /
                      "btb_integrated_host_dino_fixture.txt";
    {
        std::ofstream out(path);
        CHECK(out);
        out << "2\n100 100\n200 200\n10 10\n300 300\n"
               "80 300\n100 120\n-1 -1\n1 0\n";
    }

    CHECK(app.load_dinosaur(
        path, dino::Species::Raptor, dino::Difficulty::Easy));
    std::filesystem::remove(path);
    CHECK(app.screen() == Screen::Dinosaur);
    CHECK(app.dispatcher().dispatch);
    CHECK(app.dispatcher().dispatch->state == game_flow::State::DinoRun);
    CHECK(app.dinosaur());
    CHECK(app.dinosaur()->pieces().size() == 2);
    CHECK(!app.dino_frame().commands.empty());
    CHECK(app.last_sound_id() == dino::intro_sound_id(0));

    app.set_piece_dimensions(0, 40, 40);
    app.set_piece_dimensions(1, 40, 40);
    app.pointer_down({20, 20});
    CHECK(app.carried_piece() && *app.carried_piece() == 0);
    const auto anchor = app.carried_piece_top_left({210, 190});
    CHECK((anchor == dino::Vec2i{200, 180}));
    app.pointer_up({100, 100}); // wrong target for piece ID 1
    CHECK(app.carried_piece());
    CHECK(app.dinosaur()->pieces()[0].state ==
           dino::PieceState::RejectedDrop);
    app.pointer_up({205, 198}); // correct target for piece ID 1
    CHECK(app.dinosaur()->mode() ==
           dino::InteractionMode::FinalizeAcceptedDrop);
    app.tick();
    CHECK(!app.carried_piece());
    CHECK(app.dinosaur()->completed_piece_count() == 1);
    CHECK(app.screen() == Screen::Dinosaur);

    app.pointer_down({310, 310});
    CHECK(app.carried_piece() && *app.carried_piece() == 1);
    app.pointer_up({100, 100});
    app.tick();
    CHECK(app.dinosaur()->complete());
    CHECK(app.screen() == Screen::DinosaurComplete);
    CHECK(app.last_sound_id() == dino::completion_sound_id(0));
    CHECK(app.dispatcher().dispatch->state == game_flow::State::DinoRun);

    app.back_to_activity_select();
    CHECK(app.screen() == Screen::ActivitySelect);
    CHECK(app.dinosaur() == nullptr);
    CHECK(app.dispatcher().dispatch->state ==
           game_flow::State::ActivitySelectUpdate);
}

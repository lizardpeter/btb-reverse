#include "btb/game_session.hpp"

#include <cassert>
#include <filesystem>
#include <fstream>

int main() {
    using namespace btb;
    using namespace btb::host;

    Session app;
    assert(app.screen() == Screen::ActivitySelect);
    assert(app.dispatcher().dispatch);
    assert(app.dispatcher().dispatch->state ==
           game_flow::State::ActivitySelectUpdate);

    // An unavailable retail level is a recoverable UI error.
    assert(!app.load_dinosaur(
        "__missing_dino_data__.txt",
        dino::Species::Raptor,
        dino::Difficulty::Easy));
    assert(app.screen() == Screen::ActivitySelect);
    assert(!app.error().empty());

    const auto path = std::filesystem::temp_directory_path() /
                      "btb_integrated_host_dino_fixture.txt";
    {
        std::ofstream out(path);
        assert(out);
        out << "2\n100 100\n200 200\n10 10\n300 300\n"
               "80 300\n100 120\n-1 -1\n1 0\n";
    }

    assert(app.load_dinosaur(
        path, dino::Species::Raptor, dino::Difficulty::Easy));
    std::filesystem::remove(path);
    assert(app.screen() == Screen::Dinosaur);
    assert(app.dispatcher().dispatch);
    assert(app.dispatcher().dispatch->state == game_flow::State::DinoRun);
    assert(app.dinosaur());
    assert(app.dinosaur()->pieces().size() == 2);
    assert(!app.dino_frame().commands.empty());
    assert(app.last_sound_id() == dino::intro_sound_id(0));

    app.set_piece_dimensions(0, 40, 40);
    app.set_piece_dimensions(1, 40, 40);
    app.pointer_down({20, 20});
    assert(app.carried_piece() && *app.carried_piece() == 0);
    const auto anchor = app.carried_piece_top_left({210, 190});
    assert((anchor == dino::Vec2i{200, 180}));
    app.pointer_up({100, 100}); // wrong target for piece ID 1
    assert(app.carried_piece());
    assert(app.dinosaur()->pieces()[0].state ==
           dino::PieceState::RejectedDrop);
    app.pointer_up({205, 198}); // correct target for piece ID 1
    assert(app.dinosaur()->mode() ==
           dino::InteractionMode::FinalizeAcceptedDrop);
    app.tick();
    assert(!app.carried_piece());
    assert(app.dinosaur()->completed_piece_count() == 1);
    assert(app.screen() == Screen::Dinosaur);

    app.pointer_down({310, 310});
    assert(app.carried_piece() && *app.carried_piece() == 1);
    app.pointer_up({100, 100});
    app.tick();
    assert(app.dinosaur()->complete());
    assert(app.screen() == Screen::DinosaurComplete);
    assert(app.last_sound_id() == dino::completion_sound_id(0));
    assert(app.dispatcher().dispatch->state == game_flow::State::DinoRun);

    app.back_to_activity_select();
    assert(app.screen() == Screen::ActivitySelect);
    assert(app.dinosaur() == nullptr);
    assert(app.dispatcher().dispatch->state ==
           game_flow::State::ActivitySelectUpdate);
}

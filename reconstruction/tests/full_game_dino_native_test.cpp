#include "btb/full_game_dino.hpp"

#include <array>
#include <cassert>
#include <cstdio>
#include <cstdlib>
#undef assert
#define assert(expr) do {if(!(expr)){ \
  std::fprintf(stderr,"native Dino assertion: %s (%s:%d)\n",\
      #expr,__FILE__,__LINE__);std::abort();}} while(false)
#include <filesystem>
#include <fstream>
#include <string>

namespace {
namespace fs=std::filesystem;
using namespace btb::full_game;
using btb::game_flow::State;

void touch(const fs::path& p) {
    fs::create_directories(p.parent_path());
    std::ofstream(p,std::ios::binary).put('x');
}
void write_bitmap(const fs::path& p,int width=20,int height=20) {
    fs::create_directories(p.parent_path());
    std::array<unsigned char,54> bytes{};
    bytes[0]='B';bytes[1]='M';bytes[2]=54;
    bytes[10]=54;bytes[14]=40;
    for(int k=0;k<4;++k) {
        bytes[18+k]=static_cast<unsigned char>((width>>(8*k))&255);
        bytes[22+k]=static_cast<unsigned char>((height>>(8*k))&255);
    }
    bytes[26]=1;bytes[28]=24;
    std::ofstream out(p,std::ios::binary);
    out.write(reinterpret_cast<const char*>(bytes.data()),
              static_cast<std::streamsize>(bytes.size()));
}
void retail_level(const fs::path& root,const char* species,
                  const char* difficulty) {
    const auto folder=root/"Data"/"SubGameDino"/"Dino"/species/difficulty;
    fs::create_directories(folder);
    std::ofstream(folder/"dino.txt")
        << "1\n100 100\n10 10\n80 300\n-1 -1\n0\n";
    write_bitmap(folder/"Piece0.bmp");
    write_bitmap(folder/"background.bmp",640,480);
    touch(root/"Data"/"SubGameDino"/"Bob.bmp");
    touch(root/"Data"/"SubGameDino"/"ellis.bmp");
    touch(root/"Data"/"SubGameDino"/"vel.bmp");
    touch(root/"Data"/"SubGameDino"/"tri.bmp");
    touch(root/"Data"/"SubGameDino"/"trex.bmp");
}
bool draw_has(const ActivityFrameOutput& output,const std::string& filename) {
    for(const auto& draw:output.draws) {
        if(draw.source_asset==filename)return true;
    }
    return false;
}
}

int main() {
    const auto root=fs::temp_directory_path()/
        "btb_original_native_dino_source_regression";
    fs::remove_all(root);
    retail_level(root,"Raptor","Easy");
    retail_level(root,"Tric","Hard");
    {
        auto driver=std::make_unique<DinoDriver>(root);
        auto* actual=driver.get();
        GameRoot game;
        assert(game.select_profile(1));
        game.globals().menu.selected_subgame=0;
        game.globals().menu.subgame_selection_origin=State::DinoChooserUpdate;
        game.globals().menu.source_variant=0;
        game.globals().menu.variant_selection_origin=State::DinoPregameUpdate;
        game.install(ActivityId::Dinosaur,std::move(driver));
        game.set_outer_state(State::DinoInit);

        const auto initialization=game.advance({});
        assert(initialization.kind==FrameKind::ActivityInitialized);
        assert(initialization.state_changed);
        assert(actual->initialized());
        assert(actual->runtime()->pieces().size()==1);
        assert(actual->runtime()->pieces()[0].source_rect.right==20);
        assert(game.globals().dispatcher.current_state==
               static_cast<int>(State::DinoRun));

        auto frame=game.advance({.pointer_x=15,.pointer_y=15,
            .click_pulse=true,.random_value=2,.pointer_button_held=true});
        assert(frame.kind==FrameKind::ActivityUpdated);
        assert(actual->runtime()->selected_piece()==0);
        assert(draw_has(frame.effects,
            "Data/SubGameDino/Dino/Raptor/Easy/Piece0.bmp"));
        assert(draw_has(frame.effects,
            "Data/SubGameDino/Dino/Raptor/Easy/background.bmp"));
        assert(draw_has(frame.effects,"Data/SubGameDino/Bob.bmp"));
        assert(draw_has(frame.effects,"Data/SubGameDino/ellis.bmp"));
        assert(!frame.effects.audio.empty()); // original startup 188

        // Mouse release is not a drop event in original 0x00409E00.
        frame=game.advance({.pointer_x=200,.pointer_y=200,
                            .pointer_button_held=false});
        assert(frame.kind==FrameKind::ActivityUpdated);
        assert(actual->runtime()->completed_piece_count()==0);
        assert(actual->runtime()->selected_piece()==0);
        assert(actual->runtime()->mode()==
               btb::dino::InteractionMode::Carrying);

        // Second click at the wrong location rejects, keeping the bone
        // cursor-owned; moving/releasing is not an additional attempt.
        frame=game.advance({.pointer_x=200,.pointer_y=200,
            .random_value=1,.click_pulse=true,
            .pointer_button_held=true});
        assert(frame.kind==FrameKind::ActivityUpdated);
        assert(actual->runtime()->mode()==
               btb::dino::InteractionMode::Carrying);
        frame=game.advance({.pointer_x=100,.pointer_y=100,
            .pointer_button_held=false});
        assert(actual->runtime()->mode()==
               btb::dino::InteractionMode::Carrying);

        // Another click near this bone's own target accepts placement.
        frame=game.advance({.pointer_x=100,.pointer_y=100,
            .random_value=2,.click_pulse=true,
            .pointer_button_held=true});
        assert(frame.kind==FrameKind::ActivityUpdated);
        assert(actual->runtime()->mode()==
               btb::dino::InteractionMode::FinalizeAcceptedDrop);
        assert(!actual->runtime()->complete());

        frame=game.advance({});
        assert(frame.kind==FrameKind::ActivityUpdated);
        assert(actual->runtime()->complete());
        assert(frame.effects.progress_writes.size()==1);
        assert(frame.effects.progress_writes[0].slot==
               btb::progress::Slot::DinoRaptor);
        assert(frame.effects.shared_completion_code==1);
        assert(game.globals().shared_completion_code==1);
        assert(draw_has(frame.effects,"data\\subgamedino\\vel.bmp"));
        assert(game.globals().player_progress[1].values[
            static_cast<std::size_t>(btb::progress::Slot::DinoRaptor)]==1);

        std::string error;
        assert(game.unload_current(error));
        assert(!actual->initialized());
    }
    {
        DinoDriver driver(root);
        RetailMenuState menu{};
        menu.selected_subgame=1;
        menu.subgame_selection_origin=State::DinoChooserUpdate;
        menu.source_variant=2;
        menu.variant_selection_origin=State::DinoPregameUpdate;
        driver.configure_menu_state(menu);
        std::string error;
        assert(driver.initialize(4,error));
        assert(driver.species()==btb::dino::Species::Triceratops);
        assert(driver.difficulty()==btb::dino::Difficulty::Hard);
        auto frame=driver.advance({});
        assert(draw_has(frame,
            "Data/SubGameDino/Dino/Tric/Hard/background.bmp"));
        assert(driver.unload(error));
        assert(!driver.initialized());
    }
    {
        fs::remove(root/"Data"/"SubGameDino"/"Dino"/"Raptor"/"Easy"/
                   "Piece0.bmp");
        DinoDriver driver(root);
        std::string error;
        assert(!driver.initialize(0,error));
        assert(!error.empty());
        assert(!driver.initialized());
    }
    fs::remove_all(root);
}

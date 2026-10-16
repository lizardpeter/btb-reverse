#include "btb/full_game_herding.hpp"
#include "btb/full_game_herding_event_simulation.hpp"
#include "btb/herding_native_pickles_motion.hpp"

#include <cstdio>
#include <cstdlib>
#undef assert
#define assert(expr) do { if(!(expr)) { \
    std::fprintf(stderr,"native Pets Corner: %s at %s:%d\n", \
        #expr,__FILE__,__LINE__);std::abort();} } while(false)

#include <filesystem>
#include <fstream>
#include <memory>

int main() {
    namespace fs=std::filesystem;
    namespace fg=btb::full_game;
    using btb::game_flow::State;
    const auto root=fs::temp_directory_path()/
        "btb_native_herding_firstpass_source";
    fs::remove_all(root);
    fs::create_directories(root/"Data"/"SubGame1");
    {
        std::ofstream f(root/"Data"/"SubGame1"/"herd.txt");
        // Original 0x00419600 consumes SIX fixed setup pairs before
        // parsing sentinel-separated 12/5/6/1 coordinate groups.
        f<<"0 0\n100 200\n250 300\n350 390\n"
         <<"90 180\n120 240\n";
        constexpr int nav[][2]={
            {64,100},{800,100},{1300,100},{1300,200},
            {1300,900},{1000,900},{300,900},{64,900},
            {64,700},{64,400},{64,200},{64,150}
        };
        for(const auto& p:nav)f<<p[0]<<" "<<p[1]<<"\n";
        f<<"-1 -1\n";
        for(int i=0;i<5;++i)
            f<<100+i*20<<" "<<200+i*12<<"\n";
        f<<"-1 -1\n";
        for(int i=0;i<6;++i)
            f<<300+i*40<<" "<<350+i*10<<"\n";
        f<<"-1 -1\n";
        f<<"777 999\n-1 -1\n";
    }
    std::srand(123);
    for(int difficulty=0;difficulty<3;++difficulty) {
        fg::GameRoot game;
        assert(game.select_profile(0));
        auto simulator=std::make_unique<fg::HerdingEventSimulation>(
            std::make_unique<fg::NativePicklesMotionSource>());
        auto driver=std::make_unique<fg::HerdingDriver>(
            root/"Data"/"SubGame1",std::move(simulator),difficulty);
        auto* live=driver.get();
        game.install(fg::ActivityId::Herding,std::move(driver));
        game.set_outer_state(State::HerdingInit);
        auto first=game.advance({});
        if(first.kind!=fg::FrameKind::ActivityInitialized)
            std::fprintf(stderr,"Pets Corner init: frame=%d error=%s state=%d\\n",
                static_cast<int>(first.kind),first.error.c_str(),
                game.globals().dispatcher.current_state);
        assert(first.kind==fg::FrameKind::ActivityInitialized);
        assert(game.globals().dispatcher.current_state==
               static_cast<int>(State::HerdingRun));
        auto scene=game.advance({});
        assert(scene.kind==fg::FrameKind::ActivityUpdated);
        assert(!scene.effects.fatal_error);
        assert(live->scene().entities.size()==
               static_cast<std::size_t>(15+3*difficulty+
                   (difficulty>0?1:0)));
        assert(live->scene().entities.front().x==350);
        assert(live->scene().entities.front().y==390);
        assert(live->scene().entities.front().entity_type()==
               btb::herding::EntityType::FarmerPickles);
        bool background=false,pickles=false,surround=false;
        for(const auto& draw:scene.effects.draws) {
            background|=draw.source_asset.find("PC_Background")!=
                std::string::npos;
            pickles|=draw.source_asset.find("Pickles")!=
                std::string::npos;
            surround|=draw.source_asset.find("Surround")!=
                std::string::npos;
        }
        assert(!scene.effects.draws.empty());
        assert(scene.effects.progress_writes.empty());
        scene=game.advance({.directional_input_bits=0x02});
        assert(scene.kind==fg::FrameKind::ActivityUpdated);
        assert(!scene.effects.fatal_error);
        assert(live->scene().entities.front().x==351);
        assert(live->scene().entities.front().x_float==351.5f);
        assert(live->scene().entities.front().y==390);
        scene=game.advance({.directional_input_bits=0x01});
        assert(live->scene().entities.front().x==350);
        assert(live->scene().entities.front().x_float==350.f);
        assert(scene.effects.progress_writes.empty());
        for(int i=0;i<100;++i) {
            scene=game.advance({});
            assert(scene.kind==fg::FrameKind::ActivityUpdated);
            assert(scene.effects.progress_writes.empty());
        }
        // No fabricated delivery: source animals require autonomous
        // movement/follower collision completion to award Pets Corner.
        assert(game.globals().player_progress[0].get(
            btb::progress::Slot::PetsCorner)==0);
        (void)background;(void)pickles;(void)surround;
    }
    fs::remove_all(root);
}

#include "btb/full_game_park_designer_native.hpp"
#include <algorithm>
#include <cassert>
#include <filesystem>
#include <fstream>
#include <memory>

namespace p=btb::park_designer;
namespace fs=std::filesystem;
using namespace btb::full_game;
using S=p::TrailingStateIndex;
static bool asset(const ActivityFrameOutput& f,std::string_view name) {
    return std::any_of(f.draws.begin(),f.draws.end(),[&](const Draw& d){return d.source_asset==name;});
}
int main(){
    auto root=fs::temp_directory_path()/"btb_full_game_park_native";
    fs::remove_all(root);fs::create_directories(root);
    {
        std::ofstream polygons(root/"boundareas.txt");
        for(int i=0;i<60;++i)polygons<<"0 0 40 0 40 40 0 40 -1 -1\n";
    }
    btb::retail::OriginalRetailRandom rng{7};
    auto backend=std::make_shared<RetailParkDesignerNativeService>(rng);
    ParkDesignerDriver d(root,root/"saves",backend,&rng);
    std::string error;
    assert(d.initialize(0,error));
    auto step=[&](int x=0,int y=0,bool click=false){
        ActivityFrameInput in{};in.pointer_x=x;in.pointer_y=y;in.click_pulse=click;
        in.pointer_position_valid=true;
        auto f=d.advance(in);
        assert(!f.fatal_error);
        return f;
    };
    auto f=step();
    assert(asset(f,p::kRetailParkBackground.summer));
    assert(std::any_of(f.audio.begin(),f.audio.end(),[](auto& a){
        return a.operation==AudioOperation::StartBackingTrack&&a.source_asset=="Data/music/dyp.wav";
    }));
    step();f=step();
    assert(asset(f,p::kRetailPaletteSheets[0]));
    // Actual toolbar hit regions and source decoration hotspot.
    step(200,440,true);
    assert(d.save().state(S::ActiveEditorMode)==2);
    step(263,435,true);
    f=step(350,180);
    assert(asset(f,p::kRetailDecorationSheets[0].summer));
    step(350,180,true);
    const auto& a=d.save().objects[300];
    assert(a.object_code==400&&a.left==282&&a.top==138);
    step(300,150,true); // pick within the fixture polygon
    assert(d.save().objects[300].enabled_or_active==2);
    step(400,200,true);
    assert(d.save().objects[300].left==332&&d.save().objects[300].enabled_or_active==1);
    step(500,440,true);
    step(350,440,true); // winter
    f=step();
    assert(d.save().state(S::Season)==1);
    assert(asset(f,p::kRetailDecorationSheets[0].winter));
    assert(asset(f,p::kRetailParkBackground.winter));
    f=step(410,440,true);
    assert(f.request_original_frame_print);
    f=step(550,440,true);
    assert(f.open_yes_no_confirmation&&f.yes_no_context==3);
    ActivityFrameInput decline{};decline.confirmation_resolved=true;
    f=d.advance(decline);
    assert(!f.fatal_error&&d.save().objects[300].object_code==400);
    // Persist the real editor state and exercise completion RNG.
    d.request_exit(true);
    f=step();assert(!f.save_and_unload);
    step();f=step();
    assert(f.save_and_unload&&f.shared_completion_code==7);
    assert(d.unload(error));
    assert(fs::file_size(root/"saves"/"dypdata1.txt")==p::kRetailSaveBytes);
    assert(d.initialize(0,error));
    assert(d.save().objects[300].left==332&&d.save().state(S::Season)==1);
    step();step();
    f=step(550,440,true);
    assert(f.open_yes_no_confirmation);
    ActivityFrameInput accept{};accept.confirmation_resolved=true;accept.confirmation_yes=true;
    f=d.advance(accept);assert(!f.fatal_error);
    assert(d.save().objects[300].object_code==-1);
    assert(d.save().state(S::NextDecorateRecord)==300);
    assert(d.unload(error));
    auto game=std::make_unique<GameRoot>();
    assert(game->select_profile(0));
    auto service=std::make_shared<RetailParkDesignerNativeService>(rng);
    auto driver=std::make_unique<ParkDesignerDriver>(root,root/"modal-saves",service,&rng);
    auto* live=driver.get();
    game->install(ActivityId::ParkDesigner,std::move(driver));
    game->set_outer_state(btb::game_flow::State::ParkDesignerInit);
    auto frame=[&](int x=0,int y=0,bool down=false,bool up=false,bool held=false){
        ActivityFrameInput in{};in.pointer_position_valid=true;in.pointer_x=x;in.pointer_y=y;
        in.click_pulse=down;in.pointer_release_pulse=up;in.pointer_button_held=held;
        auto g=game->advance(in);
        assert(g.kind!=FrameKind::ActivityFailed);
        return g;
    };
    assert(frame().kind==FrameKind::ActivityInitialized);
    frame();frame();frame();
    frame(200,440,true);frame(263,435,true);frame(350,180,true);
    assert(live->save().objects[300].object_code==400);
    frame(500,440,true);frame(550,440,true);
    auto modal=frame(250,280);
    assert(modal.kind==FrameKind::Intercept&&asset(modal.effects,"Data/ui/Deleteallobjects.bmp"));
    modal=frame(250,280,true,false,true);
    assert(asset(modal.effects,"Data/ui/playagain/payesdep.bmp"));
    frame(370,280,false,true);
    assert(game->globals().dispatcher.generic_yes_no_active);
    frame(370,280,true,false,true);frame(370,280,false,true);frame();
    assert(!game->globals().dispatcher.generic_yes_no_active&&live->save().objects[300].object_code==400);
    frame(590,440,true);
    assert(game->globals().dispatcher.leave_activity_active);
    modal=frame(370,280);
    assert(asset(modal.effects,"Data/ui/main/quitmain.bmp"));
    frame(370,280,true,false,true);frame(370,280,false,true);
    assert(!game->globals().dispatcher.leave_activity_active&&live->initialized());
    frame(590,440,true);frame(250,280,true,false,true);frame(250,280,false,true);
    frame();frame();auto completed=frame();
    assert(completed.effects.save_and_unload&&completed.effects.shared_completion_code==7);
    assert(!live->initialized());
    assert(game->globals().dispatcher.current_state==static_cast<int>(btb::game_flow::State::ActivitySelectSetup));
    assert(frame().kind==FrameKind::FrontEndUpdated);
    assert(game->globals().dispatcher.current_state==static_cast<int>(btb::game_flow::State::SharedMovieTransition));
    assert(fs::file_size(root/"modal-saves"/"dypdata1.txt")==p::kRetailSaveBytes);
    fs::remove_all(root);
}

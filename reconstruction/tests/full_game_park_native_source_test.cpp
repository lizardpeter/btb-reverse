#include "btb/full_game_park_designer_native.hpp"
#include <algorithm>
#include <cassert>
#include <filesystem>
#include <fstream>
#include <memory>
#include <sstream>

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
    auto print_profile=[](btb::profiles::ProfileInfo& profile,std::span<const Rect>& glyphs,std::string& error){
        static constexpr std::array<Rect,2> rects{{{0,0,12,35},{12,0,20,35}}};
        profile.name_length=2;profile.name_glyph_codes[0]=0;profile.name_glyph_codes[1]=1;
        profile.badge_index=2;glyphs=rects;error.clear();return true;
    };
    auto backend=std::make_shared<RetailParkDesignerNativeService>(rng,print_profile);
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
    assert(f.draws.size()>=4);
    const auto& label0=f.draws[f.draws.size()-4];
    const auto& label1=f.draws[f.draws.size()-3];
    const auto& badge=f.draws[f.draws.size()-2];
    const auto& bar=f.draws.back();
    assert(label0.source_asset=="Data/ui/enter name boxed font.bmp"&&label0.x==310&&label0.y==420);
    assert(label1.x==322&&label1.y==420);
    assert(label0.source_rectangle->bottom==36);
    assert(badge.source_asset=="Data/ui/fonttools.bmp"&&badge.x==220&&badge.y==420);
    assert(badge.source_rectangle->left==100&&badge.source_rectangle->bottom==45);
    assert(bar.source_asset=="Data/ui/printbar.bmp"&&bar.x==0&&bar.y==0);
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
    std::stringstream help_table;
    for(int g=0;g<9;++g)help_table<<"fixture_"<<g<<"\n-1 -1 -1 -1 -1\n";
    // Preserve original Park Designer index ordering, including its
    // view-only and palette-only voice regions.
    help_table<<"park_9\n"
        <<"579 420 625 465 482\n68 419 119 471 255\n478 416 532 471 256\n"
        <<"178 419 229 471 257\n123 419 174 471 254\n535 426 571 462 251\n"
        <<"234 400 424 467 250\n10 0 630 380 262\n433 408 469 459 253\n"
        <<"275 419 323 468 258\n332 419 382 468 259\n391 419 442 468 260\n"
        <<"535 426 571 462 261\n-1 -1 -1 -1 -1\nEND\n";
    assert(game->configure_contextual_help(help_table,error));
    auto service=std::make_shared<RetailParkDesignerNativeService>(rng,print_profile);
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
    // Help is modal: preserve the live editor scene and do not edit or
    // advance it while the physical help cursor explores original regions.
    auto has_voice=[](const ActivityFrameOutput& o,int id){
        return std::any_of(o.audio.begin(),o.audio.end(),[&](const Audio& a){
            return a.operation==AudioOperation::ManagedSoundId&&a.sound_id==id;
        });
    };
    frame(40,440,true,false,true);
    auto entered=frame(40,440,false,true);
    assert(has_voice(entered.effects,476));
    auto helping=frame(200,440);
    assert(helping.kind==FrameKind::Intercept&&has_voice(helping.effects,257));
    assert(asset(helping.effects,"Data/ui/help cursor.bmp"));
    assert(!has_voice(frame(200,440).effects,257));
    assert(has_voice(frame(300,200).effects,262));
    assert(frame(40,440,false,true).kind==FrameKind::Intercept);
    assert(frame(0,0).kind==FrameKind::ActivityUpdated);
    assert(live->save().objects[300].object_code==400);
    for(int i=0;i<50;++i)frame();
    frame(500,440,true);
    frame(40,440,true,false,true);frame(40,440,false,true);
    assert(!has_voice(frame(240,440).effects,250)); // palette not in View mode
    assert(has_voice(frame(350,440).effects,259)); // winter control
    frame(40,440,false,true);
    assert(frame().kind==FrameKind::ActivityUpdated);
    frame(550,440,true);
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

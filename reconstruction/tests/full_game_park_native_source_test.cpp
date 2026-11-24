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
    assert(d.unload(error));fs::remove_all(root);
}

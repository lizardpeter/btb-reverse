// Installed-data Park Designer editor, seasonal art, and profile persistence.
#include "btb/full_game_park_designer_native.hpp"
#include "btb/park_designer_placement.hpp"
#include "retail_activity_audit.hpp"
#include <memory>
namespace {
using namespace btb::retail_probe;
namespace p=btb::park_designer;
using S=p::TrailingStateIndex;
void play(const fs::path& root,const fs::path& save,int profile){
    Audit audit{fg::OriginalAssetResolver(root)};VoiceModel voices;
    std::array<fg::Rect,191> font{};std::ifstream file(audit.resolve("loaddata/fontdatabrown.txt"));
    for(auto& r:font)check(bool(file>>r.left>>r.top>>r.right>>r.bottom),"invalid original glyph table");
    auto game=std::make_unique<fg::GameRoot>();check(game->select_profile(profile),"profile rejected");
    btb::retail::OriginalRetailRandom rng(37+profile);
    auto service=std::make_shared<fg::RetailParkDesignerNativeService>(rng,[&](btb::profiles::ProfileInfo& info,std::span<const fg::Rect>& glyphs,std::string& error){info.name_length=2;info.name_glyph_codes[0]=65;info.name_glyph_codes[1]=66;info.badge_index=profile;glyphs=font;error.clear();return true;});
    auto driver=std::make_unique<fg::ParkDesignerDriver>(audit.resolve("Data/SubGameDYP/boundareas.txt").parent_path(),save,service,&rng);
    auto* live=driver.get();game->install(fg::ActivityId::ParkDesigner,std::move(driver));game->set_outer_state(State::ParkDesignerInit);
    check(game->advance({}).kind==fg::FrameKind::ActivityInitialized,"Park init failed");
    auto tick=[&](int x=0,int y=0,bool down=false,bool up=false){
        fg::ActivityFrameInput in{};voices.sample(in);in.pointer_position_valid=true;in.pointer_x=x;in.pointer_y=y;in.click_pulse=down;in.pointer_button_held=down;in.pointer_release_pulse=up;
        auto f=game->advance(in);check(f.kind!=fg::FrameKind::ActivityFailed,"Park update: "+f.error);audit.observe(f.effects);voices.observe(f.effects);return f.effects;
    };
    for(int i=0;i<20;++i)tick();
    tick(90,440,true);tick(263,435,true);tick(198,255,true);
    check(live->save().objects[0].object_code==100,"pond placement failed");
    for(int i=0;i<80;++i)tick();
    tick(297,435,true);tick(176,177,true);
    check(live->save().objects[2].object_code==7,"fountain placement failed");
    for(int i=0;i<80;++i)tick();
    tick(263,435,true);tick(160,279,true);
    check(live->save().objects[3].object_code==1,"pond accessory placement failed");
    tick(145,440,true);tick(331,435,true);tick(463,236,true);
    check(live->save().objects[200].object_code==300,"bandstand base placement failed");
    for(int i=0;i<80;++i)tick();
    for(auto a:p::kRetailBandstandPillarAnchors){tick(263,435,true);tick(400+a.x+8,200+a.y-32,true);}
    check(live->save().state(S::PondLinkedCount)==4,"four pillar placement failed");
    for(int i=0;i<80;++i)tick();
    tick(365,435,true);tick(461,153,true);
    check(live->save().objects[206].object_code==300,"roof placement failed");
    tick(200,440,true);tick(263,435,true);tick(350,105,true);
    check(live->save().objects[300].object_code==400,"decoration placement failed");
    tick(500,440,true);tick(350,440,true);for(int i=0;i<180;++i)tick();
    check(live->save().state(S::Season)==1,"winter selection failed");
    check(tick(410,440,true).request_original_frame_print,"print request missing");
    tick(550,440,true);check(game->globals().dispatcher.generic_yes_no_active,"delete confirmation missing");
    tick(370,280,true);tick(370,280,false,true);tick();
    check(!game->globals().dispatcher.generic_yes_no_active&&live->save().objects[300].object_code==400,"declined deletion mutated scene");
    tick(590,440,true);check(game->globals().dispatcher.leave_activity_active,"Back confirmation missing");
    tick(250,280,true);tick(250,280,false,true);
    bool complete=false;for(int i=0;i<100&&live->initialized();++i){const auto f=tick();if(f.save_and_unload){check(f.shared_completion_code==7,"wrong park completion code");complete=true;}}
    check(complete&&!live->initialized()&&game->globals().player_progress[profile].get(Slot::ParkDesigner)==1,"Park completion failed");
    const auto path=save/("dypdata"+std::to_string(profile+1)+".txt");check(fs::file_size(path)==p::kRetailSaveBytes,"wrong Park save size");
    std::string error;check(live->initialize(profile,error),"Park reload: "+error);
    const auto& restored=live->save();check(restored.objects[0].object_code==100&&restored.objects[2].object_code==7&&restored.objects[3].object_code==1&&restored.objects[206].object_code==300&&restored.objects[300].object_code==400&&restored.state(S::Season)==1,"Park reload mismatch");
    check(live->unload(error),"Park cleanup: "+error);
    std::cout<<"park profile="<<profile<<" pond=1 fountain=1 accessory=1 bandstand=1 decoration=1 print=1 persisted=1";audit.report();
}
}
int main(int argc,char** argv){if(argc!=3){std::cerr<<"usage: btb_retail_park_playthrough <installed-root> <new-save-dir>\n";return 2;}try{check(!fs::exists(argv[2]),"save directory must not exist");for(int p=0;p<5;++p)play(argv[1],argv[2],p);return 0;}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}

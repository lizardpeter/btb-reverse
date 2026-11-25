// Real installed-data Fireworks show, with an explicit virtual playback clock.
#include "btb/full_game_fireworks.hpp"
#include "btb/fireworks_show_native.hpp"
#include "btb/bink32_native_video_stream.hpp"
#include "btb/bink32_audio_movie.hpp"
#include "retail_activity_audit.hpp"
#include <memory>
namespace {
using namespace btb::retail_probe;
namespace fw=btb::fireworks;
namespace bk=btb::bink32_recovered;
std::vector<std::uint8_t> read_movie(const fs::path& p){
    std::ifstream f(p,std::ios::binary|std::ios::ate);const auto n=f?f.tellg():std::streampos(-1);
    check(n>0,"missing movie "+p.string());std::vector<std::uint8_t> bytes(static_cast<std::size_t>(n));f.seekg(0);
    check(bool(f.read(reinterpret_cast<char*>(bytes.data()),static_cast<std::streamsize>(bytes.size()))),"movie read failed");return bytes;
}
struct Bank {
    std::array<std::optional<bk::BinkNativeVideoStream>,23> streams{};
    std::array<std::int64_t,23> starts{};
    std::array<bool,23> started{};
    std::array<unsigned,23> decoded{};
    unsigned launches{},restarts{},steps{};
    std::int64_t now{};
    bool open(Audit& audit,std::string& error){
        for(std::size_t i=0;i<streams.size();++i){
            auto bytes=read_movie(audit.resolve("Data/SubGameFirework/graph/"+std::string(fw::kOriginalFireworkBinkNames[i])));
            check(bytes.size()<=fw::kOriginalFireworkReadCapacity,"oversize Fireworks clip");
            check(bk::validate_bink1_silent_movie_audio(bytes,error),"invalid Fireworks audio: "+error);
            streams[i]=bk::BinkNativeVideoStream::open(std::move(bytes));check(bool(streams[i]),"Fireworks bank decoder open failed");
        }started.fill(false);error.clear();return true;
    }
    bool advance(const fw::Sequence& seq,fw::RetailShowRuntime& runtime,fw::ShowFrameInput in,fw::ShowFrameOutput& out,std::string& error){
        now=std::int64_t(in.elapsed_ticks)*10000;
        const bool ok=fw::advance_native_fireworks_show(seq,runtime,in,
            [&](std::size_t i,int x,int y,bool& finished,std::string& e){
                ++steps;auto& stream=*streams.at(i);finished=stream.at_end();if(finished)return true;
                if(!started[i]){started[i]=true;starts[i]=now;}
                const auto due=stream.frame_timestamp_us(stream.frame_number()+1);
                check(bool(due),"invalid Fireworks time");
                if(now-starts[i]<std::int64_t(*due))return true;
                if(!stream.decode_next()){e="actual Fireworks decode failed";return false;}
                const auto* frame=stream.current_frame();const auto size=fw::original_firework_surface_size(i);
                check(frame&&frame->width<=unsigned(size.width)&&frame->height<=unsigned(size.height),"invalid original show frame geometry");
                check(x>=0&&x+size.width<=640&&y>=0&&y+size.height<=540,"invalid show overlay placement");
                ++decoded[i];return true;
            },[&](std::size_t i,std::string&){streams.at(i)->rewind();started[i]=false;++restarts;return true;},out,error);
        launches+=out.launched_events.size();return ok;
    }
    void close(){for(auto& s:streams)s.reset();}
};
void play(const fs::path& root,const fs::path& save,int profile){
    Audit audit{fg::OriginalAssetResolver(root)};VoiceModel voices;Bank bank;
    std::optional<bk::BinkNativeVideoStream> pre;unsigned pre_frames=0;
    std::array<fg::Rect,191> font{};std::ifstream fontfile(audit.resolve("loaddata/fontdatabrown.txt"));
    for(auto& r:font)check(bool(fontfile>>r.left>>r.top>>r.right>>r.bottom),"invalid original glyph table");
    std::ifstream layoutfile(audit.resolve("Data/SubGameFirework/graph/fireworks.txt")),palettefile(audit.resolve("Data/SubGameFirework/graph/fireworkboxes.txt"));
    const auto layout=fw::parse_layout(layoutfile,palettefile);
    auto game=std::make_unique<fg::GameRoot>();check(game->select_profile(profile),"profile rejected");
    auto& info=game->globals().profile_metadata[profile];info.name_length=2;info.name_glyph_codes[0]=65;info.name_glyph_codes[1]=66;info.badge_index=profile;
    btb::retail::OriginalRetailRandom rng(19+profile);
    auto driver=std::make_unique<fg::FireworksDriver>(audit.resolve("Data/SubGameFirework/graph/fireworks.txt").parent_path().parent_path(),save,
        [&](const fs::path&,std::string& e){pre=bk::BinkNativeVideoStream::open(read_movie(audit.resolve("Data/movies/fireworkcomplete.bik")));e.clear();return bool(pre);},
        [&](){pre.reset();},[&](const fs::path&,std::string& e){return bank.open(audit,e);},[&](){bank.close();},
        [&](const auto& seq,auto& runtime,auto in,auto& out,std::string& e){return bank.advance(seq,runtime,in,out,e);},&rng,
        [&](int player,btb::profiles::ProfileInfo& p,std::span<const fg::Rect>& glyphs,std::string& e){check(player==profile,"certificate profile mismatch");p=info;glyphs=font;e.clear();return true;});
    auto* live=driver.get();game->install(fg::ActivityId::Fireworks,std::move(driver));game->set_outer_state(State::FireworksInit);
    check(game->advance({}).kind==fg::FrameKind::ActivityInitialized,"Fireworks init failed");
    int time=40000;
    auto tick=[&](int x=0,int y=0,bool click=false){
        fg::ActivityFrameInput input{};voices.sample(input);input.elapsed_centiseconds=time;time+=2;
        input.pointer_x=x;input.pointer_y=y;input.pointer_position_valid=true;input.click_pulse=click;
        input.fireworks_crowd_end_sound_playing=input.managed_sound_playing&&voices.id==349;
        if(pre){if(!pre->at_end()){check(pre->decode_next(),"pre-show actual Bink decode failed");++pre_frames;}else{input.original_global_movie_finished=true;pre.reset();}}
        auto step=game->advance(input);check(step.kind==fg::FrameKind::ActivityUpdated,"Fireworks update: "+step.error);
        audit.observe(step.effects);voices.observe(step.effects);return step.effects;
    };
    tick();
    for(int i=0;i<18;++i){
        const int row=i/6,col=i%6,type=row<2?(col+row*2+profile)%8:8+(col+profile)%4;
        const auto p=layout.palette[type];tick((p.left+p.right)/2,(p.top+p.bottom)/2,true);
        const auto q=layout.placement_runtime[i];tick((q.left+q.right)/2,(q.top+q.bottom)/2,true);tick();
        check(live->sequence().at(row,col)==fw::FireworkType(type),"original placement mismatch at cell "+std::to_string(i));
    }
    check(live->sequence().occupied_count()==18,"original full sequence not achieved");
    const auto expected=live->sequence().retail_grid();
    tick(320,440,true);
    for(int i=0;i<4000&&live->editor().internal_state!=fw::InternalState::Certificate;++i)tick();
    check(live->editor().internal_state==fw::InternalState::Certificate,"authored show failed to complete");
    check(bank.launches==18&&pre_frames>0,"show did not schedule all 18 cells");
    tick(320,440);check(tick(320,440,true).request_original_frame_print,"certificate print request missing");
    check(game->globals().player_progress[profile].get(static_cast<Slot>(63))==1,"Fireworks progress missing");
    tick(590,440,true);
    check(game->globals().dispatcher.current_state==int(State::PlayAgainYesNoSetup)&&game->globals().dispatcher.saved_state==int(State::FireworksRun)&&game->globals().menu.replay_class==2&&live->initialized()&&live->sequence().retail_grid()==expected,"Fireworks retained replay handoff failed");
    std::string error;check(game->unload_current(error),"Fireworks unload: "+error);
    const auto file=save/std::string(fw::retail_grid_filename(profile));check(fs::file_size(file)==72,"wrong saved grid bytes");
    std::ifstream saved(file,std::ios::binary);check(fw::read_retail_grid(saved)==expected,"saved Fireworks grid differs");
    unsigned total=0;for(auto n:bank.decoded)total+=n;
    std::cout<<"fireworks profile="<<profile<<" placed=18 launched="<<bank.launches<<" pre_frames="<<pre_frames<<" show_frames="<<total<<" restarts="<<bank.restarts<<" print=1 persisted=1";audit.report();
}
}
int main(int argc,char** argv){if(argc!=3){std::cerr<<"usage: btb_retail_fireworks_playthrough <installed-root> <new-save-dir>\n";return 2;}try{check(!fs::exists(argv[2]),"save directory must not exist");for(int p=0;p<5;++p)play(argv[1],argv[2],p);return 0;}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}

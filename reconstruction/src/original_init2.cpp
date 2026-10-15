#include "btb/original_init2.hpp"
#include <algorithm>
#include <array>
#include <limits>
#include <stdexcept>
namespace btb::original_init2 {
namespace {
constexpr U32 kMagenta=0x00ff00ffU;
void surface_add(std::vector<SpriteSlot>& slots,U32 slot,U32 name,U32 handle,
                 bool keyed,bool require_handle) {
 if(require_handle&&handle==0)
  throw std::runtime_error("retail bitmap surface absent");
 slots.push_back({slot,handle,name,keyed});
}
void squirrel_surface(SquirrelInit& s,SquirrelIO& io,U32 slot,U32 name,bool key) {
 const U32 handle=io.load_registered_bitmap(slot,name);
 surface_add(s.sprites,slot,name,handle,key,true);
 if(key)io.key_surface(slot,handle,kMagenta);
}
void dino_surface(DinoInit& s,DinoIO& io,U32 slot,U32 name,bool key){
 const U32 handle=io.load_registered_bitmap(slot,name);
 surface_add(s.sprites,slot,name,handle,key,true);
 if(key)io.key_surface(slot,handle,kMagenta);
}
// Original shipped resource path symbol addresses recovered from 00425160.
struct BitmapSpec {U32 dest,source;bool keyed;};
constexpr std::array<BitmapSpec,15> kSquirrelFixed{{
 {0x005150a0U,0x00446c60U,true}, // all.bmp
 {0x005150a4U,0x00446c38U,true}, // allconveyor.bmp
 {0x005150a8U,0x00446c18U,true}, // hook.bmp
 {0x005150b4U,0x00446bf4U,true}, // body_01.bmp
 {0x005150b8U,0x00446bc8U,true}, // loftyarms...
 {0x005150bcU,0x00446ba0U,true}, // squriel_1_8...
 {0x005150acU,0x00446b80U,true}, // eyes.bmp
 {0x005150b0U,0x00446b60U,true}, // mouth.bmp
 {0x00515068U,0x00446b40U,true}, // top.bmp
 {0x0051506cU,0x00446b20U,true}, // left.bmp
 {0x00515070U,0x00446b00U,true}, // right.bmp
 {0x00515074U,0x00446ae0U,true}, // bottom.bmp
 {0x005150c0U,0x00446abcU,true}, // conveyor.bmp
 {0x005150c4U,0x00446a94U,true}, // LOFTYmoves8...
 {0x005150c8U,0x00446a70U,true}  // nutpile.bmp
}};
constexpr std::array<U32,3> kSquirrelBackgrounds{
 0x00446cc0U,0x00446ca0U,0x00446c80U
};
constexpr std::array<BitmapSpec,5> kDinoFixed{{
 {0x004fc2acU,0x0043f9acU,true}, // bob.bmp
 {0x004fc434U,0x0043f990U,true}, // ellis.bmp
 {0x00508ad0U,0x0043f978U,true}, // UI printbar.bmp
 {0x0050a5c4U,0x0043f954U,true}, // certificate print b
 {0x0050a5c8U,0x0043f92cU,true}  // certificate print detail
}};
}
void initialize_squirrel(SquirrelInit& s,SquirrelIO& io){
 if(s.profile<0||s.profile>=static_cast<I32>(kSquirrelBackgrounds.size()))
  throw std::out_of_range("original Squirrel difficulty profile");
 const I32 profile=s.profile;
 // 00425160 clears the counters before invoking LoadSquirrelData.
 s=SquirrelInit{};
 s.profile=profile;s.active_level=profile;
 s.unload_installed=0;
 s.data=io.load_original_data();
 if(s.data.animation_step_delay<=0)
  throw std::runtime_error("Squirrel shipped motion cadence invalid");
 io.register_activity_unload();s.unload_installed=1;
 s.current_actor=-1;s.hover_actor=-1;s.selection=-1;
 s.invalid_slots.fill(-1);s.invalid_more.fill(-1);
 // DAT_00514F0C .. DAT_00514F38: 12 original RNG calls, each modulo three.
 for(auto& cell:s.random_cells)cell=static_cast<I32>(io.next_original_rand()%3U);
 // Recovered assignments DAT_00514F1C=DAT_00514F18,
 // DAT_00514F2C=DAT_00514F28.
 s.random_cells[4]=s.random_cells[3];
 s.random_cells[8]=s.random_cells[7];
 s.run_x=io.original_run_x_base()-s.data.initial_horizontal_alignment_offset;
 s.run_y=io.original_run_y_base()+io.original_run_y_offset(s.random_cells[0]);
 s.current_mode=3+static_cast<I32>(profile!=0);
 s.cursor_x=620;s.cursor_y=480;
 s.first_frame_x=s.run_x;s.first_frame_y=s.run_y;
 // original screen-specific level1/2/3 surface is not color keyed.
 squirrel_surface(s,io,0x0051509cU,kSquirrelBackgrounds[static_cast<std::size_t>(profile)],false);
 for(const auto& spec:kSquirrelFixed)
  squirrel_surface(s,io,spec.dest,spec.source,spec.keyed);
 s.selection_overlay.fill(0);
 io.generate_original_conveyor_choices();
 s.intro_state=0;s.music_start_pending=1;
 io.stop_activity_music();
 s.initialized=true;
}
void initialize_dino(DinoInit& s,DinoIO& io){
 if(s.species<0||s.species>2||s.difficulty<0||s.difficulty>2)
  throw std::out_of_range("original Dino species/difficulty");
 const I32 species=s.species,difficulty=s.difficulty,profile=s.profile;
 const I32 volume=s.global_volume;
 const I32 level_index=profile+species*3;
 if(level_index<0||level_index>8||profile<0||profile>2)
  throw std::out_of_range("original Dino source level index");
 s=DinoInit{};s.species=species;s.difficulty=difficulty;
 s.profile=profile;s.level_index=level_index;
 s.global_volume=volume;
 s.cursor_tolerance=(level_index==2)?9:20;
 s.level=io.load_original_level(level_index);
 const std::size_t n=s.level.piece_count();
 if(n==0||n>64||s.level.target_positions.size()!=n||
    s.level.start_positions.size()!=n||s.level.extra_positions.empty())
  throw std::runtime_error("Dino original parsed level structure invalid");
 s.piece_count=static_cast<I32>(n);
 io.register_activity_unload();
 for(const auto& spec:kDinoFixed)
  dino_surface(s,io,spec.dest,spec.source,spec.keyed);
 const U32 bg=io.load_original_background_bitmap(0x004fc9e8U,level_index);
 surface_add(s.sprites,0x004fc9e8U,0x0043e15cU,bg,false,true);
 for(std::size_t i=0;i<n;++i){
  const U32 guest_slot=0x004fc9ecU+static_cast<U32>(4*i);
  const U32 handle=io.load_original_piece_bitmap(guest_slot,static_cast<I32>(i));
  surface_add(s.sprites,guest_slot,0x0043f860U,handle,true,true);
  io.key_surface(guest_slot,handle,kMagenta);
  const I32 perm=s.level.piece_permutation[i];
  if(perm<0||static_cast<std::size_t>(perm)>=n)
   throw std::out_of_range("Dino original piece permutation");
  const auto& target=s.level.target_positions[static_cast<std::size_t>(perm)];
  const auto& initial=s.level.start_positions[i];
  const auto dimensions=io.query_original_surface_size(handle);
  if(dimensions[0]<=0||dimensions[1]<=0)
   throw std::runtime_error("Dino IDirectDrawSurface original dimensions");
  s.pieces.push_back({target.x,target.y,initial.x,initial.y,
                      perm,1,0,dimensions[0],dimensions[1],handle});
 }
 // Source 0040A670 initializes DAT_004FC400/404 from the first
 // *extra* position, NOT the final target or a permutation slot.
 const auto& anchor=s.level.extra_positions.front();
 s.first_anchor_x=anchor.x;s.first_anchor_y=anchor.y;
 s.current_anchor_x=anchor.x;s.current_anchor_y=anchor.y;
 s.anchor_x=static_cast<float>(anchor.x);
 s.anchor_y=static_cast<float>(anchor.y);
 s.pieces_completed=0;s.gameplay_phase=0;
 s.animation_mode=4;s.movement_mode=1;
 s.movement_alpha=0.5f;s.music_pending=1;
 io.apply_global_game_volume((s.global_volume-100)*0x2a);
 s.initialized=true;
}
} // namespace btb::original_init2

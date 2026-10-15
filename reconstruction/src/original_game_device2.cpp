#include "btb/original_game_device2.hpp"
#include "btb/original_ftol1.hpp"
#include "btb/original_leaf_batch_25.hpp"
#include "btb/maze_input.hpp"
#include <bit>
#include <cmath>
#include <stdexcept>
namespace btb::original_game_device2 {
namespace {
struct Bitmap {U32 slot,registration;const char* path;};
constexpr std::array<Bitmap,12> images{{
 {0x50ab18,0x50ab18,R"(Data\SubGameFirework\graph\bobsprite1_8bit.bmp)"},
 {0x50ab1c,0x50ab1c,R"(Data\SubGameFirework\graph\wendysprite8_bit.bmp)"},
 {0x50aaa8,0x50aaa8,R"(Data\SubGameFirework\graph\middle.bmp)"},
 {0x50ab08,0x50ab08,R"(Data\SubGameFirework\graph\bottom.bmp)"},
 {0x50a49c,0x50a49c,R"(Data\SubGameFirework\graph\fireworkdeleteallred.bmp)"},
 {0x50a4a0,0x50a49c,R"(Data\SubGameFirework\graph\fireworkdeletealldep.bmp)"},
 {0x50a4a4,0x50a4a4,R"(Data\SubGameFirework\graph\fireworkdeletered.bmp)"},
 {0x50a4a8,0x50a4a8,R"(Data\SubGameFirework\graph\fireworkdeletedep.bmp)"},
 {0x50a4ac,0x50a4ac,R"(Data\SubGameFirework\graph\fireworkplayred.bmp)"},
 {0x50a4b0,0x50a4b0,R"(Data\SubGameFirework\graph\fireworkplaydep.bmp)"},
 {0x50a5c4,0x50a5c4,R"(Data\SubGameFirework\certprint.bmp)"},
 {0x50a5c8,0x50a5c8,R"(Data\SubGameFirework\certprintdep.bmp)"}
}};
I32 truncate(float value){const auto extended=original_float6::double_to_extended(std::bit_cast<std::uint64_t>(static_cast<double>(value)));return std::bit_cast<I32>(static_cast<U32>(original_ftol1::truncate_integer(extended)));}
}
void load_fireworks(Fireworks& state,Handle draw,BitmapServices& io){
 const auto load=[&](std::size_t i){const auto& image=images[i];state.surfaces[i]=io.load(draw,image.path,0,0);const std::size_t registration=i==5?4:i;io.register_surface(image.registration,state.surfaces[registration],image.path);};
 const auto key=[&](std::size_t i){io.mark_keyed(images[i].slot);io.colour_key(state.surfaces[i],0xff00ff);};
 load(0);load(1);state.aliases={state.surfaces[0],state.surfaces[1]};key(0);key(1);
 load(2);load(3);key(2);key(3);
 for(std::size_t i=4;i<10;++i)load(i);for(std::size_t i=4;i<10;++i)key(i);
 load(10);load(11);key(10);key(11);
}
void move_pilchard(const Movement& state,I32 index,Actor& actor,I32 speed,std::span<const std::pair<I32,I32>> nodes){
 const auto node=[&]()->const std::pair<I32,I32>&{const auto slot=std::int64_t(state.screen)*30+index;if(slot<0 || static_cast<std::uint64_t>(slot)>=nodes.size())throw std::out_of_range("original Pilchard node");return nodes[static_cast<std::size_t>(slot)];};
 if(state.obstruction_a>=0){
  if(state.obstruction_a>2){
   // Retail tests obstruction_b==4 here, not obstruction_a==4.
   if(state.obstruction_a==3 || state.obstruction_b==4)actor.y=static_cast<float>(static_cast<long double>(actor.y)+25.0L);
   else if(state.obstruction_a==5)actor.y=static_cast<float>(node().second);
  }return;
 }
 if(state.obstruction_b>=0){
  if(state.obstruction_b>2 && actor.direction==0){
   if(state.obstruction_b==3 || state.obstruction_b==4)actor.y=static_cast<float>(static_cast<long double>(actor.y)-25.0L);
   else if(state.obstruction_b==5)actor.y=static_cast<float>(node().second);
  }return;
 }
 if(!state.moving || actor.mode<11 || actor.mode>13)return;
 const auto [x,y]=node();
 (void)original_leaf::distance_between_integer_points(truncate(actor.x),truncate(actor.y),x,y);
 const I32 degrees=maze::angle_between_integer_points_degrees({truncate(actor.x),truncate(actor.y)},{x,y});
 const long double radians=static_cast<long double>(degrees)*static_cast<long double>(0.017453530803322792f),step=static_cast<float>(speed);
 actor.y=static_cast<float>(static_cast<long double>(actor.y)-std::cos(radians)*step*10.0L);
 actor.x=static_cast<float>(static_cast<long double>(actor.x)+std::sin(radians)*step*10.0L);
 if(actor.direction==1 && static_cast<float>(x)<actor.x)actor.x=static_cast<float>(x);
 if(actor.direction==3 && actor.x<static_cast<float>(x))actor.x=static_cast<float>(x);
}
}

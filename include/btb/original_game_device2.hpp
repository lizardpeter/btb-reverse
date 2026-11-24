#pragma once
#include "btb/original_imports3.hpp"
#include <cstddef>
#include <span>
#include <utility>
namespace btb::original_game_device2 {
using namespace original_imports3;
struct Fireworks {std::array<Handle,12> surfaces{};std::array<Handle,2> aliases{};};
class BitmapServices {
public:
 virtual ~BitmapServices()=default;
 virtual Handle load(Handle draw,const char* path,U32 width,U32 height)=0;
 virtual void register_surface(U32 original_slot,Handle& surface,const char* path)=0;
 virtual void mark_keyed(U32 original_slot)=0;
 virtual void colour_key(Handle surface,U32 colour)=0;
};
void load_fireworks(Fireworks&,Handle draw,BitmapServices&); //004112A0
struct Actor {float x{},y{};std::array<U32,3> unused{};I32 direction{};U32 unused_18{};I32 mode{};};
static_assert(sizeof(Actor)==32 && offsetof(Actor,direction)==20 && offsetof(Actor,mode)==28);
struct Movement {I32 obstruction_a{-1},obstruction_b{-1},screen{};bool moving{};};
void move_pilchard(const Movement&,I32 node,Actor&,I32 speed,std::span<const std::pair<I32,I32>>); //004232B0
}

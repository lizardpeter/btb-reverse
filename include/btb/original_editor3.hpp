#pragma once
#include "btb/original_crt17.hpp"
#include "btb/squirrel_runtime.hpp"
#include <array>
#include <span>
namespace btb::original_editor3 {
using I32=std::int32_t;using U32=std::uint32_t;
struct Region {I32 left{},top{},right{},bottom{},value{-1};};
struct FireworksState {
    I32 mode{},x{},y{},hot{},drag_type{-1},bob{},wendy{},bob_type{},wendy_type{};
    std::array<I32,18> grid{};
    friend bool operator==(const FireworksState&,const FireworksState&)=default;
};
class FireworksServices {
public:
    virtual ~FireworksServices()=default;
    virtual void set_cursor(I32 firework_type)=0;
    virtual void play_sound(I32 id,I32 priority,I32 arbitration)=0;
};
// 004126F0: complete ordered hit-test, cursor/actor/pointer mutations.
I32 hit_test_fireworks(FireworksState&,std::span<const Region>,U32& seed,FireworksServices&);
struct SquirrelState {
    I32 difficulty{},row{},phase{},target{},variant{-1},outer{},timer{},step{};
    std::array<I32,3> stages{};
    std::array<std::array<I32,4>,3> plan{},placed{};
    std::array<I32,4> correct{},piece_ids{},motion{};
    std::array<std::array<I32,2>,4> pairs{};
    friend bool operator==(const SquirrelState&,const SquirrelState&)=default;
};
// 004259D0 / 00426F30. Actual MSVC rand state and original table order.
void select_squirrel_motion(SquirrelState&);
void generate_squirrel_choices(SquirrelState&,U32& seed);
// Connect original selector output to the existing loaded-motion consumer.
squirrel::RunAssemblyMotionKeys original_motion_keys(SquirrelState&);
void begin_original_run_assembly(SquirrelState&,squirrel::RunAssemblyAnimationRuntime&,const squirrel::Data&);
bool verify_3_original_editor_functions();
}

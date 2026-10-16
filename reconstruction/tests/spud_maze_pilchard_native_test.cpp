#include "btb/spud_maze_pilchard_native.hpp"

#include <cstdio>
#include <cstdlib>
#undef assert
#define assert(expr) do { if(!(expr)) { \
    std::fprintf(stderr,"Pilchard source: %s at %s:%d\\n", \
        #expr,__FILE__,__LINE__);std::abort();} } while(false)

int main() {
    namespace sm=btb::spud_maze;
    namespace mb=btb::maze_spud_batch;

    sm::Data source{};
    source.spud_spawn_times={-1,1000,200};
    source.spud_speed_regular={1,1,2};
    source.spud_speed_package={1,1,2};
    sm::Node entrance{};
    entrance.index=0;entrance.x=100;entrance.y=200;
    entrance.links={-1,2,-1,-1};
    sm::Node exit{};
    exit.index=2;exit.x=100;exit.y=250;
    exit.links={-1,-1,-1,0};
    source.screens[0]={entrance,exit};
    std::uint32_t roll{};
    auto rand=[&]() -> std::uint32_t {return roll++;};

    {
        sm::PilchardNative easy{};
        mb::CollisionState hammer{};hammer.hammer_held=true;
        for(int tick=0;tick<1200;++tick) {
            const auto result=sm::update_pilchard_native(
                easy,hammer,source,0,{0,1,100.f,200.f,false},rand);
            assert(!result.actor_visible);
            assert(easy.phase==0);
            assert(hammer.hammer_held);
        }
    }
    {
        sm::PilchardNative active{};
        mb::CollisionState hammer{};
        hammer.hammer_held=true;
        auto begin=sm::update_pilchard_native(
            active,hammer,source,2,{0,1,400.f,200.f,false},rand);
        assert(active.phase==0);
        active.spawn_wait=1;
        begin=sm::update_pilchard_native(
            active,hammer,source,2,{0,1,400.f,200.f,false},rand);
        assert(active.phase==1);
        begin=sm::update_pilchard_native(
            active,hammer,source,2,{0,1,400.f,200.f,false},rand);
        assert(active.phase==2);
        assert(active.screen==0);
        assert(active.current_node==0);
        assert(active.x==100.f&&active.y==200.f);
        assert(begin.actor_visible);
        assert(!begin.sound_and_hammer.empty());
        assert(begin.sound_and_hammer[0].op==mb::Op::PlayManagedSound);

        // Original Pilchard collision is strictly less than 60 px,
        // cannot steal during repair or when Bob Y >= 360.
        active.phase=5;active.route={0,2};active.route_index=1;
        active.x=100.f;active.y=200.f;
        auto safe=sm::update_pilchard_native(
            active,hammer,source,2,{0,1,101.f,360.f,false},rand);
        assert(!safe.hammer_dropped);
        assert(hammer.hammer_held);
        safe=sm::update_pilchard_native(
            active,hammer,source,2,{0,1,101.f,201.f,true},rand);
        assert(!safe.hammer_dropped);
        assert(hammer.hammer_held);

        active.x=100.f;active.y=200.f;active.phase=5;
        auto impact=sm::update_pilchard_native(
            active,hammer,source,2,{0,1,101.f,202.f,false},rand);
        assert(impact.hammer_dropped);
        assert(!hammer.hammer_held);
        assert(hammer.hammer_dropped);
        assert(hammer.cooldown==500);
        assert(hammer.hammer.screen==0);
        assert(hammer.hammer.x==111);
        assert(hammer.hammer.y==243);
        bool saw_drop=false,saw_bob_voice=false;
        for(const auto& event:impact.sound_and_hammer) {
            saw_drop|=event.op==mb::Op::DropHammer;
            saw_bob_voice|=event.op==mb::Op::PlayManagedSound &&
                event.argument>=696&&event.argument<=701;
        }
        assert(saw_drop&&saw_bob_voice);

        auto cooldown=sm::update_pilchard_native(
            active,hammer,source,2,{0,1,101.f,202.f,false},rand);
        assert(hammer.cooldown==499);
        assert(hammer.hammer_dropped);
        assert(!sm::pickup_dropped_hammer(
            hammer,{0,1,111.f,350.f,false}));
        assert(!sm::pickup_dropped_hammer(
            hammer,{1,1,111.f,351.f,false}));
        assert(!sm::pickup_dropped_hammer(
            hammer,{0,1,131.f,351.f,false}));
        assert(sm::pickup_dropped_hammer(
            hammer,{0,1,130.f,351.f,false}));
        assert(hammer.hammer_held);
        assert(!hammer.hammer_dropped);
        assert(!sm::pickup_dropped_hammer(
            hammer,{0,1,111.f,351.f,false}));
        (void)cooldown;
    }
}

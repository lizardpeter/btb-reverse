// Optional original-executable differential probe; paired with tools/herding_near_escape_differential.py.
#include "btb/herding_source_near_escape.hpp"
#include "btb/herding_source_animal_chatter.hpp"
#include <iostream>
#include <iomanip>
int main(){int type,dx,dy;float speed;int frame,count;unsigned seed,status;
 while(std::cin>>type>>dx>>dy>>speed>>frame>>count>>seed>>status){
 btb::herding::RetailEntityRecord32 hero{},animal{};
 hero.type=0;hero.x=500;hero.y=400;hero.source_right=100;hero.source_bottom=100;
 animal.type=type;animal.x=551+dx;animal.y=433+dy;
 animal.x_float=static_cast<float>(animal.x);animal.y_float=static_cast<float>(animal.y);
 animal.source_right=102;animal.source_bottom=95;
 animal.movement_speed=speed;animal.animation_frame=frame;animal.animation_frame_countdown=count;
 auto out=btb::herding::original_herding_near_escape_step(animal,hero);
 if(!out){std::cout<<"none\n";continue;}
 btb::retail::OriginalRetailRandom rng(seed);
 auto chatter=btb::herding::original_herding_animal_chatter(rng,type-1,
     {bool(status&1),bool(status&2),bool(status&4)});
 if(!chatter)return 2;
 const auto& a=out->animal;
 std::cout<<std::setprecision(10)<<a.x<<' '<<a.y<<' '<<a.x_float<<' '<<a.y_float<<' '<<a.direction<<' '<<a.movement_speed<<' '<<a.animation_frame<<' '<<a.animation_frame_countdown<<' '<<a.movement_active<<' '<<rng.state()<<' '<<chatter->requested_sound_id.value_or(-1)<<'\n';
 }
}

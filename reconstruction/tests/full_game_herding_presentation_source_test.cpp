#include "btb/full_game_herding_presentation.hpp"
#include "btb/full_game_blt_clip.hpp"

#include <cassert>
#include <string_view>

int main() {
    using namespace btb::full_game;
    namespace h=btb::herding;

    HerdingScene scene;
    h::RetailEntityRecord32 pickles{};
    pickles.entity_id=0;
    pickles.type=static_cast<int>(h::EntityType::FarmerPickles);
    pickles.x=478;pickles.y=471;
    pickles.direction=1;
    pickles.animation_frame=2;
    pickles.source_bottom=128;
    scene.entities.push_back(pickles);

    h::RetailEntityRecord32 sheep{};
    sheep.entity_id=1;
    sheep.type=static_cast<int>(h::EntityType::Sheep);
    sheep.x=530;sheep.y=450;
    sheep.direction=2;
    sheep.animation_frame=6;
    sheep.source_bottom=103;
    sheep.movement_speed=2.0f;
    sheep.animation_frame_countdown=2;
    scene.entities.push_back(sheep);

    h::RetailEntityRecord32 gate{};
    gate.entity_id=2;
    gate.type=static_cast<int>(h::EntityType::GateLeft);
    gate.x=0;gate.y=0;
    gate.source_bottom=70;
    gate.animation_frame=0;
    gate.animation_frame_countdown=50;
    scene.entities.push_back(gate);

    scene.selected_food=h::FoodType::DuckFood;
    const auto frame=compose_original_herding_frame(scene);
    assert(!frame.missing_farmer_pickles);
    assert(frame.rejected_unknown_entities==0);
    assert(frame.camera.camera_x==178);
    assert(frame.camera.camera_y==361);
    assert(frame.draws.size()==9); // BG, gate, sheep, Pickles, 3 bags, UI, toolbar

    const auto& background=frame.draws[0];
    assert(background.source_asset=="Data\\SubGame1\\bk)1_revised_01.bmp");
    assert(background.x==20 && background.y==20);
    assert(background.source_rectangle);
    assert(background.source_rectangle->left==178);
    assert(background.source_rectangle->top==361);
    assert(background.source_rectangle->right==778);
    assert(background.source_rectangle->bottom==741);
    assert(background.destination_clip);
    assert(*background.destination_clip == (Rect{20,20,620,400}));

    assert(frame.draws[1].source_asset=="Data\\SubGame1\\gateleft.bmp");
    assert(frame.draws[1].x==20+717-178);
    assert(frame.draws[1].y==20+162-361);
    assert(frame.draws[2].source_asset=="Data\\SubGame1\\sheep_shadow.bmp");
    assert(frame.draws[3].source_asset=="Data\\SubGame1\\Pickles_1_8bit.bmp");
    assert(frame.draws[3].source_rectangle);
    assert(frame.draws[3].source_rectangle->left==(39+2)*128);
    assert(frame.draws[3].source_rectangle->top==0);
    assert(scene.entities[1].animation_frame==7);
    assert(scene.entities[1].animation_frame_countdown==100);
    assert(scene.entities[2].animation_frame_countdown==49);

    assert(frame.draws[7].source_asset=="Data\\SubGame1\\uisurround.bmp");
    assert(!frame.draws[7].destination_clip);
    assert(frame.draws[8].source_asset=="Data\\SubGame1\\ducktoolbar.bmp");
    assert(frame.draws[8].x==284 && frame.draws[8].y==417);
    assert(!frame.draws[8].destination_clip);

    const auto crop=retail_clip_blt_to_rect(
        {0,0,128,128},10,10,{20,20,620,400});
    assert(crop.visible);
    assert(crop.destination.left==20 && crop.destination.top==20);
    assert(crop.source.left==10 && crop.source.top==10);
    assert(crop.source.right==128 && crop.source.bottom==128);

    const auto entirely_outside=retail_clip_blt_to_rect(
        {0,0,128,128},630,50,{20,20,620,400});
    assert(!entirely_outside.visible);

    HerdingScene missing;
    const auto invalid=compose_original_herding_frame(missing);
    assert(invalid.missing_farmer_pickles);
    assert(invalid.draws.empty());
}

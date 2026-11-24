#include "btb/park_designer_retail_catalog.hpp"

#include <cassert>
#include <limits>
#include <sstream>
using namespace btb::park_designer;

static SaveData empty_park(){
    SaveData data{};
    for(auto& o:data.objects){o.object_code=-1;o.surface_ptr32=~U32{0};}
    data.state(TrailingStateIndex::ActiveEditorMode)=2;
    data.state(TrailingStateIndex::NextDecorateRecord)=300;
    return data;
}
int main(){
    for(I32 i=0;i<12;++i){
        const auto& r=kRetailToolbarRegions[static_cast<std::size_t>(i)];
        assert(retail_toolbar_hit((r.left+r.right)/2,(r.top+r.bottom)/2)==i);
        assert(retail_toolbar_hit(r.left,r.top)==-1);
    }
    assert(retail_toolbar_hit(280,430)==-1);
    assert(retail_toolbar_hit(1,1)==-1);
    assert(kRetailModeToolbarSheets[3].hover.ends_with("vuiewred.bmp"));
    assert(kRetailPaletteSheets[0].ends_with("pondopjects.bmp"));
    for(I32 cat=0;cat<4;++cat){
        const auto dim=kRetailDecorationDimensions[static_cast<std::size_t>(cat)];
        for(I32 variant=0;variant<5;++variant){
            auto summer=retail_park_sprite(EditorMode::Decorate,Season::Summer,cat,variant);
            auto winter=retail_park_sprite(EditorMode::Decorate,Season::Winter,cat,variant);
            assert(summer&&winter&&summer->bitmap!=winter->bitmap);
            assert(winter->bitmap.find("/SNOW/")!=std::string_view::npos);
            assert(summer->source.left==variant*dim.x&&summer->source.right==(variant+1)*dim.x);
            assert(summer->source.bottom==dim.y);
            auto thumbnail=retail_palette_thumbnail(cat,variant);
            assert(thumbnail&&thumbnail->left==variant*34&&thumbnail->top==cat*61);
            auto data=empty_park();
            data.state(TrailingStateIndex::DecoratePalettePage)=cat;
            btb::retail::OriginalRetailRandom rng{2},expected{2};
            assert(expected.next_rand()%3==0);
            const auto base=cat==0?219:cat==1?216:cat==2?(variant<=2?225:229):222;
            const auto voice=base+expected.next_rand()%2;
            auto placed=commit_retail_decoration(data,100,50,variant,{},rng);
            assert(placed.status==DecorationCommitStatus::Placed&&placed.slot==300);
            assert(placed.managed_voice==voice&&placed.voice_context==99);
            assert(rng.state()==expected.state());
            auto& o=data.objects[300];
            assert(o.object_code==400&&o.enabled_or_active==1&&o.sort_layer==201);
            assert(o.bound_category==cat&&o.bound_variant==variant);
            assert(o.record_index==300&&o.surface_ptr32==static_cast<U32>(cat*5+variant));
            assert(o.right==100+dim.x&&o.bottom==50+dim.y);
            assert(o.sprite_width==dim.x&&o.sprite_height==dim.y&&o.fountain_frame_countdown==9);
            assert((placed.restored_pointer==Vec2i{100+dim.x/2,50+dim.y/2}));
            assert(data.state(TrailingStateIndex::NextDecorateRecord)==301);
            o.special_collision_origin_x=123;o.special_collision_origin_y=234;
            auto moved=commit_retail_decoration(data,150,60,-1,300,rng);
            assert(moved.status==DecorationCommitStatus::Placed&&moved.slot==300);
            assert(o.left==150&&o.top==60&&o.special_collision_origin_x==123&&o.special_collision_origin_y==234);
            assert(data.state(TrailingStateIndex::NextDecorateRecord)==301);
            std::stringstream bytes(std::ios::in|std::ios::out|std::ios::binary);
            write_save(bytes,data);bytes.seekg(0);
            auto loaded=read_save(bytes);
            assert(loaded.objects[300].surface_ptr32==o.surface_ptr32);
            assert(loaded.objects[300].bound_variant==variant&&loaded.objects[300].left==150);
        }
    }
    for(I32 cat=0;cat<3;++cat)for(I32 variant=0;variant<5;++variant){
        auto sprite=retail_park_sprite(EditorMode::Bandstand,Season::Summer,cat,variant);
        assert(sprite&&sprite->source.bottom==kRetailBandstandDimensions[static_cast<std::size_t>(cat)].y);
    }
    assert(!retail_park_sprite(EditorMode::Bandstand,Season::Summer,3,0));
    assert(!retail_park_sprite(EditorMode::Decorate,static_cast<Season>(5),0,0));
    auto data=empty_park();
    data.state(TrailingStateIndex::NextDecorateRecord)=398;
    btb::retail::OriginalRetailRandom rng{1};
    auto full=commit_retail_decoration(data,100,50,0,{},rng);
    assert(full.status==DecorationCommitStatus::Full&&full.managed_voice==241&&full.voice_context==1);
    assert(rng.state()==1&&data.objects[398].object_code==-1);
    // Retail checks the pond footprint before reporting a full decoration table.
    data.objects[0].object_code=100;data.objects[0].left=100;data.objects[0].top=50;
    data.objects[0].bound_variant=0;
    auto blocked=commit_retail_decoration(data,100,50,0,{},rng);
    assert(blocked.status==DecorationCommitStatus::Blocked&&!blocked.managed_voice);
    assert(rng.state()==1);
    data.objects[0].object_code=-1;
    data.state(TrailingStateIndex::NextDecorateRecord)=300;
    auto silent=commit_retail_decoration(data,100,50,0,{},rng);
    assert(silent.status==DecorationCommitStatus::Placed&&!silent.managed_voice);
    btb::retail::OriginalRetailRandom expected{1};(void)expected.next_rand();
    assert(rng.state()==expected.state());
    assert(commit_retail_decoration(data,std::numeric_limits<I32>::max(),50,0,{},rng).status==DecorationCommitStatus::InvalidInput);
    assert(commit_retail_decoration(data,100,50,5,{},rng).status==DecorationCommitStatus::InvalidInput);
    assert(rng.state()==expected.state());
}

#include "sprite_motion.hpp"
using namespace btb;
using namespace btb::enhanced;
static full_game::ResolvedDraw sprite(const char* name,int x,int y,bool keyed=true){
    full_game::ResolvedDraw d;
    d.original={name,x,y,{},keyed};
    d.source.status=full_game::AssetResolutionStatus::FoundInstalled;d.source.absolute_path=name;return d;
}
int main(){
    SpriteMotion m;m.state_changed(0x21);
    std::vector<full_game::ResolvedDraw> a{sprite("back",0,0,false),sprite("car",100,200)};
    auto b=a;b[1].original.x=116;b[1].original.y=184;
    m.ingest(a,100000000);m.ingest(b,116000000);
    if(!m.active())return 1;
    auto s=m.sample(116000000);if(!s||(*s)[1].original.x!=100||(*s)[1].original.y!=200)return 2;
    s=m.sample(120000000);if(!s||(*s)[1].original.x!=104||(*s)[1].original.y!=196)return 3;
    if(m.sample(120000001))return 4;
    s=m.sample(124000000);if(!s||(*s)[1].original.x!=108||(*s)[1].original.y!=192)return 5;
    s=m.sample(132000000);if(!s||(*s)[1].original.x!=116||(*s)[1].original.y!=184||m.active())return 6;
    if(m.sample(140000000)||!m.current_sample())return 7;
    auto c=b;c[1].original.x=300;m.ingest(c,132000000);
    if(m.active())return 8; // teleport
    c.push_back(sprite("car",40,40));m.ingest(c,148000000);
    c[1].original.x=310;m.ingest(c,164000000);if(m.active())return 9; // ambiguous
    m.reset();m.ingest(a,200000000);
    b[1].original.source_rectangle=full_game::Rect{0,0,10,10};
    m.ingest(b,216000000);if(m.active())return 10; // art change
    m.reset();m.ingest(a,300000000);b=a;b[1].original.x=112;m.ingest(b,400000000);
    if(m.active())return 11; // stall
    m.state_changed(0x05);m.ingest(a,500000000);m.ingest(b,516000000);if(m.active())return 12;
    m.state_changed(0x21);m.ingest(a,600000000);m.ingest(b,616000000);if(!m.active())return 13;
    m.state_changed(0x22);if(m.active()||m.current_sample())return 14;
    return 0;
}

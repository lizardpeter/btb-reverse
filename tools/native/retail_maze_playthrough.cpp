// Read-only installed-map navigation, not a gameplay AI or a state override.
#include "btb/full_game_maze.hpp"
#include "btb/maze_package_pickup.hpp"
#include "retail_activity_audit.hpp"
#include <cmath>
#include <functional>
#include <iterator>
#include <memory>
#include <queue>
#include <utility>

namespace {
using namespace btb::retail_probe;
namespace maze=btb::maze;
using NodeKey=std::pair<int,int>;

const maze::Node& node(const maze::Data& data,NodeKey key) {
    for(const auto& n:data.screens.at(key.first).nodes)
        if(n.id==key.second)return n;
    throw std::runtime_error("route references unknown Maze node");
}

// Weighted graph search is diagnostic-only. At every captured junction
// choose a route towards a currently available package. The game itself
// still performs normal movement, node/portal capture, pickup and AI.
int choose_direction(const maze::Data& data,const fg::MazeDriver& game) {
    const auto& player=*game.player();
    const NodeKey start{player.screen,player.node};
    std::map<NodeKey,double> distance;
    std::map<NodeKey,int> first_direction;
    using QueueEntry=std::pair<double,NodeKey>;
    std::priority_queue<QueueEntry,std::vector<QueueEntry>,std::greater<QueueEntry>> pending;
    distance[start]=0;first_direction[start]=0;pending.push({0,start});
    constexpr int keys[]{4,2,8,1}; // retail keyboard up/right/down/left
    while(!pending.empty()) {
        const auto [cost,key]=pending.top();pending.pop();
        if(cost!=distance.at(key))continue;
        const auto& from=node(data,key);
        const auto offer=[&](NodeKey next,double edge,int direction) {
            const auto candidate=cost+edge;
            if(!distance.contains(next)||candidate<distance.at(next)) {
                distance[next]=candidate;
                first_direction[next]=key==start?direction:first_direction.at(key);
                pending.push({candidate,next});
            }
        };
        for(int i=0;i<4;++i)if(from.links[i]>=0) {
            const NodeKey next{key.first,from.links[i]};
            const auto& to=node(data,next);
            offer(next,std::hypot(double(from.retail_position.x-to.retail_position.x),
                                  double(from.retail_position.y-to.retail_position.y)),keys[i]);
        }
        if(const auto destination=from.portal_destination()) {
            for(const auto& paired:data.screens.at(int(*destination)).nodes)
                if(paired.portal_destination()==maze::Screen(key.first))
                    offer({int(*destination),paired.id},10,0);
        }
    }
    double best=std::numeric_limits<double>::infinity();
    int direction=0;
    for(int screen=0;screen<3;++screen)for(const auto& slot:game.boxes().screens[screen]) {
        if(slot.node_id<0)continue;
        int x{},y{};
        if(slot.status==maze::kSpudAvailablePackageStatus) {
            const auto& box=data.box_locations.at(screen).at(slot.node_id);
            x=box.position.x;y=box.position.y;
        } else if(slot.status==maze::kSpudDroppedPackageStatus) {
            x=slot.dropped_x;y=slot.dropped_y;
        } else continue;
        // The player collects by physical distance, not the NPC's
        // package-node table. Consider actual pixel anchors, including
        // dropped boxes between two nodes and the off-center retail boxes.
        for(const auto& n:data.screens[screen].nodes) {
            const NodeKey key{screen,n.id};
            if(!distance.contains(key)||!first_direction.at(key))continue;
            const auto gap=std::hypot(double(n.retail_position.x-x),double(n.retail_position.y-y));
            const auto cost=distance.at(key)+gap*4+(gap>45?1000:0);
            if(cost<best){best=cost;direction=first_direction.at(key);}
        }
    }
    return direction;
}

void play(const fs::path& root,const maze::Data& data,int difficulty,unsigned seed,bool wait_for_npc) {
    const int profile=int((seed+difficulty)%btb::progress::kPlayerCount);
    Audit audit{fg::OriginalAssetResolver(root)};
    VoiceModel voices;
    btb::retail::OriginalRetailRandom rng(seed);
    fg::GameRoot game;
    check(game.select_profile(profile),"profile rejected");
    auto driver=std::make_unique<fg::MazeDriver>(root,difficulty,&rng);
    auto* maze_game=driver.get();game.install(fg::ActivityId::Maze,std::move(driver));
    game.set_outer_state(State::MazeInit);
    check(game.advance({}).kind==fg::FrameKind::ActivityInitialized,"Maze initialization failed");
    const int total=maze_game->boxes().total;
    check(total==4*(difficulty+1),"wrong package count");
    int remaining=total,previous_node=-1,previous_screen=-1,direction=0;
    unsigned portals=0,npc_active=0,phase_mask=0;
    int observed_screen=-1;
    for(int tick=0;tick<30000;++tick) {
        const auto* player=maze_game->player();
        check(player!=nullptr,"Maze unloaded early");
        if(observed_screen>=0&&observed_screen!=player->screen)++portals;
        observed_screen=player->screen;
        if(!player->frozen&&!player->transition&&
            (player->screen!=previous_screen||player->node!=previous_node||!direction)) {
            direction=choose_direction(data,*maze_game);
            previous_screen=player->screen;previous_node=player->node;
        }
        // Intentionally wait in Hard for an authentic NPC spawn; never
        // force phase, package status, position or time inside the driver.
        const auto* npc=maze_game->npc();
        if(npc&&int(npc->phase)>1)++npc_active;
        if(npc)phase_mask|=1u<<unsigned(npc->phase);
        fg::ActivityFrameInput input{};
        input.elapsed_centiseconds=tick*2; // simulated 50 Hz clock
        input.directional_input_bits=(wait_for_npc&&tick<900)?0:direction;
        voices.sample(input);
        const auto step=game.advance(input);
        check(step.kind==fg::FrameKind::ActivityUpdated,"Maze advance failed: "+step.error);
        audit.observe(step.effects);voices.observe(step.effects);
        if(step.effects.save_and_unload) {
            check(remaining==0,"Maze ended before collecting every package");
            check(!maze_game->player(),"Maze did not unload");
            check(!wait_for_npc||npc_active>0,"NPC failed to spawn during deliberate wait");
            completion(game,step.effects,Slot::Maze,State::MazeInit,profile);
            std::cout<<"maze difficulty="<<difficulty<<" seed="<<seed<<" npc_wait="<<wait_for_npc
                     <<" collected="<<total<<" portals="<<portals<<" npc_active="<<npc_active
                     <<" phase_mask="<<phase_mask;
            audit.report();return;
        }
        remaining=maze_game->boxes().remaining;
    }
    throw std::runtime_error("Maze diagnostic exhausted frame budget");
}
}

int main(int argc,char** argv) {
    if(argc<2||argc>3) {
        std::cerr<<"usage: btb_retail_maze_playthrough <installed-root> [seed-count]\n";
        return 2;
    }
    try {
        const fs::path root=argv[1];
        const int seeds=argc==3?std::stoi(argv[2]):1;
        check(seeds>=1&&seeds<=100,"seed count outside 1..100");
        Audit audit{fg::OriginalAssetResolver(root)};
        std::ifstream source(audit.resolve("loaddata/maze_nodes.txt"));
        const auto data=maze::parse_data(std::string(std::istreambuf_iterator<char>(source),{}));
        for(unsigned seed=1;seed<=unsigned(seeds);++seed) {
            for(int difficulty=0;difficulty<3;++difficulty)play(root,data,difficulty,seed,false);
            play(root,data,2,seed,true);
        }
        return 0;
    } catch(const std::exception& e) {std::cerr<<e.what()<<'\n';return 1;}
}

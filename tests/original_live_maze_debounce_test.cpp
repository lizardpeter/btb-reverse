#include "btb/maze_runtime.hpp"
#include "btb/original_activity9.hpp"
#include <array>
#include <cstdint>
#include <cstdio>

int main() {
    btb::maze::DirectionalDebounce live{};
    btb::retail_activity::MazeDebounce source{};
    constexpr std::array<btb::maze::DirectionalIntent,15> sequence{{
        {1,0},{1,0},{1,0},{1,-1},{1,-1},{1,-1},
        {0,-1},{0,-1},{0,-1},{-1,1},{-1,1},{-1,1},
        {0,0},{0,0},{0,0}
    }};
    for (std::size_t i=0;i<sequence.size();++i) {
        auto output=live.update(sequence[i]);
        auto x=sequence[i].x,y=sequence[i].y;
        btb::original_activity9::debounce_maze_directions(source,x,y);
        if (output.x!=x || output.y!=y ||
            live.x_history()!=source.first ||
            live.y_history()!=source.second) {
            std::fprintf(stderr,
                "live Maze and PE32 0041AEB0 diverged at frame %zu\n",i);
            return 1;
        }
        if(i==2 && (x!=1 || y!=0))return 2;
        if(i==3 && (x!=0 || y!=0))return 3;
        if(i==5 && (x!=1 || y!=-1))return 4;
        if(i==8 && (x!=0 || y!=-1))return 5;
    }
    return 0;
}

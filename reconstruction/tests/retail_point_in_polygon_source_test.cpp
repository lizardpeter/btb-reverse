#include "btb/retail_point_in_polygon.hpp"

#include <cassert>
#include <cstdint>
#include <vector>

using btb::herding::Vec2i;
using btb::retail_geometry::original_point_in_polygon_status;
using btb::retail_geometry::original_polygon_contains;

int main() {
    const std::vector<Vec2i> square{{
        {0,0},{10,0},{10,10},{0,10}
    }};
    // The 2002 binary returns 0 on ODD intersections / inside.
    assert(original_point_in_polygon_status(square,5,5)==0);
    assert(original_point_in_polygon_status(square,-1,5)==1);
    assert(original_point_in_polygon_status(square,11,5)==1);
    assert(original_polygon_contains(square,5,5)==true);
    assert(original_polygon_contains(square,-1,5)==false);
    assert(original_point_in_polygon_status({},5,5)==1);

    // Source 0x428520 uses (Y>minY) && (Y<=maxY) and X<=intercept.
    // Thus touching the two opposite rectangle edges differs.
    assert(original_point_in_polygon_status(square,5,0)==1);
    assert(original_point_in_polygon_status(square,5,10)==0);
    assert(original_point_in_polygon_status(square,0,5)==1);
    assert(original_point_in_polygon_status(square,10,5)==0);

    // The diagonal intersection uses signed integer IMUL/IDIV with
    // truncation, NOT an IEEE floating-point intersection.
    const std::vector<Vec2i> triangle{{
        {0,0},{7,5},{0,5}
    }};
    // For Y=3 the positive diagonal X=7*3/5 truncates to 4.
    assert(original_point_in_polygon_status(triangle,4,3)==0);
    assert(original_point_in_polygon_status(triangle,5,3)==1);

    // Installed original herd.txt group 0, after retail (-64,-100)
    // initializer transformation. Pickles starts inside the known
    // main-world navigation region at (478,471).
    const std::vector<Vec2i> retail_navigation{{
        {-20,-1},{138,-1},{289,179},{397,163},
        {466,199},{902,81},{902,66},{902,135},
        {1022,249},{1193,305},{1193,800},{-20,800}
    }};
    assert(original_point_in_polygon_status(
        retail_navigation,478,471)==0);
    assert(original_point_in_polygon_status(
        retail_navigation,-40,471)==1);
    assert(original_point_in_polygon_status(
        retail_navigation,1250,471)==1);

    // 2002 source group 2 is a lower-world six-point polygon,
    // not a rectangular collision derived from rendered art.
    const std::vector<Vec2i> retail_exclusion{{
        {30,500},{500,500},{500,500},{1200,500},
        {1200,850},{30,850}
    }};
    assert(original_point_in_polygon_status(
        retail_exclusion,650,650)==0);
    assert(original_point_in_polygon_status(
        retail_exclusion,650,486)==1);
}

#include "btb/herding_navigation_boundary.hpp"

#include <cassert>
#include <limits>
#include <vector>

int main() {
    using namespace btb::herding;
    const std::vector<Vec2i> square{{
        {0,0},{10,0},{10,10},{0,10}
    }};

    auto result=retail_herding_navigation_boundary_step(
        square,6.75F,6.5F,5,5,true);
    assert(result);
    assert(result->choice==RetailHerdingBoundaryChoice::ValidNewPosition);
    assert(result->x==6.75F);
    assert(result->y==6.5F);
    assert(result->polygon_tests==1);

    // Candidate (-1,6) falls outside, but (5,6) is within polygon:
    // keep Y movement and rollback only X.
    result=retail_herding_navigation_boundary_step(
        square,-1.25F,6.75F,5,5,true);
    assert(result);
    assert(result->choice==RetailHerdingBoundaryChoice::RestorePreviousX);
    assert(result->x==5.0F);
    assert(result->y==6.75F);
    assert(result->polygon_tests==2);

    // Candidate (6,-1) falls outside. Trying previous X first still
    // fails, but current X + previous Y is accepted.
    result=retail_herding_navigation_boundary_step(
        square,6.5F,-1.75F,5,5,true);
    assert(result);
    assert(result->choice==RetailHerdingBoundaryChoice::RestorePreviousY);
    assert(result->x==6.5F);
    assert(result->y==5.0F);
    assert(result->polygon_tests==3);

    // Both axes outside: neither candidate may be accepted.
    result=retail_herding_navigation_boundary_step(
        square,-1.25F,-1.75F,5,5,true);
    assert(result);
    assert(result->choice==RetailHerdingBoundaryChoice::RestoreBothAxes);
    assert(result->x==5.0F && result->y==5.0F);
    assert(result->polygon_tests==3);

    // Native extra source flag can skip axis recovery entirely.
    result=retail_herding_navigation_boundary_step(
        square,-1.25F,6.75F,5,5,false);
    assert(result);
    assert(result->choice==RetailHerdingBoundaryChoice::RestoreBothAxes);
    assert(result->polygon_tests==1);

    // Original 0x4304D0 truncates floating inputs toward zero
    // before integer polygon evaluation; it does not floor -0.5 to -1.
    result=retail_herding_navigation_boundary_step(
        square,1.25F,0.99F,5,5,true);
    assert(result);
    assert(result->choice==RetailHerdingBoundaryChoice::RestorePreviousY);
    assert(result->polygon_tests==3);

    assert(!retail_herding_navigation_boundary_step(
        square,std::numeric_limits<float>::infinity(),
        5.0F,5,5,true));
    assert(!retail_herding_navigation_boundary_step(
        square,5.0F,std::numeric_limits<float>::quiet_NaN(),
        5,5,true));

    // Installed original Herding navigation group0 after -64,-100.
    const std::vector<Vec2i> world{{
        {-20,-1},{138,-1},{289,179},{397,163},
        {466,199},{902,81},{902,66},{902,135},
        {1022,249},{1193,305},{1193,800},{-20,800}
    }};
    result=retail_herding_navigation_boundary_step(
        world,478.5F,471.25F,478,471,true);
    assert(result);
    assert(result->choice==RetailHerdingBoundaryChoice::ValidNewPosition);
    assert(result->polygon_tests==1);
}

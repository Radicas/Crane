#include "geometry_algo_point.h"

namespace geometry {

bool twoPointsIsMuchClose(
    const geometry::POINT& p1,
    const geometry::POINT& p2,
    double EP) {
    return (lessThanEpsilon(p1.x, p2.x, EP) && lessThanEpsilon(p1.y, p2.y, EP));
}

} // namespace geometry

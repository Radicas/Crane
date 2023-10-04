#include "geometry_algo_math.h"

namespace geometry {

bool lessThanEpsilon(double num1, double num2, double EP) {
    return std::abs(num1 - num2) < EP;
};

} // namespace geometry

#ifndef GEOMETRY_ALGO_POLYGON_H
#define GEOMETRY_ALGO_POLYGON_H

#include "geometry_algo_core.h"

namespace geometry {
    
bool polygonOffset(geometry::POLYGON& polygon, double gap, bool expand);

} // namespace geometry

#endif // GEOMETRY_ALGO_POLYGON_H
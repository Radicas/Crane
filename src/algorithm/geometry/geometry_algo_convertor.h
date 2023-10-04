#ifndef GEOMETRY_ALGO_CONVERTOR_H
#define GEOMETRY_ALGO_CONVERTOR_H
#include "geometry_algo_polygon.h"
#include <QPolygonF>
namespace geometry {

geometry::POLYGON polygonF2POLYGON(const QPolygonF& polygon) {
    geometry::POLYGON result;
    for (const auto& p : polygon) {
        result.emplace_back(p.x(), -p.y());
    }
    return result;
};

QPolygonF POLYGON2polygonF(const geometry::POLYGON& polygon) {
    QPolygonF result;
    for (const auto& p : polygon) {
        result.push_back({p.x, -p.y});
    }
    return result;
};

} // namespace geometry

#endif // GEOMETRY_ALGO_CONVERTOR_H
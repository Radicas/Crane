//
// Created by Radica on 2023/3/21.
//

#ifndef CRANE_SHAPEHELPER_H
#define CRANE_SHAPEHELPER_H

#include "algorithm/geometry/polygon.h"

#include <boost/foreach.hpp>
#include <boost/geometry/geometries/point_xy.hpp>
#include <boost/geometry/geometries/polygon.hpp>
#include <boost/geometry/geometry.hpp>
#include <boost/polygon/polygon.hpp>

namespace Crane {

namespace bg = boost::geometry;
namespace bp = boost::polygon;

typedef bg::model::d2::point_xy<double> point_t;
typedef bg::model::polygon<point_t> polygon_t;

/**
 * @brief 从boost geometry转成geometry下的PolygonWithHoles
 * @param poly_t
 * @return
 */
static geometry::PolygonWithHoles fromBoostPolygon(const polygon_t& poly_t) {
    geometry::PolygonWithHoles poly_with_holes;
    geometry::POLYGON out_poly;
    std::vector<geometry::POLYGON> inner_polys;

    for (const auto& o_p : poly_t.outer()) {
        out_poly.emplace_back(o_p.x(), o_p.y());
    }

    for (const auto& inner : poly_t.inners()) {
        geometry::POLYGON inner_poly;
        for (auto i_p : inner) {
            inner_poly.emplace_back(i_p.x(), i_p.y());
        }
        inner_polys.emplace_back(inner_poly);
    }

    poly_with_holes.setOuter(out_poly);
    poly_with_holes.setInner(inner_polys);
    return poly_with_holes;
}

static polygon_t toBoostPolygon(const geometry::PolygonWithHoles& poly) {
    polygon_t boost_polygon;
    for (const auto& o_p : poly.outer()) {
        bg::append(boost_polygon.outer(), point_t(o_p.x, o_p.y));
    }
    auto& inners = poly.inner();
    boost_polygon.inners().resize(poly.inner().size());
    for (int i = 0; i < inners.size(); ++i) {
        for (int j = 0; j < inners[i].size(); ++j) {
            bg::append(boost_polygon.inners()[i], point_t(inners[i][j].x, inners[i][j].y));
        }
    }
    return boost_polygon;
}

} // namespace Crane

#endif // CRANE_SHAPEHELPER_H

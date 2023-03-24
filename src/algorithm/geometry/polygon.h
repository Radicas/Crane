//
// Created by Radica on 2023/3/24.
//

#ifndef CRANE_POLYGON_H
#define CRANE_POLYGON_H

#include "geometry_algo.h"

namespace R_GEOMETRY {

/**
 *
 */
typedef std::vector<POINT> Polygon;
// class Polygon {
// private:
//     std::vector<POINT> m_points;
//
// public:
//     Polygon();
//
//     Polygon(std::vector<POINT> points);
//
//     Polygon(std::vector<POINT>& points);
//
//     inline Polygon& operator<<(const POINT& p);
//
//     POINT& operator[](int index);
//
//     Polygon& operator=(const Polygon& poly);
//
//     const POINT& first() const;
//
//     const POINT& at(int index) const;
//
//     int size() const;
// };

class PolygonWithHoles {
public:
    PolygonWithHoles();

    ~PolygonWithHoles();

    void setOuter(const Polygon& aOuter);

    void setInner(const std::vector<Polygon>& aInner);

    Polygon& outer();

    std::vector<Polygon>& inner();

private:
    Polygon              m_outer;  // 外轮廓，顺时针放置
    std::vector<Polygon> m_inner;  // 孔洞轮廓，逆时针放置
};

}  // namespace R_GEOMETRY

#endif  // CRANE_POLYGON_H

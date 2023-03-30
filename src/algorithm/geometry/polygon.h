//
// Created by Radica on 2023/3/24.
//

#ifndef CRANE_POLYGON_H
#define CRANE_POLYGON_H

#include "geometry_algo.h"

namespace R_GEOMETRY {

typedef std::vector<POINT> Polygon;

class PolygonWithHoles {
public:
    PolygonWithHoles();

    ~PolygonWithHoles();

    void updatePos(const POINT& delta);

    void setOuter(const Polygon& aOuter);

    void setInner(const std::vector<Polygon>& aInner);

    const Polygon& outer() const;

    const std::vector<Polygon>& inner() const;

private:
    Polygon              m_outer;  // 外轮廓，顺时针放置
    std::vector<Polygon> m_inner;  // 孔洞轮廓，逆时针放置
};

}  // namespace R_GEOMETRY

#endif  // CRANE_POLYGON_H

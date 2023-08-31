//
// Created by Radica on 2023/3/24.
//

#ifndef CRANE_POLYGON_H
#define CRANE_POLYGON_H

#include "geometry_algo.hpp"

namespace geometry {

class PolygonWithHoles {
public:
    PolygonWithHoles();

    PolygonWithHoles(geometry::POLYGON outer, std::vector<geometry::POLYGON> inners);

    ~PolygonWithHoles();

    void updatePos(const POINT& delta);

    void setOuter(const geometry::POLYGON& aOuter);

    void setInner(const std::vector<geometry::POLYGON>& aInner);

    [[nodiscard]] const geometry::POLYGON& outer() const;

    [[nodiscard]] const std::vector<geometry::POLYGON>& inner() const;

private:
    geometry::POLYGON m_outer;              // 外轮廓，顺时针放置
    std::vector<geometry::POLYGON> m_inner; // 孔洞轮廓，逆时针放置
};

} // namespace geometry

#endif // CRANE_POLYGON_H

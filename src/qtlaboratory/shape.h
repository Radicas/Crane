/**
 * @file shape.h
 * @author Radica
 * @brief 通用图形类
 * @version 0.1
 * @date 2023-03-01
 *
 * @copyright Copyright (c) 2023
 *
 */
#include "algorithm/geometry/geometry_algo.h"
#include "algorithm/geometry/polygon.h"
#include <QPainterPath>
#include <vector>

typedef R_GEOMETRY::POINT POINT;

class Shape {
public:
    Shape();

    ~Shape();

    void addHole(const Shape& hole);

    bool contains(const POINT& point) const;

    Shape intersected(const Shape& other) const;

    Shape united(const Shape& other) const;

    Shape subtracted(const Shape& other) const;

    const QPainterPath& path() const;

    const R_GEOMETRY::PolygonWithHoles& getPolygonWithHoles() const;

    R_GEOMETRY::Polygon& getOuter();

    std::vector<R_GEOMETRY::Polygon>& getInner();

protected:
    QPainterPath                 m_path;                // 绘制路径
    R_GEOMETRY::PolygonWithHoles m_polygon_with_holes;  // 带孔多边形
    std::vector<QPolygonF>       m_vertex;              // 顶点

private:
};

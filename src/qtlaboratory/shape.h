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
#include "algorithm/geometry/geometry_algo.hpp"
#include "algorithm/geometry/polygon.h"
#include <QPainterPath>
#include <vector>

typedef geometry::POINT POINT;

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

    const geometry::PolygonWithHoles& getPolygonWithHoles() const;

    geometry::POLYGON& getOuter();

    std::vector<geometry::POLYGON>& getInner();

protected:
    QPainterPath m_path;                             // 绘制路径
    geometry::PolygonWithHoles m_polygon_with_holes; // 带孔多边形
    std::vector<QPolygonF> m_vertex;                 // 顶点

private:
};

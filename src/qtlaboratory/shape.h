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
/**
 * @brief 路径元素类型
 */
enum elementType { Line, Arc };

/**
 * @brief 路径元素
 */
struct PathElement {
    elementType type;
    POINT       startPoint;
    POINT       endPoint;
    POINT       centerPoint;
    qreal       radius;
    qreal       startAngle;
    qreal       spanAngle;

    PathElement(POINT sp, POINT ep)
        : type(Line)
        , startPoint(sp)
        , endPoint(ep)
        , centerPoint()
        , radius()
        , startAngle()
        , spanAngle() {}

    PathElement(POINT sp, POINT ep, POINT center, qreal radius, qreal sa, qreal swa)
        : type(Arc)
        , startPoint(sp)
        , endPoint(ep)
        , centerPoint(center)
        , radius(radius)
        , startAngle(sa)
        , spanAngle(swa) {}
};
class Shape {
public:
    Shape();

    ~Shape();

    void addSegment(PathElement segment);

    void addHole(const Shape& hole);

    bool contains(const POINT& point) const;

    Shape intersected(const Shape& other) const;

    Shape united(const Shape& other) const;

    Shape subtracted(const Shape& other) const;

    const QPainterPath& path() const;

    R_GEOMETRY::Polygon& getOuter();

    std::vector<R_GEOMETRY::Polygon>& getInner();

protected:
    QPainterPath                 m_path;                // 绘制路径
    R_GEOMETRY::PolygonWithHoles m_polygon_with_holes;  // 带孔多边形
    std::vector<PathElement>     m_path_elements;       // 路径元素，用于show info，导出标准数据等
    std::vector<QPolygonF>       m_vertex;              // 顶点

private:
};

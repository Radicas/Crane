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
#include <QPainterPath>
#include <vector>
/**
 * @brief 路径元素类型
 */
enum elementType { Line, Arc };

/**
 * @brief 路径元素
 */
struct PathElement {
    elementType type;
    QPointF     startPoint;
    QPointF     endPoint;
    QPointF     centerPoint;
    qreal       radius;
    qreal       startAngle;
    qreal       spanAngle;

    PathElement(QPointF sp, QPointF ep)
        : type(Line)
        , startPoint(sp)
        , endPoint(ep)
        , centerPoint()
        , radius()
        , startAngle()
        , spanAngle() {}

    PathElement(QPointF sp, QPointF ep, QPointF center, qreal radius, qreal sa, qreal swa)
        : type(Arc)
        , startPoint(sp)
        , endPoint(ep)
        , centerPoint(center)
        , radius(radius)
        , startAngle(sa)
        , spanAngle(swa) {}
};
using namespace R_GEOMETRY;
class Shape {
public:
    Shape();

    ~Shape();

    void addSegment(PathElement segment);

    void addHole(Shape hole);

    bool contains(const QPointF& point) const;

    Shape intersected(const Shape& other) const;

    Shape united(const Shape& other) const;

    Shape subtracted(const Shape& other) const;

    const QPainterPath& path() const;

protected:
    QPainterPath             m_path;           // 绘制路径
    std::vector<SEGMENT>     m_outlines;       // 边界线段组
    std::vector<Shape>       m_holes;          // 孔洞
    std::vector<PathElement> m_path_elements;  // 路径元素，用于show info，导出标准数据等
    std::vector<QPolygonF>   m_vertex;         // 顶点

private:
};

#include "geometryitem.h"
#include "algorithm/geometry/polygon.h"
#include "craneglobal.h"

#include <QDebug>
#include <QPainter>
#include <QPainterPath>
#include <QPolygonF>

/* region GeometryItem */
GeometryItem::GeometryItem(QGraphicsItem* parent) :
    QGraphicsItem(parent),
    m_selected_color(Crane::COLOR::BLUE.rgba()) {
}

GeometryItem::~GeometryItem() = default;

void GeometryItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) {
    // painter->setBrush(QColor::fromRgba(m_selected_color));
    painter->setBrush(Qt::NoBrush);
    QPen pen(Crane::COLOR::BLUE);
    pen.setWidth(0);
    painter->setPen(pen);
    painter->drawPath(m_path);
}

void GeometryItem::setColor(unsigned int color) {
    m_selected_color = color;
}

QRectF GeometryItem::boundingRect() const {
    return m_path.boundingRect();
}

void GeometryItem::updatePos(QPointF delta) {
    m_polygon_with_holes.updatePos(POINT(delta.x(), delta.y()));
    QGraphicsItem::update();
}

QPainterPath GeometryItem::shape() const {
    return m_path;
}

void GeometryItem::initPathByPolygonWithHoles(const geometry::PolygonWithHoles& poly) {
    auto& outer = poly.outer();
    auto& inners = poly.inner();
    if (outer.empty()) {
        std::cerr << "initPathByPolygonWithHoles outer is empty" << std::endl;
    }
    m_path.moveTo(outer[0].x, outer[0].y);
    for (const auto& p : outer) {
        m_path.lineTo(p.x, p.y);
    }
    for (const auto& inner : inners) {
        if (inner.empty()) {
            std::cerr << "initPathByPolygonWithHoles inner is empty" << std::endl;
            continue;
        }
        m_path.moveTo(inner[0].x, inner[0].y);
        for (const auto& p : inner) {
            m_path.lineTo(p.x, p.y);
        }
    }
    m_path.closeSubpath();
}

/* endregion */

/* region Rectangle */
RectangleItem::RectangleItem(QGraphicsItem* parent) :
    GeometryItem(parent) {
    // geometry::Polygon outer_pts, inner_pts;
    // outer_pts.emplace_back(0, 100);
    // outer_pts.emplace_back(100, 100);
    // outer_pts.emplace_back(100, 0);
    // outer_pts.emplace_back(0, 0);
    // outer_pts.emplace_back(0, 100);

    // inner_pts.emplace_back(20, 20);
    // inner_pts.emplace_back(80, 20);
    // inner_pts.emplace_back(80, 80);
    // inner_pts.emplace_back(20, 80);
    // inner_pts.emplace_back(20, 20);

    // m_polygon_with_holes.setOuter(outer_pts);
    // m_polygon_with_holes.setInner({inner_pts});
    // initPathByPolygonWithHoles(m_polygon_with_holes);
}

RectangleItem::~RectangleItem() = default;
/* endregion */

/* region CircleItem */
CircleItem::CircleItem(QGraphicsItem* parent) :
    GeometryItem(parent) {
    auto outer = geometry::arc2points<geometry::POINT>(0.0, 0.0, 50.0, 0.0, 2.0 * M_PI, 180);
    m_polygon_with_holes.setOuter(outer);
    m_polygon_with_holes.setInner({});
    initPathByPolygonWithHoles(m_polygon_with_holes);
}

CircleItem::~CircleItem() = default;
/* endregion */

/* region CurvePolygonItem */
CurvePolygonItem::CurvePolygonItem(QGraphicsItem* parent) :
    GeometryItem(parent) {
    // 将路径移动到矩形的左上角
    m_path.moveTo(10, 10);
    // 添加左侧贝塞尔曲线
    m_path.cubicTo(20, 20, 20, 80, 10, 90);
    // 添加底部贝塞尔曲线
    m_path.cubicTo(5, 95, 25, 95, 30, 90);
    // 添加右侧贝塞尔曲线
    m_path.cubicTo(40, 80, 40, 20, 30, 10);
    // 添加顶部贝塞尔曲线
    m_path.cubicTo(25, 5, 15, 5, 10, 10);
    // 将路径闭合
    m_path.closeSubpath();
}

CurvePolygonItem::~CurvePolygonItem() = default;
/* endregion */

/* region PolygonItem */
PolygonItem::PolygonItem(const geometry::PolygonWithHoles& polygon, QGraphicsItem* parent) :
    GeometryItem(parent) {
    m_polygon_with_holes = polygon;
    if (!polygon.outer().empty()) {
        auto p0 = polygon.outer()[0];
        m_path.moveTo(p0.x, p0.y);
        for (auto p : polygon.outer()) {
            m_path.lineTo(p.x, p.y);
        }
    }
    for (const auto& inners : polygon.inner()) {
        if (inners.empty()) {
            continue;
        }
        m_path.moveTo(inners[0].x, inners[0].y);
        for (const auto& inner : inners) {
            m_path.lineTo(inner.x, inner.y);
        }
    }
    m_path.closeSubpath();
}

PolygonItem::~PolygonItem() = default;
/* endregion */

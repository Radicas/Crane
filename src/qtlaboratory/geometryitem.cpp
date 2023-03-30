#include "geometryitem.h"
#include "algorithm/geometry/polygon.h"
#include "craneglobal.h"

#include <QDebug>
#include <QPainter>
#include <QPainterPath>
#include <QPolygonF>

/* region GeometryItem */
GeometryItem::GeometryItem(QGraphicsItem* parent)
    : QGraphicsItem(parent)
    , m_selected_color(Crane::COLOR::BLUE.rgba()) {}

GeometryItem::~GeometryItem() = default;

void GeometryItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) {
    painter->setBrush(QColor::fromRgba(m_selected_color));
    QPen pen(Crane::COLOR::RED);
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
/* endregion */

/* region Rectangle */
RectangleItem::RectangleItem(QGraphicsItem* parent)
    : GeometryItem(parent) {
    R_GEOMETRY::Polygon outer;
    outer.emplace_back(-100, 100);
    outer.emplace_back(100, 100);
    outer.emplace_back(100, -100);
    outer.emplace_back(-100, -100);
    outer.emplace_back(-100, 100);
    m_polygon_with_holes.setOuter(outer);
    m_path.moveTo(QPointF(outer[0].x, outer[0].y));
    for (auto& i : outer) {
        m_path.lineTo(QPointF(i.x, i.y));
    }
    m_path.closeSubpath();
}

RectangleItem::~RectangleItem() = default;
/* endregion */

/* region CircleItem */
CircleItem::CircleItem(QGraphicsItem* parent)
    : GeometryItem(parent) {
    auto outer = R_GEOMETRY::arc2segments(0, 0, 50, 0, 360 * R_GEOMETRY::PI / 180, 360);
    m_path.moveTo(outer[0].x, outer[0].y);
    for (auto& outlinePoint : outer) {
        m_path.lineTo(QPointF(outlinePoint.x, outlinePoint.y));
    }

    m_polygon_with_holes.setOuter(outer);
    m_polygon_with_holes.setInner({});
    /* 孔洞 */
    //    m_path.moveTo(-25, 25);
    //    m_path.lineTo(25, 25);
    //    m_path.lineTo(25, -25);
    //    m_path.lineTo(-25, -25);
    //    m_path.lineTo(-25, 25);
    //    m_path.closeSubpath();
    //    m_path.closeSubpath();
}

CircleItem::~CircleItem() = default;
/* endregion */

/* region CurvePolygonItem */
CurvePolygonItem::CurvePolygonItem(QGraphicsItem* parent)
    : GeometryItem(parent) {
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
PolygonItem::PolygonItem(R_GEOMETRY::PolygonWithHoles polygon, QGraphicsItem* parent)
    : GeometryItem(parent) {
    m_polygon_with_holes = polygon;
    if (!polygon.outer().empty()) {
        auto p0 = polygon.outer()[0];
        m_path.moveTo(p0.x, p0.y);
        for (auto p : polygon.outer()) {
            m_path.lineTo(p.x, p.y);
        }
        m_path.closeSubpath();
    }
}
PolygonItem::~PolygonItem() {}
/* endregion */

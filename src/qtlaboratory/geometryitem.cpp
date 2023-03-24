#include "geometryitem.h"
#include "algorithm/geometry/polygon.h"
#include "craneglobal.h"

#include <QDebug>
#include <QPainter>
#include <QPainterPath>
#include <QPolygonF>

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

RectangleItem::RectangleItem(QGraphicsItem* parent)
    : GeometryItem(parent) {
    R_GEOMETRY::Polygon outer, inner1;
    outer.emplace_back(-100, 100);
    outer.emplace_back(100, 100);
    outer.emplace_back(100, -100);
    outer.emplace_back(-100, -100);
    outer.emplace_back(-100, 100);

    inner1.emplace_back(-50, 50);
    inner1.emplace_back(50, 50);
    inner1.emplace_back(50, -50);
    inner1.emplace_back(-50, -50);
    inner1.emplace_back(-50, 50);

    m_polygon_with_holes.setOuter(outer);
    m_polygon_with_holes.setInner({ inner1 });

    // TODO: add to polygon_with_holes
    QPainterPath path;
    path.moveTo(QPointF(outer[0].x, outer[0].y));
    for (auto& i : outer) {
        path.lineTo(QPointF(i.x, i.y));
    }
    path.moveTo(QPointF(inner1[0].x, inner1[0].y));
    for (auto& i : inner1) {
        path.lineTo(i.x, i.y);
    }
    path.closeSubpath();
    m_path = path;
    qDebug() << "rect path: " << m_path;
}

RectangleItem::~RectangleItem() = default;

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
    qDebug() << "CurvePolygon" << m_path;
}

CurvePolygonItem::~CurvePolygonItem() = default;

PolygonItem::PolygonItem(const QPolygonF& polygon, QGraphicsItem* parent)
    : GeometryItem(parent) {
    m_path.addPolygon(polygon);
    qDebug() << "Polygon" << m_path;
}
PolygonItem::~PolygonItem() {}

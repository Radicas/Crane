#include "geometryitem.h"
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

Rectangle::Rectangle(QGraphicsItem* parent)
    : GeometryItem(parent) {
    QPolygonF polygon;
    polygon << QPointF(-100, -100) << QPointF(100, -100) << QPointF(100, 100) << QPointF(-100, 100);
    for (int i = 0; i < polygon.size(); ++i) {
        int     startIndex = i;
        int     endIndex   = i == polygon.size() - 1 ? 0 : i + 1;
        SEGMENT seg(POINT(polygon[startIndex].x(), polygon[startIndex].y()),
                    POINT(polygon[endIndex].x(), polygon[endIndex].y()));
        m_outlines.push_back(seg);
    }
    QPainterPath path;
    path.moveTo(polygon.first());
    for (int i = 1; i < polygon.size(); ++i) {
        path.lineTo(polygon.at(i));
    }
    path.closeSubpath();
    m_path = path;
    qDebug() << "rect path: " << m_path;
}

Rectangle::~Rectangle() = default;

Circle::Circle(QGraphicsItem* parent)
    : GeometryItem(parent) {
    auto outlinePoints = arc2segments(0, 0, 50, 0, 360 * R_GEOMETRY::PI / 180, 64);
    for (int i = 0; i < outlinePoints.size(); ++i) {
        int     startIndex = i;
        int     endIndex   = i == outlinePoints.size() - 1 ? 0 : i + 1;
        SEGMENT seg(POINT(outlinePoints[startIndex].x, outlinePoints[startIndex].y),
                    POINT(outlinePoints[endIndex].x, outlinePoints[endIndex].y));
        m_outlines.push_back(seg);
    }
    m_path.moveTo(outlinePoints[0].x, outlinePoints[0].y);
    for (auto& outlinePoint : outlinePoints) {
        m_path.lineTo(QPointF(outlinePoint.x, outlinePoint.y));
    }
    //    m_path.moveTo(-25, 25);
    //    m_path.lineTo(25, 25);
    //    m_path.lineTo(25, -25);
    //    m_path.lineTo(-25, -25);
    //    m_path.lineTo(-25, 25);
    //    m_path.closeSubpath();
    //    m_path.closeSubpath();
}

Circle::~Circle() = default;

CurvePolygon::CurvePolygon(QGraphicsItem* parent)
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

CurvePolygon::~CurvePolygon() = default;

Polygon::Polygon(const QPolygonF& polygon, QGraphicsItem* parent)
    : GeometryItem(parent) {
    m_path.addPolygon(polygon);
    qDebug() << "Polygon" << m_path;
}

Polygon::~Polygon() = default;

#include "geometryitem.h"
#include "craneglobal.h"

#include <QDebug>
#include <QPainter>
#include <QPainterPath>
#include <QPolygonF>

GeometryItem::GeometryItem(QGraphicsItem* parent)
    : QGraphicsItem(parent)
    , m_selected_color(Crane::COLOR::BLUE.rgb()) {}

GeometryItem::~GeometryItem() {}

void GeometryItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) {
    painter->setBrush(QColor(m_selected_color));
    painter->setPen(Crane::COLOR::RED);
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
    QPainterPath path;
    path.moveTo(polygon.first());
    for (int i = 1; i < polygon.size(); ++i) {
        path.lineTo(polygon.at(i));
    }
    path.closeSubpath();
    m_path = path;
}

Rectangle::~Rectangle() {}

Circle::Circle(QGraphicsItem* parent)
    : GeometryItem(parent) {
    m_path.addEllipse(QPointF(0, 0), 50, 50);
}

Circle::~Circle() {}

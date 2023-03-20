#include "geometryitem.h"
#include "Craneglobal.h"

#include <QPainter>
#include <QPainterPath>
#include <QPolygonF>

GeometryItem::GeometryItem(QGraphicsItem* parent)
    : QGraphicsItem(parent) {}

GeometryItem::~GeometryItem() {}

void GeometryItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) {
    painter->setBrush(Crane::COLOR::BLUE);
    painter->setPen(Crane::COLOR::RED);
    painter->drawPath(m_path);
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
}

Rectangle::~Rectangle() {}
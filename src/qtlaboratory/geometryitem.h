#ifndef REDEDA_GEOMETRYITEM_H
#define REDEDA_GEOMETRYITEM_H

#include "shape.h"
#include <QGraphicsItem>

class GeometryItem : public QGraphicsItem, public Shape {
public:
    explicit GeometryItem(QGraphicsItem* parent = nullptr);

    ~GeometryItem() override;

    QRectF boundingRect() const override;

    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = nullptr) override;

    void setColor(unsigned int color);

private:
    /* data */
    unsigned int m_selected_color;
};

class Rectangle : public GeometryItem {
private:
    /* data */
public:
    explicit Rectangle(QGraphicsItem* parent = nullptr);

    ~Rectangle() override;
};

class Circle : public GeometryItem {
public:
    explicit Circle(QGraphicsItem* parent = nullptr);

    ~Circle() override;
};

class Polygon : public GeometryItem {
public:
    explicit Polygon(const QPolygonF& polygon, QGraphicsItem* parent = nullptr);

    ~Polygon() override;
};

class CurvePolygon : public GeometryItem {
public:
    explicit CurvePolygon(QGraphicsItem* parent = nullptr);

    ~CurvePolygon() override;
};

#endif  // REDEDA_GEOMETRYITEM_H
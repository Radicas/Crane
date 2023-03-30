#ifndef REDEDA_GEOMETRYITEM_H
#define REDEDA_GEOMETRYITEM_H

#include "algorithm/geometry/polygon.h"
#include "shape.h"

#include <QGraphicsItem>

class GeometryItem : public QGraphicsItem, public Shape {
public:
    explicit GeometryItem(QGraphicsItem* parent = nullptr);

    ~GeometryItem() override;

    QRectF boundingRect() const override;

    QPainterPath shape() const override;

    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = nullptr) override;

    void updatePos(QPointF delta);

    void setColor(unsigned int color);

private:
    /* data */
    unsigned int m_selected_color;

protected:
    /* methods */
    void initPathByPolygonWithHoles(const R_GEOMETRY::PolygonWithHoles& poly);
};

class RectangleItem : public GeometryItem {
private:
    /* data */
public:
    explicit RectangleItem(QGraphicsItem* parent = nullptr);

    ~RectangleItem() override;
};

class CircleItem : public GeometryItem {
public:
    explicit CircleItem(QGraphicsItem* parent = nullptr);

    ~CircleItem() override;
};

class PolygonItem : public GeometryItem {
public:
    explicit PolygonItem(R_GEOMETRY::PolygonWithHoles polygon, QGraphicsItem* parent = nullptr);

    ~PolygonItem() override;
};

class CurvePolygonItem : public GeometryItem {
public:
    explicit CurvePolygonItem(QGraphicsItem* parent = nullptr);

    ~CurvePolygonItem() override;
};

#endif  // REDEDA_GEOMETRYITEM_H
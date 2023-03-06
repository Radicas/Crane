#ifndef REDEDA_GEOMETRYITEM_H
#define REDEDA_GEOMETRYITEM_H

#include "shape.h"
#include <QGraphicsItem>

class GeometryItem: public QGraphicsItem, Shape
{
public:
    explicit GeometryItem(QGraphicsItem* parent = nullptr);
    ~GeometryItem() override;
    QRectF boundingRect() const override;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;
private:
    /* data */
};

class Rectangle: public GeometryItem
{
private:
    /* data */
public:
    explicit Rectangle(QGraphicsItem* parent = nullptr);
    ~Rectangle() override;
};




#endif // REDEDA_GEOMETRYITEM_H
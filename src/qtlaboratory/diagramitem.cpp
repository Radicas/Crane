#include "diagramitem.h"

#include <QGraphicsScene>
#include <QGraphicsSceneContextMenuEvent>
#include <QMenu>
#include <QPainter>

DiagramItem::DiagramItem(DiagramType diagramType, QGraphicsItem* parent)
    : QGraphicsPolygonItem(parent)
{
    m_diagram_type = diagramType;
    QPainterPath path;
    switch (m_diagram_type)
    {
    case Rectangle:
        m_polygon << QPointF(-100, -100) << QPointF(100, -100)
            << QPointF(100, 100) << QPointF(-100, 100)
            << QPointF(-100, -100);
        break;
    case RoundRect:
        path.moveTo(200, 50);
        path.arcTo(150, 0, 50, 50, 0, 90);
        path.arcTo(50, 0, 50, 50, 90, 90);
        path.arcTo(50, 50, 50, 50, 180, 90);
        path.arcTo(150, 50, 50, 50, 270, 90);
        path.lineTo(200, 25);
        m_polygon = path.toFillPolygon();
        break;
    case Circle:
        path.moveTo(100, 0);
        path.arcTo(-100, -100, 200, 200, 0, 360);
        m_polygon = path.toFillPolygon();
        break;
    default:
        m_polygon << QPointF(-200, 0)
            << QPointF(-75, -75)
            << QPointF(-25, -25)
            << QPointF(25, -75)
            << QPointF(45, 30)
            << QPointF(-25, 75)
            << QPointF(-200, 0);
        break;
    }
    setPolygon(m_polygon);
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
    setPen(QPen(Qt::yellow));
    setBrush(QBrush(Qt::green));
}
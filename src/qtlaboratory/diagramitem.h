#ifndef DIAGRAMITEM_H
#define DIAGRAMITEM_H

#include <QGraphicsItem>
#include <QList>

QT_BEGIN_NAMESPACE
class QGraphicsItem;
class QGraphicsScene;
class QPolygonF;
QT_END_NAMESPACE

class DiagramItem : public QGraphicsItem {
public:
    enum { Type = UserType + 15 };
    enum DiagramType {
        Rectangle,
        RoundRect,
        Circle,
        Polygon,
    };

    DiagramItem( DiagramType diagramType, QGraphicsItem* parent = 0 );

    DiagramType diagramType() const {
        return m_diagram_type;
    }
    QPolygonF polygon() const {
        return m_polygon;
    }
    int type() const override {
        return Type;
    }

private:
    DiagramType m_diagram_type;
    QPolygonF   m_polygon;
};

#endif  // DIAGRAMITEM_H

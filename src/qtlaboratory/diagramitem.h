#ifndef DIAGRAMITEM_H
#define DIAGRAMITEM_H

#include <QGraphicsPixmapItem>
#include <QList>

QT_BEGIN_NAMESPACE
class QPixmap;
class QGraphicsItem;
class QGraphicsScene;
class QTextEdit;
class QGraphicsSceneMouseEvent;
class QMenu;
class QGraphicsSceneContextMenuEvent;
class QPainter;
class QStyleOptionGraphicsItem;
class QWidget;
class QPolygonF;
QT_END_NAMESPACE

class DiagramItem : public QGraphicsPolygonItem
{
public:
    enum
    {
        Type = UserType + 15
    };
    enum DiagramType
    {
        Step,
        Conditional,
        StartEnd,
        Io,
        Circle
    };

    DiagramItem(DiagramType diagramType, QMenu* contextMenu, QGraphicsItem* parent = 0);

    DiagramType diagramType() const { return myDiagramType; }
    QPolygonF polygon() const { return myPolygon; }
    QPixmap image() const;
    int type() const override { return Type; }

protected:
    void contextMenuEvent(QGraphicsSceneContextMenuEvent* event) override;

private:
    DiagramType myDiagramType;
    QPolygonF myPolygon;
    QMenu* myContextMenu;
};

#endif // DIAGRAMITEM_H

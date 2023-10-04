#ifndef REDEDA_LABSCENE_H
#define REDEDA_LABSCENE_H

#include <QGraphicsScene>
#include <vector>
class GeometryItem;
class LabScene : public QGraphicsScene {
public:
    explicit LabScene(QObject* parent = nullptr);

    ~LabScene() override;

    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;

    void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override;

    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;

    void updateItem(QGraphicsItem* item);

private:
    /* data */
    QGraphicsItem* _selected_item;
};

#endif // REDEDA_LABSCENE_H
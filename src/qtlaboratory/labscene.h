#ifndef REDEDA_LABSCENE_H
#define REDEDA_LABSCENE_H

#include <QGraphicsScene>
#include <QList>

class LabScene : public QGraphicsScene {

public:
    explicit LabScene(QObject* parent = nullptr);

    ~LabScene() override;

    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;

    void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override;

    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;

private:
    /* data */
    QList<QGraphicsItem*> m_select_items;
    QGraphicsItem*        m_curr_item;
};

#endif  // REDEDA_LABSCENE_H
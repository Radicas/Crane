#ifndef REDEDA_LABSCENE_H
#define REDEDA_LABSCENE_H

#include <QGraphicsScene>
#include <QVector>

class GeometryItem;
class LabScene : public QGraphicsScene {

public:
    explicit LabScene(QObject* parent = nullptr);

    ~LabScene() override;

    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;

    void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override;

    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;

    QVector<QGraphicsItem*>& getSelectedItems();

    void updateItem(QGraphicsItem* item);

private:
    /* data */
    QVector<QGraphicsItem*> m_select_items;  ///< 被选中的对象
    GeometryItem*           m_curr_item;     ///< 当前处理的对象
    QPointF                 m_item_offset;   ///< 对象偏移量，用于移动
    QPointF                 m_press_pos;
    QPointF                 m_release_pos;
    QPointF                 m_marked;     ///< 当前点击的位置，用于判断是否框选
    bool                    m_is_moving;  ///< 是否正在移动
};

#endif  // REDEDA_LABSCENE_H
//
// Created by Radica on 2023/3/21.
//

#ifndef CRANE_LABVIEW_H
#define CRANE_LABVIEW_H

#include <QGraphicsView>

class LabView : public QGraphicsView {
public:
    explicit LabView(QGraphicsScene* scene, QWidget* parent = nullptr);

    ~LabView() override;

    void wheelEvent(QWheelEvent* event) override;

protected:
    void mousePressEvent(QMouseEvent* event) override;

    void mouseMoveEvent(QMouseEvent* event) override;

    void mouseReleaseEvent(QMouseEvent* event) override;

private:
    QPoint lastMousePos; // 上一个鼠标位置
};

#endif // CRANE_LABVIEW_H

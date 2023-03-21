//
// Created by Radica on 2023/3/21.
//
#include "labview.h"
#include <QDebug>
#include <QScrollBar>
#include <QWheelEvent>

LabView::LabView(QGraphicsScene* scene, QWidget* parent)
    : QGraphicsView(scene, parent) {
    setViewportUpdateMode(QGraphicsView::ViewportUpdateMode::FullViewportUpdate);
}


LabView::~LabView() = default;

void LabView::wheelEvent(QWheelEvent* event) {
    double scaleFactor = 1.15;
    if (event->angleDelta().y() < 0) {
        scaleFactor = 1.0 / scaleFactor;
    }
    scale(scaleFactor, scaleFactor);
    event->accept();
}
void LabView::mousePressEvent(QMouseEvent* event) {
    if (event->button() == Qt::MiddleButton) {
        setCursor(Qt::ClosedHandCursor);  // 设置鼠标样式为关闭手指
        lastMousePos = event->pos();      // 记录上一个鼠标位置
    }
    else {
        QGraphicsView::mousePressEvent(event);  // 调用父类函数处理事件
    }
}

void LabView::mouseMoveEvent(QMouseEvent* event) {
    if (event->buttons() & Qt::MiddleButton) {
        QPointF delta = mapToScene(event->pos()) - mapToScene(lastMousePos);          // 计算移动距离
        lastMousePos  = event->pos();                                                 // 更新上一个鼠标位置
        horizontalScrollBar()->setValue(horizontalScrollBar()->value() - delta.x());  // 水平方向滚动条移动
        verticalScrollBar()->setValue(verticalScrollBar()->value() - delta.y());      // 垂直方向滚动条移动
    }
    else {
        QGraphicsView::mouseMoveEvent(event);  // 调用父类函数处理事件
    }
}

void LabView::mouseReleaseEvent(QMouseEvent* event) {
    if (event->button() == Qt::MiddleButton) {
        setCursor(Qt::ArrowCursor);  // 设置鼠标样式为箭头
    }
    else {
        QGraphicsView::mouseReleaseEvent(event);  // 调用父类函数处理事件
    }
}

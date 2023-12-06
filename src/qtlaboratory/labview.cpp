#include "labview.h"

LabView::LabView(QWidget* parent) :
    QGraphicsView(parent) {
    initView();
}

void LabView::mousePressEvent(QMouseEvent* event) {
    QGraphicsView::mousePressEvent(event);
}

/* region Private Methods */
void LabView::initView() {
    setRenderHint(QPainter::Antialiasing);      // 抗锯齿渲染
    setDragMode(QGraphicsView::RubberBandDrag); // 拖拽模式为选框拖拽

    // 启用滚轮缩放功能
    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    setResizeAnchor(QGraphicsView::AnchorUnderMouse);
    setDragMode(QGraphicsView::ScrollHandDrag);
    setInteractive(true);
    setRenderHint(QPainter::Antialiasing);
    setRenderHint(QPainter::HighQualityAntialiasing);
    setRenderHint(QPainter::SmoothPixmapTransform);
}
/* endregion */
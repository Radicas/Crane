#include "labscene.h"

#include <QDebug>
#include <QGraphicsSceneMouseEvent>
#include <QPainterPath>
#include <QRectF>
#include <QGraphicsItem>

LabScene::LabScene(QObject* parent) :
    QGraphicsScene(parent),
    _selected_item(nullptr) {
    setSceneRect(-5000, -5000, 10000, 10000);
    setSelectionArea(QPainterPath(), Qt::IntersectsItemShape);
}

LabScene::~LabScene() {
}

void LabScene::mousePressEvent(QGraphicsSceneMouseEvent* event) {
    _selected_item = itemAt(event->scenePos(), QTransform());
}

void LabScene::mouseMoveEvent(QGraphicsSceneMouseEvent* event) {
    if (_selected_item) {
        _selected_item->setPos(event->scenePos());
    }
}

void LabScene::mouseReleaseEvent(QGraphicsSceneMouseEvent* event) {
    _selected_item = nullptr;
}
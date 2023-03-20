#include "labscene.h"
#include "geometryitem.h"
#include "Craneglobal.h"

#include <QDebug>
#include <QGraphicsSceneMouseEvent>
#include <QPainterPath>
#include <QRectF>

LabScene::LabScene(QObject* parent)
    : QGraphicsScene(parent)
    , m_select_items({})
    , m_curr_item(nullptr) {
    setSceneRect(-5000, -5000, 10000, 10000);
    setSelectionArea(QPainterPath(), Qt::IntersectsItemShape);
}

LabScene::~LabScene() {
    m_select_items.clear();
}

void LabScene::mousePressEvent(QGraphicsSceneMouseEvent* event) {
    m_curr_item = itemAt(event->scenePos(), QTransform());
    if (m_curr_item && !m_select_items.contains(m_curr_item)) {
        m_curr_item->setSelected(true);
        m_select_items.append(m_curr_item);
    }
//    dynamic_cast<GeometryItem*>(m_curr_item)->setBrush(Crane::COLOR::GRAY);
}

void LabScene::mouseMoveEvent(QGraphicsSceneMouseEvent* event) {
    if (m_curr_item) {
        m_curr_item->setPos(event->scenePos());
    }
}

void LabScene::mouseReleaseEvent(QGraphicsSceneMouseEvent* event) {
    if (m_curr_item) {
//        dynamic_cast<GeometryItem*>(m_curr_item)->setBrush(Crane::COLOR::BLUE);
    }
}

#include "labscene.h"
#include "craneglobal.h"
#include "geometryitem.h"

#include <QDebug>
#include <QGraphicsSceneMouseEvent>
#include <QPainterPath>
#include <QRectF>

LabScene::LabScene(QObject* parent) :
    QGraphicsScene(parent),
    m_select_items({}),
    m_curr_item(nullptr),
    m_is_moving(false),
    m_item_offset(),
    m_press_pos(),
    m_release_pos(),
    m_marked() {
    setSceneRect(-5000, -5000, 10000, 10000);
    setSelectionArea(QPainterPath(), Qt::IntersectsItemShape);
}

LabScene::~LabScene() {
    m_select_items.clear();
}

void LabScene::mousePressEvent(QGraphicsSceneMouseEvent* event) {
    m_press_pos = event->scenePos();
    m_release_pos = m_press_pos;
    auto* g_item = itemAt(event->scenePos(), QTransform());
    m_curr_item = dynamic_cast<GeometryItem*>(g_item);
    if (m_curr_item && !m_select_items.contains(m_curr_item)) {
        m_curr_item->setSelected(true);
        m_select_items.append(m_curr_item);
    }
    if (m_curr_item) {
        m_is_moving = true;
        m_item_offset = m_curr_item->pos() - event->scenePos();
        m_marked = event->scenePos();
        dynamic_cast<GeometryItem*>(m_curr_item)->setColor(Crane::COLOR::GRAY.rgba());
    }
}

void LabScene::mouseMoveEvent(QGraphicsSceneMouseEvent* event) {
    if (m_curr_item && m_is_moving) {
        m_curr_item->setPos(event->scenePos() + m_item_offset);
    }
}

void LabScene::mouseReleaseEvent(QGraphicsSceneMouseEvent* event) {
    m_release_pos = event->scenePos();
    if (m_curr_item) {
        if (event->scenePos() != m_marked) {
            dynamic_cast<GeometryItem*>(m_curr_item)->setColor(Crane::COLOR::BLUE.rgba());
        }
        m_curr_item->updatePos(m_release_pos - m_press_pos);
        m_curr_item = nullptr;
    } else {
        for (auto* item : m_select_items) {
            dynamic_cast<GeometryItem*>(item)->setColor(Crane::COLOR::BLUE.rgba());
            item->update();
        }
        m_select_items.clear();
    }
    m_is_moving = false;
}
QVector<QGraphicsItem*>& LabScene::getSelectedItems() {
    return m_select_items;
}

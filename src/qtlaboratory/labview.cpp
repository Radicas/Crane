#include "labview.h"

#include <QWheelEvent>
#include <QGraphicsSceneWheelEvent>
#include <QGestureEvent>
#include <QScrollBar>
#include <QtMath>

LabView::LabView(QWidget* parent) :
    QGraphicsView(parent) {
    setBackgroundBrush(Qt::black);
    setRenderHint(QPainter::Antialiasing, true);
    setOptimizationFlag(QGraphicsView::DontSavePainterState, true);
    setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    setMouseTracking(true);
}

void LabView::wheelEvent(QWheelEvent* event) {
    if (event->source() == Qt::MouseEventSynthesizedBySystem) {
        QAbstractScrollArea::wheelEvent(event);
    } else {
        QGraphicsView::wheelEvent(event);
    }
}

bool LabView::eventFilter(QObject* object, QEvent* event) {
    switch (event->type()) {
        case QEvent::Gesture: {
            QGestureEvent* ge = dynamic_cast<QGestureEvent*>(event);
            QPinchGesture* pinch_g = dynamic_cast<QPinchGesture*>(ge->gesture(Qt::PinchGesture));
            if (pinch_g) {
                scale(pinch_g->scaleFactor());
                return true;
            }
            break;
        }
        case QEvent::GraphicsSceneMousePress: {
            QGraphicsSceneMouseEvent* e = dynamic_cast<QGraphicsSceneMouseEvent*>(event);
            Q_ASSERT(e);
            if (e->button() == Qt::MiddleButton) {
                mCursorBeforePanning = cursor();
                setCursor(Qt::ClosedHandCursor);
            }
            if (mEventHandlerObject) {
                mEventHandlerObject->graphicsViewEventHandler(event);
            }
            return true;
        }
        case QEvent::GraphicsSceneMouseRelease: {
            QGraphicsSceneMouseEvent* e = dynamic_cast<QGraphicsSceneMouseEvent*>(event);
            Q_ASSERT(e);
            if (e->button() == Qt::MiddleButton) {
                setCursor(mCursorBeforePanning);
            }
            if (mEventHandlerObject) {
                mEventHandlerObject->graphicsViewEventHandler(event);
            }
            return true;
        }
        case QEvent::GraphicsSceneMouseMove: {
            QGraphicsSceneMouseEvent* e = dynamic_cast<QGraphicsSceneMouseEvent*>(event);
            Q_ASSERT(e);
            if (e->buttons().testFlag(Qt::MiddleButton) && (!mPanningActive)) {
                QPoint diff = mapFromScene(e->scenePos()) - mapFromScene(e->buttonDownScenePos(Qt::MiddleButton));
                mPanningActive = true; // avoid recursive calls (=> stack overflow)
                horizontalScrollBar()->setValue(horizontalScrollBar()->value() - diff.x());
                verticalScrollBar()->setValue(verticalScrollBar()->value() - diff.y());
                mPanningActive = false;
            }
            emit cursorScenePosChanged(e->scenePos());
        }
            // fall through
        case QEvent::GraphicsSceneMouseDoubleClick:
        case QEvent::GraphicsSceneContextMenu:
        case QEvent::KeyRelease:
        case QEvent::KeyPress: {
            if (mEventHandlerObject && mEventHandlerObject->graphicsViewEventHandler(event)) {
                return true;
            }
            break;
        }
        case QEvent::GraphicsSceneWheel: {
            if (!underMouse())
                break;
            if (mEventHandlerObject) {
                if (!mEventHandlerObject->graphicsViewEventHandler(event)) {
                    handleMouseWheelEvent(dynamic_cast<QGraphicsSceneWheelEvent*>(event));
                }
            } else {
                handleMouseWheelEvent(dynamic_cast<QGraphicsSceneWheelEvent*>(event));
            }
            return true;
        }
        default:
            break;
    }
    return QWidget::eventFilter(object, event);
}

void LabView::drawBackground(QPainter* painter, const QRectF& rect) {
    QGraphicsView::drawBackground(painter, rect);
    QPen oldPen = painter->pen();
    QPen pen;
    pen.setWidth(0);
    pen.setColor(Qt::white);
    painter->setPen(pen);
    painter->drawLine(-10, 0, 10, 0);
    painter->drawLine(0, -10, 0, 10);
    painter->setPen(oldPen);
}

void LabView::drawForeground(QPainter* painter, const QRectF& rect) {
    QGraphicsView::drawForeground(painter, rect);
}

void LabView::handleMouseWheelEvent(QGraphicsSceneWheelEvent* event) noexcept {
    if (event->modifiers().testFlag(Qt::ShiftModifier)) {
        // horizontal scrolling
        horizontalScrollBar()->setValue(horizontalScrollBar()->value() - event->delta());
    } else if (event->modifiers().testFlag(Qt::ControlModifier)) {
        if (event->orientation() == Qt::Horizontal) {
            // horizontal scrolling
            horizontalScrollBar()->setValue(horizontalScrollBar()->value() - event->delta());
        } else {
            // vertical scrolling
            verticalScrollBar()->setValue(verticalScrollBar()->value() - event->delta());
        }
    } else {
        // Zoom to mouse
        qreal scaleFactor = qPow(sZoomStepFactor, event->delta() / qreal(120));
        scale(scaleFactor);
    }
    event->setAccepted(true);
}

void LabView::scale(qreal factor) noexcept {
    QGraphicsView::scale(factor, factor);
}

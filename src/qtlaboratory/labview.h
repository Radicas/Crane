#ifndef POLYGONOFFSET_GRAPHICSVIEW_H
#define POLYGONOFFSET_GRAPHICSVIEW_H

#include <QGraphicsView>

class IF_GraphicsViewEventHandler {
public:
    // Constructors / Destructor
    explicit IF_GraphicsViewEventHandler() noexcept = default;
    virtual ~IF_GraphicsViewEventHandler() noexcept = default;

    // 提供接口
    virtual bool graphicsViewEventHandler(QEvent* event) = 0;
};

class LabView : public QGraphicsView {
    Q_OBJECT

public:
    explicit LabView(QWidget* parent = nullptr);

Q_SIGNALS:
    void cursorScenePosChanged(const QPointF& pos);

protected:
    void wheelEvent(QWheelEvent* event) override;

    void drawBackground(QPainter* painter, const QRectF& rect) override;

    void drawForeground(QPainter* painter, const QRectF& rect) override;

    bool eventFilter(QObject* object, QEvent* event) override;

private:
    void handleMouseWheelEvent(QGraphicsSceneWheelEvent* event) noexcept;

    void scale(qreal factor) noexcept;

    QScopedPointer<IF_GraphicsViewEventHandler> mEventHandlerObject;

    bool mOriginCrossVisible;
    bool mUseOpenGl;
    volatile bool mPanningActive;
    qreal mMaximumZoomMultiplier; // 当前最大缩放倍数
    qreal mZoomMultiplier;        // 当前缩放倍数
    qreal mMinimumZoomMultiplier; // 当前最小缩放倍数

    QRectF mSceneRectMarker;
    QCursor mCursorBeforePanning;
    // Static Variables
    static constexpr qreal sMinimumZoomMultiplier = 0.5; // 默认最小缩放倍数
    static constexpr qreal sMaximumZoomMultiplier = 500; // 默认最大缩放倍数
    // 最小缩放倍数和最大缩放倍数之间的差距不能小于 100 倍
    static constexpr qreal sDefaultScaleFactor = 10;       // 默认缩放因子
    static constexpr qreal sMultiplierBetweenMinMax = 100; // 最小缩放倍数和最大缩放倍数的差距
    static constexpr qreal sZoomStepFactor = 1.3;          // 单次缩放倍数
};

#endif //POLYGONOFFSET_GRAPHICSVIEW_H

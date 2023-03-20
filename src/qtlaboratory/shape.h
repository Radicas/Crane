/**
 * @file shape.h
 * @author Radica
 * @brief 通用图形类
 * @version 0.1
 * @date 2023-03-01
 *
 * @copyright Copyright (c) 2023
 *
 */
#include <QList>
#include <QPainterPath>

struct POINT {
    double x;
    double y;
};

struct LINE {
    POINT sp;
    POINT ep;
};

struct ARC {
    POINT  sp;
    POINT  ep;
    POINT  cp;
    double sa;
    double swa;
};

class Shape {
public:
    enum Type { Line, Arc };

    struct PathSegment {
        Type    type;
        QPointF startPoint;
        QPointF endPoint;
        QPointF centerPoint;
        qreal   radius;
        qreal   startAngle;
        qreal   spanAngle;

        PathSegment(QPointF sp, QPointF ep)
            : type(Line)
            , startPoint(sp)
            , endPoint(ep)
            , centerPoint()
            , radius()
            , startAngle()
            , spanAngle() {}

        PathSegment(QPointF sp, QPointF ep, QPointF center, qreal radius, qreal sa, qreal swa)
            : type(Arc)
            , startPoint(sp)
            , endPoint(ep)
            , centerPoint(center)
            , radius(radius)
            , startAngle(sa)
            , spanAngle(swa) {}
    };

    Shape();

    ~Shape();

    void addSegment(PathSegment segment);

    void addHole(Shape hole);

    bool contains(const QPointF& point) const;

    Shape intersected(const Shape& other) const;

    Shape united(const Shape& other) const;

    Shape subtracted(const Shape& other) const;

    const QPainterPath& path() const;

protected:
    QPainterPath m_path;
    QList<Shape> m_holes;

private:
};

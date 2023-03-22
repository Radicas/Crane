#include "shape.h"

Shape::Shape()
    : m_path()
    , m_outlines()
    , m_holes()
    , m_path_elements()
    , m_vertex() {}

Shape::~Shape() = default;

void Shape::addSegment(PathElement segment) {
    if (segment.type == Line) {
        m_path.lineTo(segment.endPoint);
    }
    else if (segment.type == Arc) {
        m_path.arcTo(segment.centerPoint.x() - segment.radius, segment.centerPoint.y() - segment.radius,
                     segment.radius * 2, segment.radius * 2, segment.startAngle, segment.spanAngle);
    }
}

void Shape::addHole( Shape hole ) {
    QPainterPath holePath;
    holePath.addPath( hole.m_path );
    m_path.addPath( holePath.toReversed() );
}

bool Shape::contains( const QPointF& point ) const {
    return m_path.contains( point );
}

Shape Shape::intersected( const Shape& other ) const {
    Shape result;
    result.m_path = m_path.intersected( other.m_path );
    return result;
}

Shape Shape::united( const Shape& other ) const {
    Shape result;
    result.m_path = m_path.united( other.m_path );
    return result;
}

Shape Shape::subtracted( const Shape& other ) const {
    Shape result;
    result.m_path = m_path.subtracted( other.m_path );
    return result;
}

const QPainterPath& Shape::path() const {
    return m_path;
}

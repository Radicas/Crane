#include "shape.h"
Shape::Shape() :
    m_path(),
    m_polygon_with_holes(),
    m_vertex() {
}

Shape::~Shape() = default;

void Shape::addHole(const Shape& hole) {
}

bool Shape::contains(const POINT& point) const {
    return false;
}

Shape Shape::intersected(const Shape& other) const {
    return {};
}

Shape Shape::united(const Shape& other) const {
    return {};
}

Shape Shape::subtracted(const Shape& other) const {
    return {};
}

const QPainterPath& Shape::path() const {
    return m_path;
}

geometry::POLYGON& Shape::getOuter() {
    return const_cast<geometry::POLYGON&>(m_polygon_with_holes.outer());
}
std::vector<geometry::POLYGON>& Shape::getInner() {
    return const_cast<std::vector<geometry::POLYGON>&>(m_polygon_with_holes.inner());
}
const geometry::PolygonWithHoles& Shape::getPolygonWithHoles() const {
    return m_polygon_with_holes;
}

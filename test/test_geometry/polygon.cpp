//
// Created by Radica on 2023/3/24.
//

#include "polygon.h"
#include "iostream"

namespace geometry {

PolygonWithHoles::PolygonWithHoles()
    : m_outer()
    , m_inner() {}

PolygonWithHoles::~PolygonWithHoles() = default;

const Polygon& PolygonWithHoles::outer() const {
    return m_outer;
}

const std::vector<Polygon>& PolygonWithHoles::inner() const {
    return m_inner;
}

void PolygonWithHoles::setOuter(const Polygon& aOuter) {
    m_outer = aOuter;
}

void PolygonWithHoles::setInner(const std::vector<Polygon>& aInner) {
    m_inner = aInner;
}
void PolygonWithHoles::updatePos(const POINT& delta) {
    for (auto& p : m_outer) {
        p.x += delta.x;
        p.y += delta.y;
    }
    for (auto& inners : m_inner) {
        for (auto& inner : inners) {
            inner.x += delta.x;
            inner.y += delta.y;
        }
    }
}
};  // namespace geometry

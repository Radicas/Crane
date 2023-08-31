//
// Created by Radica on 2023/3/24.
//

#include "polygon.h"

#include <utility>

namespace geometry {

PolygonWithHoles::PolygonWithHoles() :
    m_outer(),
    m_inner() {
}

PolygonWithHoles::PolygonWithHoles(geometry::POLYGON outer, std::vector<geometry::POLYGON> inners) :
    m_outer(std::move(outer)),
    m_inner(std::move(inners)) {
}

PolygonWithHoles::~PolygonWithHoles() = default;

const geometry::POLYGON& PolygonWithHoles::outer() const {
    return m_outer;
}

const std::vector<geometry::POLYGON>& PolygonWithHoles::inner() const {
    return m_inner;
}

void PolygonWithHoles::setOuter(const geometry::POLYGON& aOuter) {
    m_outer = aOuter;
}

void PolygonWithHoles::setInner(const std::vector<geometry::POLYGON>& aInner) {
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
}; // namespace geometry

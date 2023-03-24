//
// Created by Radica on 2023/3/24.
//

#include "polygon.h"

namespace R_GEOMETRY {

//Polygon::Polygon() {}
//
//Polygon::Polygon(std::vector<POINT> points) {
//    m_points = points;
//}
//Polygon::Polygon(std::vector<POINT>& points) {
//    m_points = points;
//}
//inline Polygon& Polygon::operator<<(const POINT& p) {
//    m_points.emplace_back(p);
//    return *this;
//}
//POINT& Polygon::operator[](int index) {
//    if (!m_points.empty()) {
//        return m_points[index];
//    }
//    std::cerr << "index of polygon out of range!" << std::endl;
//    exit(1);
//}
//Polygon& Polygon::operator=(const Polygon& poly) {
//    m_points = poly.m_points;
//    return *this;
//}
//const POINT& Polygon::first() const {
//    if (!m_points.empty()) {
//        return m_points.at(0);
//    }
//    std::cerr << "index of polygon out of range!" << std::endl;
//    exit(1);
//}
//const POINT& Polygon::at(int index) const {
//    if (!m_points.empty()) {
//        return m_points.at(index);
//    }
//    std::cerr << "index of polygon out of range!" << std::endl;
//    exit(1);
//}
//int Polygon::size() const {
//    return m_points.size();
//}

PolygonWithHoles::PolygonWithHoles()
    : m_outer()
    , m_inner() {}

PolygonWithHoles::~PolygonWithHoles() = default;

Polygon& PolygonWithHoles::outer() {
    return m_outer;
}

std::vector<Polygon>& PolygonWithHoles::inner() {
    return m_inner;
}

void PolygonWithHoles::setOuter(const Polygon& aOuter) {
    m_outer = aOuter;
}

void PolygonWithHoles::setInner(const std::vector<Polygon>& aInner) {
    m_inner = aInner;
}
};  // namespace R_GEOMETRY

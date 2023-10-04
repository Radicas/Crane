#ifndef GEOMETRY_ALGO_CORE_H
#define GEOMETRY_ALGO_CORE_H

#include <math.h>
#include <vector>

namespace geometry {

const double G_EP = 1E-10;

struct POINT {
    /* Data */
    double x;
    double y;

    /* Methods */
    POINT() :
        x(0.0f),
        y(0.0f) {
    }

    POINT(double a, double b) {
        x = a;
        y = b;
    }

    POINT(const POINT& rhs) :
        x(rhs.x),
        y(rhs.y) {
    }

    POINT& operator+=(const POINT& rhs) {
        x += rhs.x;
        y += rhs.y;
        return *this;
    }

    POINT& operator-=(const POINT& rhs) {
        x -= rhs.x;
        y -= rhs.y;
        return *this;
    }

    POINT& operator-() {
        x = -x;
        y = -y;
        return *this;
    }

    POINT operator+(const POINT& rhs) {
        return {x + rhs.x, y + rhs.y};
    }

    POINT operator-(const POINT& rhs) const {
        return {x - rhs.x, y - rhs.y};
    }

    POINT operator*(double scalar) {
        return {x * scalar, y * scalar};
    }

    POINT operator/(double scalar) {
        return {x / scalar, y / scalar};
    }
};

typedef std::vector<POINT> POLYGON;

typedef POINT VECTOR2D;

struct SEGMENT {
    /* Data */
    POINT _start;
    POINT _end;
};

/**
 * @brief 两个相邻向量，以及两个向量组成的是否凸点
 *
 *           /\ 凸点
 *          /  \
 *         /    \______
 *         |     凹点  |
 *         |___________|
 */
struct ADJACENT_VECTOR2D {
    VECTOR2D _v1;
    VECTOR2D _v2;
    bool _is_convex;
    ADJACENT_VECTOR2D(const VECTOR2D& aVector, const VECTOR2D& bVector, bool convex = false) :
        _v1(aVector),
        _v2(bVector),
        _is_convex(convex) {
        //     // _v1 = aVector;
        //     // _v2 = bVector;
        //     // _is_convex = convex;
    }
};

/**
* @brief 向量的差积
*        r = (sp-op)和(ep-op)的叉积
*        r>0：ep在矢量 op sp 的逆时针方向
*        r=0：op sp ep 三点共线
*        r<0：ep在矢量 op sp 的顺时针方向
* @param sp
* @param ep
* @param op
* @return double 差积
*/
template<typename T>
double multiply(T sp, T ep, T op) {
    return ((sp.x - op.x) * (ep.y - op.y) - (ep.x - op.x) * (sp.y - op.y));
}

/**
 * @brief 向量的差积
 * @tparam T
 * @param v1
 * @param v2
 * @return
 */
template<typename T>
double multiply(T v1, T v2) {
    return v1.x * v2.y - v1.y * v2.x;
}

/**
* @brief 计算向量的单位向量
*        u = v / ||v||
* @param v 向量
* @return T 单位向量
*/
template<typename T>
T normalize(const T& v) {
    double len = sqrt(v.x * v.x + v.y * v.y);
    return T(v.x / len, v.y / len);
}

} // namespace geometry

#endif // GEOMETRY_ALGO_CORE_H
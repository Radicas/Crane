/**
 * @file geometry_algo.h
 * @author Radica
 * @brief 通用几何算法库 Generic Geometry Algorithm Library
 * @version 0.1
 * @date 2023-03-23
 *
 * @copyright Copyright (c) 2023
 *
 */
#ifndef CRANE_GEOMETRY_ALGO_H
#define CRANE_GEOMETRY_ALGO_H

#include <vector>

namespace R_GEOMETRY {

/* 常用的常量定义 */
const double INF  = 1E200;       // infinity 无穷大
const double EP   = 1E-10;       // Epsilon 很小的值，浮点数误差小于它则认为是相等
const int    MAXV = 300;         // max value 最大范围
const double PI   = 3.14159265;  // 圆周率

/**
 * @brief
 *
 */
struct POINT {
    double x;
    double y;
    explicit POINT(double a = 0, double b = 0);
    POINT operator-(const POINT& p) const;
    POINT operator+(const POINT& p) const;
    POINT operator*(double d) const;
};

typedef POINT VECTOR2D;

/**
 * @brief
 *
 */
struct SEGMENT {
    POINT s;
    POINT e;
    SEGMENT(POINT a, POINT b);
};

/**
 * @brief
 *
 */
struct LINE {
    double a;
    double b;
    double c;
    explicit LINE(double d1 = 1, double d2 = -1, double d3 = 0);
};

/**
 * @brief
 *
 */
enum class DIRECTION {
    CW = 0,
    CCW,
};

/**
 * @brief
 *
 */
struct ARC {
    POINT     c;
    double    r;
    POINT     st;
    POINT     et;
    double    sta;
    double    spa;
    DIRECTION d;
    ARC(POINT c, double r, POINT st, POINT et, double sta, double spa, DIRECTION d);
};

/**
 * @brief
 *
 * @param p1
 * @param p2
 * @return double
 */
double dist(POINT p1, POINT p2);

/**
 * @brief
 *
 * @param p1
 * @param p2
 * @param p0
 * @return double
 */
double dotmultiply(POINT p1, POINT p2, POINT p0);

/**
 * @brief
 *
 * @param sp
 * @param ep
 * @param op
 * @return double
 */
double crossmultiply(POINT sp, POINT ep, POINT op);

/**
 * @brief
 *
 * @param sp
 * @param ep
 * @return double
 */
double module(POINT sp, POINT ep);

/**
 * @brief
 *
 * @param v
 * @return double
 */
double module(VECTOR2D v);

/**
 * @brief
 *
 * @param v
 * @return VECTOR2D
 */
VECTOR2D normalize(VECTOR2D v);

/**
 * @brief
 *
 * @param p
 * @param l
 * @return double
 */
double p_seg_relation(POINT p, SEGMENT l);

/**
 * @brief
 *
 * @param p
 * @param l
 * @return POINT
 */
POINT perpendicular(POINT p, SEGMENT l);

/**
 * @brief
 *
 * @param p
 * @param l
 * @param np
 * @return double
 */
double p_seg_dist(POINT p, SEGMENT l, POINT& np);

/**
 * @brief
 *
 * @param u
 * @param v
 * @return true
 * @return false
 */
bool intersect(SEGMENT u, SEGMENT v);

/**
 * @brief
 *
 * @param u
 * @param v
 * @param l
 * @return true
 * @return false
 */
bool intersect(LINE u, LINE v, POINT& l);

/**
 * @brief
 *
 * @param a
 * @param b
 * @return LINE
 */
LINE p2line(POINT a, POINT b);

/**
 * @brief
 *
 * @param ap
 * @param bp
 * @param angle
 * @return true
 * @return false
 */
bool included_angle(POINT ap, POINT bp, double& angle);

/**
 * @brief
 *
 * @param p1
 * @param p2
 * @param op
 * @return double
 */
double sweep_angle(POINT p1, POINT p2, POINT op);

/**
 * @brief
 *
 * @param p
 * @param a
 * @param np
 * @return double
 */
double p_arc_dist(POINT p, ARC a, POINT& np);

/**
 * @brief
 *
 * @param o
 * @param r
 * @param p
 * @return true
 * @return false
 */
bool pincircle(POINT o, double r, POINT p);

/**
 * @brief
 *
 * @param p1
 * @param r1
 * @param p2
 * @param r2
 * @return int
 */
int circlerelation(POINT p1, double r1, POINT p2, double r2);

/**
 * @brief
 *
 * @param p1
 * @param p2
 * @param p3
 * @param r
 * @return POINT
 */
POINT trip2circle(POINT p1, POINT p2, POINT p3, double& r);

/**
 * @brief
 *
 * @param cx
 * @param cy
 * @param r
 * @param startAngle
 * @param endAngle
 * @param numSegments
 * @return std::vector<POINT>
 */
std::vector<POINT> arc2segments(double cx, double cy, double r, double startAngle, double endAngle, int numSegments);
}  // namespace R_GEOMETRY

#endif  // CRANE_GEOMETRY_ALGO_H
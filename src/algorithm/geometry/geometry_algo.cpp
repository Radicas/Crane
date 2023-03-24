#include "geometry_algo.h"

#include <cmath>
#include <vector>

namespace R_GEOMETRY {

/*
 * 专业名词介绍
 *
 * Delta(Δ):
 * 表示两个数之间的差异或变化量，通常用于比较两个数的变化大小。在计算机中，Delta通常用于表示精度或误差，例如比较两个浮点数的差值是否小于Delta。
 * Tolerance(公差):
 * 表示允许的误差范围或容忍度，通常用于判断一个值是否在指定的范围内。在计算机中，Tolerance通常用于比较两个浮点数的大小，或者判断一个值是否等于另一个值，例如在图形学中，判断一个点是否在指定的区域内。
 * Precision(精度):
 * 表示一个量的精确程度或准确度，通常用于描述一个测量或计算结果的可靠程度。在计算机中，Precision通常用于表示浮点数的位数或有效数字位数，例如单精度浮点数的精度为7位，双精度浮点数的精度为15位。
 * Error(误差):
 * 表示测量或计算结果与真实值之间的差异或偏差，通常用于描述一个测量或计算的准确度。在计算机中，Error通常用于表示程序运行时出现的错误或异常，例如除零错误、内存溢出等。
 */

double dist(POINT p1, POINT p2) {
    return (sqrt((p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y)));
}

double dotmultiply(POINT p1, POINT p2, POINT p0) {
    return ((p1.x - p0.x) * (p2.x - p0.x) + (p1.y - p0.y) * (p2.y - p0.y));
}

double crossmultiply(POINT sp, POINT ep, POINT op) {
    return ((sp.x - op.x) * (ep.y - op.y) - (ep.x - op.x) * (sp.y - op.y));
}

double module(POINT sp, POINT ep) {
    return sqrt((ep.x - sp.x) * (ep.x - sp.x) + (ep.y - sp.y) * (ep.y - sp.y));
}

double module(VECTOR2D v) {
    return std::sqrt(v.x * v.x + v.y * v.y);
}

VECTOR2D normalize(VECTOR2D v) {
    double len = module(v);
    return VECTOR2D(v.x / len, v.y / len);
}

double p_seg_relation(POINT p, SEGMENT l) {
    SEGMENT tl(l.s, p);
    return dotmultiply(tl.e, l.e, l.s) / (dist(l.s, l.e) * dist(l.s, l.e));
}

POINT perpendicular(POINT p, SEGMENT l) {
    double r = p_seg_relation(p, l);
    POINT  tp;
    tp.x = l.s.x + r * (l.e.x - l.s.x);
    tp.y = l.s.y + r * (l.e.y - l.s.y);
    return tp;
}

double p_seg_dist(POINT p, SEGMENT l, POINT& np) {
    double r = p_seg_relation(p, l);
    if (r < 0) {
        np = l.s;
        return dist(p, l.s);
    }
    if (r > 1) {
        np = l.e;
        return dist(p, l.e);
    }
    np = perpendicular(p, l);
    return dist(p, np);
}

bool intersect(SEGMENT u, SEGMENT v) {
    return ((std::fmax(u.s.x, u.e.x) >= std::fmin(v.s.x, v.e.x)) &&  // 排斥实验
            (std::fmax(v.s.x, v.e.x) >= std::fmin(u.s.x, u.e.x)) && (std::fmax(u.s.y, u.e.y) >= std::fmin(v.s.y, v.e.y))
            && (std::fmax(v.s.y, v.e.y) >= std::fmin(u.s.y, u.e.y))
            && (crossmultiply(v.s, u.e, u.s) * crossmultiply(u.e, v.e, u.s) >= 0) &&  // 跨立实验
            (crossmultiply(u.s, v.e, v.s) * crossmultiply(v.e, u.e, v.s) >= 0));
}

bool intersect(LINE u, LINE v, POINT& l) {
    bool res = u.a * v.b != v.a * u.b;
    if (res) {
        l.x = (u.b * v.c - v.b * u.c) / (u.a * v.b - v.a * u.b);
        l.y = (v.a * u.c - u.a * v.c) / (u.a * v.b - v.a * u.b);
    }
    return res;
}

LINE p2line(POINT a, POINT b) {
    return LINE(b.y - a.y, a.x - b.x, b.x * a.y - a.x * b.y);
}

bool included_angle(POINT ap, POINT bp, double& angle) {
    if (bp.x == ap.x)
        return false;
    angle = atan((bp.y - ap.y) / (bp.x - ap.x)) * 180 / PI;
    return true;
}

double sweep_angle(POINT p1, POINT p2, POINT op) {
    double theta = atan2(p1.x - op.x, p1.y - op.y) - atan2(p2.x - op.x, p2.y - op.y);
    if (theta > M_PI)
        theta -= 2 * M_PI;
    if (theta < -M_PI)
        theta += 2 * M_PI;

    theta = abs(theta * 180.0 / M_PI);
    return theta;
}

double p_arc_dist(POINT p, ARC a, POINT& np) {
    SEGMENT se(a.st, a.et);
    double  spdist = dist(a.st, p);
    double  epdist = dist(a.et, p);
    if (intersect(p2line(a.c, p), p2line(a.st, a.et), np)) {
        auto r = p_seg_relation(np, se);
        if (r >= 0 && r <= 1 && dotmultiply(p, np, a.c) > 0) {
            auto cp(p - a.c);
            auto normalCP = normalize(cp);
            np            = (normalCP * a.r) + a.c;
            return std::abs(module(p, a.c) - a.r);
        }
    }
    np = spdist < epdist ? a.st : a.et;
    return std::fmin(spdist, epdist);
}

bool pincircle(POINT o, double r, POINT p) {
    double d2 = (p.x - o.x) * (p.x - o.x) + (p.y - o.y) * (p.y - o.y);
    double r2 = r * r;
    return d2 < r2 || abs(d2 - r2) < EP;
}

int circlerelation(POINT p1, double r1, POINT p2, double r2) {
    double d = sqrt((p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y));

    if (fabs(d - r1 - r2) < EP)  // 必须保证前两个if先被判定！
        return 2;
    if (fabs(d - fabs(r1 - r2)) < EP)
        return 4;
    if (d > r1 + r2)
        return 1;
    if (d < fabs(r1 - r2))
        return 5;
    if (fabs(r1 - r2) < d && d < r1 + r2)
        return 3;
    return 0;  // indicate an error!
}

POINT trip2circle(POINT p1, POINT p2, POINT p3, double& r) {
    double x1, y1, x2, y2, x3, y3;
    double a, b, c, g, e, f;
    x1       = p1.x;
    y1       = p1.y;
    x2       = p2.x;
    y2       = p2.y;
    x3       = p3.x;
    y3       = p3.y;
    e        = 2 * (x2 - x1);
    f        = 2 * (y2 - y1);
    g        = x2 * x2 - x1 * x1 + y2 * y2 - y1 * y1;
    a        = 2 * (x3 - x2);
    b        = 2 * (y3 - y2);
    c        = x3 * x3 - x2 * x2 + y3 * y3 - y2 * y2;
    double X = (g * b - c * f) / (e * b - a * f);
    double Y = (a * g - c * e) / (a * f - b * e);
    r        = sqrt((X - x1) * (X - x1) + (Y - y1) * (Y - y1));
    return POINT(X, Y);
}

std::vector<POINT> arc2segments(double cx, double cy, double r, double startAngle, double endAngle, int numSegments) {
    std::vector<POINT> points;
    double             theta        = (endAngle - startAngle) / numSegments;
    double             tan_factor   = tan(theta);
    double             radialFactor = cos(theta);
    double             x            = r * cos(startAngle);
    double             y            = r * sin(startAngle);

    for (int i = 0; i < numSegments; i++) {
        points.emplace_back(cx + x, cy + y);
        double tx = -y;
        double ty = x;
        x += tx * tan_factor;
        y += ty * tan_factor;
        x *= radialFactor;
        y *= radialFactor;
    }
    points.emplace_back(cx + r * cos(endAngle), cy + r * sin(endAngle));
    return points;
}

POINT POINT::operator+(const POINT& p) const {
    return { x + p.x, y + p.y };
}

POINT POINT::operator-(const POINT& p) const {
    return { x - p.x, y - p.y };
}

POINT POINT::operator*(double d) const {
    return { x * d, y * d };
}

SEGMENT::SEGMENT(POINT a, POINT b) {
    s = a;
    e = b;
}

LINE::LINE(double d1, double d2, double d3) {
    a = d1;
    b = d2;
    c = d3;
}

ARC::ARC(POINT c, double r, POINT st, POINT et, double sta, double spa, DIRECTION d) {
    this->c   = c;
    this->r   = r;
    this->st  = st;
    this->et  = et;
    this->sta = sta;
    this->spa = spa;
    this->d   = d;
}
}  // namespace R_GEOMETRY

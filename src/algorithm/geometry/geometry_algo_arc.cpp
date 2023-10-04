#include "geometry_algo_arc.h"

namespace geometry {

double degree2Radian(double angle) {
    return angle * M_PI / 180.0;
}

double radian2degree(double angle) {
    return angle * 180.0 / M_PI;
}

double closestPointToArc(POINT p, ARC arc, POINT& np) {
    // 计算点P到圆心的向量
    double dx = p.x - arc._ct.x;
    double dy = p.y - arc._ct.y;

    // 计算点P到圆心的角度
    double angle = atan2(dy, dx);

    // 角度范围调整到与圆弧起始角度一致
    if (angle < 0)
        angle += 2 * M_PI;
    if (angle < arc._sta)
        angle += 2 * M_PI;
    if (angle > arc._sta + arc._spa)
        angle -= 2 * M_PI;

    // 计算最近点的坐标
    double x = arc._ct.x + arc._r * cos(angle);
    double y = arc._ct.y + arc._r * sin(angle);

    // 更新最近点的坐标
    np.x = x;
    np.y = y;

    // 计算最近点到点P的距离
    double dx2 = p.x - np.x;
    double dy2 = p.y - np.y;
    return sqrt(dx2 * dx2 + dy2 * dy2);
}

bool pincircle(geometry::POINT o, double r, geometry::POINT p) {
    double d2 = (p.x - o.x) * (p.x - o.x) + (p.y - o.y) * (p.y - o.y);
    // 计算圆的半径的平方
    double r2 = r * r;
    // 判断点是否在圆内（包括边界）
    return d2 < r2 || std::abs(d2 - r2) < G_EP;
}

geometry::POINT trip2circle(
    geometry::POINT p1,
    geometry::POINT p2,
    geometry::POINT p3,
    double& r) {
    double x1, y1, x2, y2, x3, y3;
    double a, b, c, g, e, f;
    x1 = p1.x;
    y1 = p1.y;
    x2 = p2.x;
    y2 = p2.y;
    x3 = p3.x;
    y3 = p3.y;
    e = 2 * (x2 - x1);
    f = 2 * (y2 - y1);
    g = x2 * x2 - x1 * x1 + y2 * y2 - y1 * y1;
    a = 2 * (x3 - x2);
    b = 2 * (y3 - y2);
    c = x3 * x3 - x2 * x2 + y3 * y3 - y2 * y2;
    double X = (g * b - c * f) / (e * b - a * f);
    double Y = (a * g - c * e) / (a * f - b * e);
    r = sqrt((X - x1) * (X - x1) + (Y - y1) * (Y - y1));
    return std::move(geometry::POINT(X, Y));
}

QRectF circle2QRectF(geometry::POINT c, double r) {
    return QRectF(c.x - r, -(c.y + r), 2 * r, 2 * r);
}

bool pointIsOnCircle(CIRCLE c, POINT p) {
    return geometry::dist(c.c, p) - c.r < G_EP;
}

std::vector<POINT> tangentPoints(CIRCLE c, POINT p) {
    // 计算点 p 到圆心的距离
    double d = sqrt(pow(p.x - c.c.x, 2) + pow(p.y - c.c.y, 2));
    if (d < c.r) {
        // 点 p 在圆内，不存在切线
        return {};
    } else if (pointIsOnCircle(c, p)) {
        // 点 p 在圆上，返回该点作为切点
        return {p};
    }
    std::vector<POINT> ret;
    VECTOR2 cp = p - c.c;
    double cpAngle = cp.getAngle();
    // alpha 是圆心角
    double alpha = acos(c.r / d) / M_PI * 180.0;
    double p1Angle = cpAngle + alpha;
    double p2Angle = cpAngle - alpha;
    auto p1 = POINT::fromDegree(p1Angle) * c.r + c.c;
    auto p2 = POINT::fromDegree(p2Angle) * c.r + c.c;
    if (pointIsOnCircle(c, p1)) {
        ret.push_back(p1);
    }
    if (pointIsOnCircle(c, p2)) {
        ret.push_back(p2);
    }
    return ret;
}

std::vector<POINT> getIntersection(const LINE& line, const CIRCLE& circle) {
    POINT C = perpendicular(circle.c, line.getALineSeg());
    POINT A = circle.c;
    double c = circle.r;

    double b = dist(A, C);
    if (b - circle.r > G_EP) {
        // 直线与圆相离，无交点
        return {};
    } else if (abs(b - circle.r) < G_EP) {
        // 直线与圆相切，返回切点
        return {C};
    } else {
        // 直线与圆相交，计算交点坐标
        double a = sqrt(c * c - b * b);
        VECTOR2 lineUnitVec =
            normalize(line.getALineSeg()._et - line.getALineSeg()._st);
        return {C + lineUnitVec * a, C - lineUnitVec * a};
    }
}

double squaredDistance(const POINT& p1, const POINT& p2) {
    double rv = std::pow(p1.x - p2.x, 2) + std::pow(p1.y - p2.y, 2);
    if (std::isnan(rv)) {
        return -1;
    }
    return rv;
}

POINT getIntersection(const LINE& l1, const LINE& l2) {
    double denominator = l1.a * l2.b - l1.b * l2.a;

    // 如果分母为 0，则两条直线平行，返回一个特殊值表示无穷远点
    if (denominator == 0) {
        return INVALID_POINT;
    }

    // 计算交点的 x 和 y 坐标
    double x = (l1.b * l2.c - l1.c * l2.b) / denominator;
    double y = (l1.c * l2.a - l1.a * l2.c) / denominator;

    // 返回交点坐标
    return POINT(x, y);
}

POINT getIntersection(const LINE_SEG& seg1, const LINE_SEG& seg2) {
    LINE l1 = LINE::fromLineSeg(seg1);
    LINE l2 = LINE::fromLineSeg(seg2);

    POINT intersection = getIntersection(l1, l2);

    if (pointIsOnLineSeg(intersection, seg1) &&
        pointIsOnLineSeg(intersection, seg2)) {
        return intersection;
    }
    return POINT(G_INF, G_INF);
}

std::vector<POINT> getIntersection(
    const LINE_SEG& lineSeg,
    const CIRCLE& circle) {
    std::vector<POINT> ret;
    auto lineIntersections =
        getIntersection(LINE::fromLineSeg(lineSeg), circle);
    for (auto p : lineIntersections) {
        if (pointIsOnLineSeg(p, lineSeg)) {
            ret.push_back(p);
        }
    }
    return std::move(ret);
}

std::vector<POINT> getIntersection(
    const LINE_SEG& lineSeg,
    const POLYGON& polygon) {
    int sz = polygon.size();
    std::vector<POINT> rv;
    for (int i = 0; i < sz; ++i) {
        LINE_SEG seg = {polygon[i], polygon[(i + 1) % sz]};
        POINT intersection = getIntersection(seg, lineSeg);
        if (intersection != POINT(G_INF, G_INF)) {
            rv.push_back(intersection);
        }
    }
    return rv;
}

std::vector<POINT> getIntersection(const LINE_SEG& lineSeg, const LINE& line) {
    std::vector<POINT> ret;
    POINT np;
    bool hasIntersection = intersect(LINE::fromLineSeg(lineSeg), line, np);
    if (!hasIntersection) {
        return {};
    }

    if (pointIsOnLineSeg(np, lineSeg))
        return {np};
    else
        return {};
}

std::vector<POINT> getIntersection(const LINE& line, const RECTANGLE& rect) {
    std::vector<POINT> ret;
    auto rectEdges = rect.getFourEdge();
    for (auto edge : rectEdges) {
        for (auto crossPoint : getIntersection(edge, line)) {
            ret.push_back(crossPoint);
        }
    }
    return ret;
}

POINT getPointOutsideCircleWithDesiredAngle(
    const CIRCLE& circle,
    VECTOR2 dir,
    double angle) {
    double alpha = angle / 2;
    double len = circle.r / sin(alpha / 180.0 * M_PI);
    return circle.c + normalize(dir) * len;
}

POINT
getPointMoveFromEdge(const CIRCLE& circle, VECTOR2 dir, double dist) {
    double moveDist = circle.r + dist;
    return circle.c + normalize(dir) * moveDist;
}

POINT getPointMoveFromPoint(const POINT& p, VECTOR2 dir, double dist) {
    dir = normalize(dir);
    return p + dir * dist;
}

bool pointIsInCircle(const CIRCLE& circle, POINT p) {
    return dist(circle.c, p) <= circle.r + G_EP;
}

} // namespace geometry

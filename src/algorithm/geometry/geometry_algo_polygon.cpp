#include "geometry_algo_polygon.h"
#include "geometry_algo_point.h"
#include "geometry_algo_line.h"
#include "geometry_algo_arc.h"
#include "log/log.h"

#include <clipper2/clipper.h>
namespace geometry {

const double STRICT_SEG_INCLUDED_ANGLE = 178.0; // 严格线段夹角
const double LOOSE_SEG_INCLUDED_ANGLE = 175.0;  // 宽松线段夹角
const double BOUNDING_BOX_TOLERANCE = 1E-5;     // bounding box 容差
const double CAPSULE_TO_DOTS_SEG_NUMS = 180; // 操场形一端转成点数量

bool polygonIntersect(const geometry::POLYGON& A, const geometry::POLYGON& B) {
    for (int i = 0; i < A.size(); ++i) {
        int endA = i == A.size() - 1 ? 0 : i + 1;
        geometry::LINE_CHAIN seg1 = {
            geometry::POINT(A[i].x, A[i].y),
            geometry::POINT(A[endA].x, A[endA].y)};
        for (int j = 0; j < B.size(); ++j) {
            int endB = j == B.size() - 1 ? 0 : j + 1;
            geometry::LINE_CHAIN seg2 = {
                geometry::POINT(B[j].x, B[j].y),
                geometry::POINT(B[endB].x, B[endB].y)};
            if (geometry::intersect(seg1, seg2)) {
                return true;
            }
        }
    }
    return false;
}

double polygonArea(const std::vector<geometry::POINT>& polygon) {
    double area = 0.0;
    int n = polygon.size();
    for (int i = 0; i < n; ++i) {
        const geometry::POINT& p1 = polygon[i];
        const geometry::POINT& p2 = polygon[(i + 1) % n];
        area += (p2.x - p1.x) * (p2.y + p1.y);
    }
    return area / 2.0;
}

bool polygonOffset(geometry::POLYGON& polygon, double gap, bool expand) {
    // 要记得去重，不然会外扩出异常值。
    deduplication(polygon);
    if (geometry::polygonArea(polygon) > 0.0f) {
        std::reverse(polygon.begin(), polygon.end());
    }
    size_t vertexCnt = polygon.size();
    auto oldPolygon = polygon;
    polygon.clear();
    std::vector<geometry::ADJACENT_VECTOR2> adjVectors {};
    int vertexSize = vertexCnt - 1;
    // 根据顶点生成向量数据
    for (size_t i = 0; i < vertexCnt; ++i) {
        geometry::VECTOR2 vp1 =
            oldPolygon[i] - oldPolygon[i == vertexSize ? 0 : i + 1];
        geometry::VECTOR2 vp2 =
            oldPolygon[i] - oldPolygon[i == 0 ? vertexSize : i - 1];
        /*
        由于传入的多边形是逆时针序，相邻向量的叉积：
        >0 : 凸点
        <0 : 凹点
        =0 : 都行
        */
        bool convex = multiply(vp1, vp2) > 0;
        adjVectors.emplace_back(vp1, vp2, convex);
    }
    // 产生新顶点
    for (int i = 0; i < vertexCnt; ++i) {
        auto v1 = adjVectors.at(i).v1;
        auto v2 = adjVectors.at(i).v2;
        auto sin = std::abs(
            multiply(geometry::normalize(v1), geometry::normalize(v2)));
        // sin 等于 0 时，向量共线
        if (sin > 0) {
            // 凹点取反
            if (!adjVectors.at(i).convex) {
                sin = -sin;
            }
            geometry::VECTOR2 offset =
                gap / sin * (normalize(v1) + normalize(v2));
            auto vec = geometry::POINT(oldPolygon[i].x, oldPolygon[i].y);
            // 内缩取反
            if (!expand) {
                offset = -offset;
            }
            polygon.emplace_back(vec + offset);
        }
    }
    return true;
}

bool polygonOffsetByClipper2(
    geometry::POLYGON& polygon,
    double gap,
    bool expand) {
    // 检查输入是否为空
    if (polygon.empty()) {
        RLOG_WARNING("polygon is empty");
        return false;
    }

    // 检查间距是否小于0
    if (gap < 0) {
        RLOG_WARNING("gap can not be less than zero");
        return false;
    }

    // 判断外扩还是内缩
    gap = expand ? gap : -gap;

    // 多边形转成clipper2多边形
    Clipper2Lib::PathsD subject;
    Clipper2Lib::PathD outer;
    for (auto& point : polygon) {
        // 检查点是否合法
        if (&point == nullptr) {
            return false;
        }
        outer.emplace_back(point.x, point.y);
    }
    subject.push_back(outer);

    Clipper2Lib::PathsD solution;
    // 多边形外扩/内缩，采用Miter规则
    try {
        solution = InflatePaths(
            subject,
            gap,
            Clipper2Lib::JoinType::Miter,
            Clipper2Lib::EndType::Polygon);
    } catch (const std::exception& e) {
        RLOG_ERROR(e.what());
        return false;
    }

    // 检查外扩后的结果是否为空
    if (solution.empty()) {
        return false;
    }

    // 检查第1个多边形是否为空
    if (solution[0].empty()) {
        return false;
    }

    geometry::POLYGON result;
    // 外扩/内缩后的点加到结果里
    // NOTE: 外扩/内缩后不应该产生多个多边形
    for (auto& point : solution[0]) {
        // 判断点是否合法
        if (&point == nullptr) {
            return false;
        }
        if (std::isnan(point.x) || std::isnan(point.y)) {
            continue;
        }
        result.emplace_back(point.x, point.y);
    }

    polygon = result;
    return true;
}

void deduplication(std::vector<geometry::POINT>& polygon) {
    // 这里用的极小值
    double localEP = 1E-4;
    std::vector<geometry::POINT> res;
    int size = (int)polygon.size();
    if (size < 3) {
        return;
    }
    for (int i = 0; i < size; ++i) {
        auto& prePt = polygon[(i - 1 + size) % size];
        auto& pt = polygon[i];
        if (geometry::twoPointsIsMuchClose(pt, prePt, localEP)) {
            continue;
        }
        res.emplace_back(polygon[i]);
    }
    if (size < 3) {
        return;
    }
    // 首尾相连，去除最后一个
    if (res.size() &&
        geometry::twoPointsIsMuchClose(res.front(), res.back(), localEP)) {
        res.pop_back();
    }
    polygon = res;
}

std::vector<geometry::POINT> deduplication2(
    const std::vector<geometry::POINT>& polygon) {
    std::vector<geometry::POINT> rv;
    for (int j = 0; j < polygon.size(); ++j) {
        if (rv.size() && rv.back() == polygon[j]) {
            continue;
        }
        rv.push_back(polygon[j]);
    }
    if (rv.size() > 1 && rv.front() == rv.back())
        rv.pop_back();
    return rv;
}

void polygonSmooth(std::vector<geometry::POINT>& polygon, double radius) {
    // 去除首尾重复点
    geometry::deduplication2(polygon);
    // 用于存储平滑后的多边形
    std::vector<geometry::POINT> newPolygon;
    int polySize = (int)polygon.size();
    // 用户输入的值
    double diameter = 2.0 * radius;
    // 备份，后面会改变radius值
    double radiusBackUp = radius;
    // 开始处理
    for (int i = 0; i < polySize; ++i) {
        radius = radiusBackUp;
        int prevInd = (i - 1 + polySize) % polySize;
        int nextInd = (i + 1) % polySize;
        geometry::POINT curr = polygon[i];
        geometry::POINT prev = polygon[prevInd];
        geometry::POINT next = polygon[nextInd];
        // 倒角半径处理，基于肉哥给出的规则
        double minL = std::min(dist(curr, next), dist(curr, prev));
        double angle = sweepAngle(prev, next, curr);
        if (minL >= diameter) { // 最小长度 大于等于 用户输入的值，正常倒角
            // useless，正常倒角
        } else if (
            minL > radius) { // 最小长度 大于 用户输入的值的一半，倒角减一半
            radius /= 2.0;
        } else { // 最小长度 小于 用户输入的值的一半，不倒角
            newPolygon.push_back(curr);
            continue;
        }

        // 处理所有角
        if (angle <= LOOSE_SEG_INCLUDED_ANGLE) {
            int segNum = sweepAngle(prev, next, curr) / 2;
            std::vector<geometry::POINT> dots;
            auto currToPrev = prev - curr;
            auto currToNext = next - curr;
            if (geometry::multiply<geometry::POINT>(currToPrev, currToNext) <
                0) {
                dots = geometry::smoothAngle<geometry::POINT>(
                    curr,
                    prev,
                    next,
                    radius,
                    segNum);
                newPolygon.insert(newPolygon.end(), dots.begin(), dots.end());
            } else {
                dots = geometry::smoothAngle<geometry::POINT>(
                    curr,
                    next,
                    prev,
                    radius,
                    segNum);
                std::reverse(dots.begin(), dots.end());
                newPolygon.insert(newPolygon.end(), dots.begin(), dots.end());
            }
        } else {
            newPolygon.push_back(curr);
        }
    }

    polygon = newPolygon; // 更新原多边形为平滑后的多边形
}

std::vector<geometry::POINT> capsule2outline(
    geometry::LINE_CHAIN seg,
    double width,
    int partition,
    double gap,
    bool expand) {
    auto length = dist(seg._st, seg._et);
    gap = expand ? gap : -gap;
    RECTANGLE rect(
        length,
        width,
        (seg._et + seg._st) / 2,
        getAngleWithXAxis(seg._st, seg._et));
    std::vector<geometry::POINT> ret(2 * (partition - 1) + 4);
    auto points = rect.getFourVertex();
    ret[0] = points[0];
    ret[1] = points[1];
    ret[partition + 1] = points[2];
    ret[partition + 2] = points[3];
    // 同一个圆心同样的半径生成出来圆上的点合并后有凹点，所以用1，在同一个点上生成来避免这个问题
    double oneRotate = 180.0 / partition;
    int partitionBak = partition, originPartition = partition;
    partitionBak--; // 180份 BC之间有179个点
    geometry::POINT midBC = (points[1] + points[2]) / 2;
    geometry::POINT rotP = points[1];
    int ind = 2;
    while (partitionBak--) {
        rotP = rotatePoint(rotP, midBC, oneRotate);
        ret[ind++] = rotP;
    }

    geometry::POINT midAD = (points[0] + points[3]) / 2;
    rotP = points[3];
    ind = partition + 3; // 找到D点下一个点下标
    partition--;         // 重新设点数
    while (partition--) {
        rotP = rotatePoint(rotP, midAD, oneRotate);
        ret[ind++] = rotP;
    }
    polygonOffset(ret, gap, expand);
    return ret;
}

bool ptInPolygon(
    const geometry::POINT& point,
    const std::vector<geometry::POINT>& polygon) {
    int intersectCnt = 0;
    int numVertices = polygon.size();
    for (int i = 0; i < numVertices; i++) {
        const geometry::POINT& vertex1 = polygon[i];
        const geometry::POINT& vertex2 = polygon[(i + 1) % numVertices];
        // 检查点是否在顶点之间的水平射线路径上
        if ((vertex1.y > point.y) != (vertex2.y > point.y)) {
            // 计算水平射线与边的交点的 x 坐标
            double intersectX = (vertex2.x - vertex1.x) *
                                    (point.y - vertex1.y) /
                                    (vertex2.y - vertex1.y) +
                                vertex1.x;
            // 检查交点是否在射线的右侧
            if (point.x < intersectX) {
                intersectCnt++;
            }
        }
    }
    // 如果交点数是奇数，则点在多边形内部
    return (intersectCnt % 2) == 1;
}

bool polygonInsidePolygon(
    const std::vector<geometry::POINT>& innerPolygon,
    const std::vector<geometry::POINT>& outerPolygon) {
    for (const geometry::POINT& point : innerPolygon) {
        if (!geometry::ptInPolygon(point, outerPolygon)) {
            return false;
        }
    }
    return true;
}

bool ptOnPolygon(
    const geometry::POINT& point,
    const std::vector<geometry::POINT>& polygon) {
    size_t numVertices = polygon.size();
    for (size_t i = 0; i < numVertices; ++i) {
        const geometry::POINT& vertex1 = polygon[i];
        const geometry::POINT& vertex2 = polygon[(i + 1) % numVertices];
        LINE_SEG seg(vertex1, vertex2);
        // 检查点是否在当前边的水平范围内 抓linechain是0.1nm，对应1e-6，这里试到1e-8 OK
        const double toleranceAccordingToLineChainWidth = 1e-8;
        if (seg.calculatePoint2LineShortestDistance(point) <
            toleranceAccordingToLineChainWidth) {
            return true;
        }
    }
    return false; // 点不在边界上，返回 false
}

bool ptOnSameSide(
    const geometry::POINT& p1,
    const geometry::POINT& p2,
    const std::vector<geometry::POINT>& polygon) {
    int sz = polygon.size();
    for (int i = 0; i < sz; ++i) {
        geometry::LINE_CHAIN seg {polygon[i], polygon[(i + 1) % sz]};
        if (geometry::pointIsOnLineSeg(p1, seg) &&
            geometry::pointIsOnLineSeg(p2, seg)) {
            return true;
        }
    }
    return false;
}

std::vector<geometry::POINT> getPolygonSegmentIntersections(
    const std::vector<geometry::POINT>& polygon,
    const geometry::POINT& start,
    const geometry::POINT& end) {
    std::vector<geometry::POINT> res;
    geometry::POINT ipt;
    int sz = polygon.size();
    for (int i = 0; i < sz; ++i) {
        geometry::LINE_CHAIN seg(polygon.at(i), polygon.at((i + 1) % sz));
        if (intersect(seg, {start, end}, ipt)) {
            res.emplace_back(ipt);
        }
    }
    return res;
}

double nPolygonInscribedDistance(int n, double radius) {
    return radius * std::cos(M_PI / n);
}

geometry::POINT computeCentroid(const std::vector<geometry::POINT>& points) {
    if (points.empty()) {
        return {0, 0};
    }
    double sumX = 0.0;
    double sumY = 0.0;
    int count = 0;

    for (const auto& point : points) {
        sumX += point.x;
        sumY += point.y;
        count++;
    }

    return {sumX / count, sumY / count};
}

std::vector<geometry::POINT> sortPointsInCounterClockwise(
    std::vector<geometry::POINT>& points) {
    // Not enough points to form a polygon
    if (points.size() < 3) {
        return points;
    }
    // Find the leftmost and bottommost point as the starting point
    geometry::POINT startingPoint = points[0];
    for (const geometry::POINT& point : points) {
        if (point.y < startingPoint.y ||
            (point.y == startingPoint.y && point.x < startingPoint.x)) {
            startingPoint = point;
        }
    }

    // Sort the points based on the angle with the starting point
    std::sort(
        points.begin(),
        points.end(),
        [&](const geometry::POINT& p1, const geometry::POINT& p2) {
            double crossProduct = geometry::multiply(p1, p2, startingPoint);
            if (std::abs(crossProduct) < geometry::G_EP) {
                // Points are collinear, compare distance from the starting point
                double dist1 =
                    std::hypot(p1.x - startingPoint.x, p1.y - startingPoint.y);
                double dist2 =
                    std::hypot(p2.x - startingPoint.x, p2.y - startingPoint.y);
                return dist1 < dist2;
            }
            return crossProduct > 0;
        });

    return points;
}

bool isPolyPolyIntersect(POLYGON p1, POLYGON p2) {
    for (auto p : p1) {
        if (geometry::ptInPolygon(p, p2)) {
            return true;
        }
    }

    for (auto p : p2) {
        if (geometry::ptInPolygon(p, p1)) {
            return true;
        }
    }

    for (int i = 0; i < p1.size(); ++i) {
        for (int j = 0; j < p2.size(); ++j) {
            if (intersect(
                    {p1[i], p1[(i + 1) % p1.size()]},
                    {p2[j], p2[(j + 1) % p2.size()]})) {
                return true;
            }
        }
    }
    return false;
}

POLYGON movePolygonBy(const POLYGON& plg, const POINT& dir) {
    POLYGON ret = plg;
    for (auto& p : ret) {
        p.x += dir.x;
        p.y += dir.y;
    }
    return ret;
}

bool isConcavePolygon(const POLYGON& polygon) {
    int vertexCount = polygon.size();

    for (int i = 0; i < vertexCount; ++i) {
        const geometry::POINT& current = polygon[i];
        const geometry::POINT& previous =
            polygon[(i - 1 + vertexCount) % vertexCount];
        const geometry::POINT& next = polygon[(i + 1) % vertexCount];

        geometry::POINT v1 = current - previous;
        geometry::POINT v2 = next - current;

        // 计算叉积
        double crossProduct = v1.x * v2.y - v1.y * v2.x;

        // 如果叉积小于0，表示存在内凹
        if (crossProduct < 0) {
            return true;
        }
    }

    // 所有顶点的叉积都大于等于0，表示不是内凹图形
    return false;
}

/**
* @brief 获取以 B 为顶点的角度
*        c² = a² + b² - 2ab * cos(C)
*               B
*              /\
*          A /  \ C
* @param A
* @param B
* @param C
* @return double
* @note 确保输入的三个点不会共线，否则夹角无法正确计算
*/
double geoAngleBetweenPoints(
    const geometry::POINT& A,
    const geometry::POINT& B,
    const geometry::POINT& C) {
    /* 向量 BA 和向量 BC */
    double ba_x = A.x - B.x;
    double ba_y = A.y - B.y;
    double bc_x = C.x - B.x;
    double bc_y = C.y - B.y;

    /* BA 和 BC 的点积 */
    double dot_product = ba_x * bc_x + ba_y * bc_y;

    /* 计算 BA 和 BC 的长度 */
    double ba_length = std::hypot(ba_x, ba_y);
    double bc_length = std::hypot(bc_x, bc_y);

    /* BA 和 BC 之间的夹角的余弦值 */
    double cos_theta = dot_product / (ba_length * bc_length);
    double theta = std::acos(cos_theta);

    /* 弧度转角度 */
    return theta * 180.0 / M_PI;
}

/**
* @brief 返回两点的中点
*
* @tparam T 点
* @param A 点A
* @param B 点B
* @return T 中心点
*/
geometry::POINT mid(const geometry::POINT& A, const geometry::POINT& B) {
    return {(A.x + B.x) / 2, (A.y + B.y) / 2};
}

/**
* @brief 三点共线判断
*        三点共线时，两条直线的斜率相等：
*        (y2 - y1) / (x2 - x1) == (y3 - y1) / (x3 -x1)
*        防止除0问题转化成乘法：
*        (y3 - y1) * (x2 - x1) - (y2 - y1) * (x3 - x1) == 0
* @param p1 点1
* @param p2 点2
* @param p3 点3
* @return true 三点共线
* @return false 三点不共线
*/
bool collinear(
    const geometry::POINT& p1,
    const geometry::POINT& p2,
    const geometry::POINT& p3) {
    qreal p1X = p1.x;
    qreal p1Y = p1.y;
    return !((p3.y - p1Y) * (p2.x - p1X) - (p2.y - p1Y) * (p3.x - p1X));
}

/**
 * @brief 获取一组点中距离目标点最近的点(暴力法)
 *
 * @tparam T 点类型，需要支持 x 和 y 坐标访问
 * @param points 一组点的向量
 * @param targetPoint 目标点
 * @return T 距离目标点最近的点
 */
geometry::POINT getNearestPoint(
    const std::vector<geometry::POINT>& points,
    const geometry::POINT& targetPoint) {
    double minDistance = std::numeric_limits<double>::max();
    geometry::POINT nearestPoint;
    for (const auto& point : points) {
        double distance = std::sqrt(
            std::pow(point.x - targetPoint.x, 2) +
            std::pow(point.y - targetPoint.y, 2));
        if (distance < minDistance) {
            minDistance = distance;
            nearestPoint = point;
        }
    }
    return nearestPoint;
}

/**
 * @brief 计算极角
 * 
 * @tparam V 
 * @tparam T 
 * @param p 
 * @param centroid 
 * @return V 
 */
double computePolarAngle(const geometry::POINT& p1, const geometry::POINT& p2) {
    if (p1 == p2) {
        return 0;
    }
    double dx = p1.x - p2.x;
    double dy = p1.y - p2.y;
    double angle = std::atan2(dy, dx);
    if (angle < 0) {
        angle += 2 * M_PI;
    }
    return angle;
}
/******************************************************************************/

/**********************************向量基本运算*********************************/

/******************************************************************************/

/********************************线段、直线基本运算*******************************/

/**
* @brief 求点p到线段l的最短距离,并返回线段上距该点最近的点np
* 注意：np是线段l上到点p最近的点，不一定是垂足
* @param p 点的坐标
* @param l 线段的起点和终点
* @param np 最近点的坐标
* @return double 最短距离
*/
double ptolinesegdist(const POINT& p, const LINE_SEG& l, POINT& np) {
    double r = geometry::relation(p, l);
    if (r < 0) {
        np = l._st;
        return geometry::dist(p, l._st);
    }
    if (r > 1) {
        np = l._et;
        return geometry::dist(p, l._et);
    }
    np = perpendicular(p, l);
    return geometry::dist(p, np);
}

/**
* @brief 判断两点是否在一条直线ab的同一侧
* @param p1 第一个点
* @param p2 第二个点
* @param a 直线上的点A
* @param b 直线上的点B
* @return true 在同一侧
* @return false 不在同一侧
*/
bool pointsOnSameSide(
    const POINT& p1,
    const POINT& p2,
    const POINT& a,
    const POINT& b) {
    auto dir1 = geometry::normalize(geometry::perpendicular(p1, {a, b}) - p1);
    auto dir2 = geometry::normalize(geometry::perpendicular(p2, {a, b}) - p2);
    return dir1.x * dir2.x >= 0 && dir1.y * dir2.y >= 0;
}

/**
* @brief 判断点p是否在矩形r内
* @param p 点p
* @param r 矩形r
* @return true 在矩形内
* @return false 不在矩形内
*/
bool pointIsInRectangle(const POINT& p, const RECTANGLE& r) {
    auto segs = r.getFourEdge();
    return geometry::pointsOnSameSide(r.center, p, segs[0]._st, segs[0]._et) &&
           geometry::pointsOnSameSide(r.center, p, segs[1]._st, segs[1]._et) &&
           geometry::pointsOnSameSide(r.center, p, segs[2]._st, segs[2]._et) &&
           geometry::pointsOnSameSide(r.center, p, segs[3]._st, segs[3]._et);
}

/**
* @brief 判断点p是否在椭圆oct内
*/
bool pointIsInOctagon(POINT p, OCTAGON oct) {
    auto segs = oct.getEightEdge();
    for (auto seg : segs) {
        if (!geometry::pointsOnSameSide(p, oct.center, seg._st, seg._et)) {
            return false;
        }
    }
    return true;
}

/**
* @brief 判断点p是否在椭圆oct内
*/
bool pointIsInPolygon(POINT p, POLYGON plg) {
    std::vector<LINE_SEG> segs(plg.size());
    for (int i = 1; i < plg.size(); ++i) {
        segs[i] = {plg[i - 1], plg[i]};
    }
    segs[0] = {plg[plg.size() - 1], plg[0]};
    auto innerPoint = (plg[0] + plg[2]) / 2;
    for (auto seg : segs) {
        if (!geometry::pointsOnSameSide(p, innerPoint, seg._st, seg._et)) {
            return false;
        }
    }
    return true;
}

/**
* @brief 给定一个点和一组线段，求离这个点最近的线段
* @param p 给定的点
* @param segs 线段集合
* @return LINE_SEG 最近的线段
*/
LINE_SEG nearestLineSeg(const POINT& p, const std::vector<LINE_SEG>& segs) {
    if (segs.empty()) {
        throw "empty set";
    }
    POINT np;
    double minDist = geometry::ptolinesegdist(p, segs[0], np);
    LINE_SEG minDistSeg = segs[0];
    for (const LINE_SEG& seg : segs) {
        double dist = geometry::ptolinesegdist(p, seg, np);
        if (dist < minDist) {
            minDist = dist;
            minDistSeg = seg;
        }
    }
    return minDistSeg;
}

/**
* @brief 通过两点创建直线
*        已知两个点A(x1, y1)和B(x2, y2)时，
*        直线的一般方程为：a * x + b * y + c = 0
*        其中，a = y2 - y1，b = x1 - x2，c = x2 * y1 - x1 * y2
* @param p1 第一个点
* @param p2 第二个点
* @return LINE 创建的直线
*/
LINE pt2line(const geometry::POINT& p1, const geometry::POINT& p2) {
    return {p2.y - p1.y, p1.x - p2.x, p2.x * p1.y - p1.x * p2.y};
}

/**
* @brief 计算直线与x轴的夹角（绝对值）
*
* @param p1 直线上的点1
* @param p2 直线上的点2
* @param angle 输出的角度值
* @return bool 如果直线与x轴平行，返回false；否则返回true
*/
bool includedAngle(
    const geometry::POINT& p1,
    const geometry::POINT& p2,
    double& angle) {
    if (p2.x == p1.x)
        return false;
    angle = std::abs(std::atan((p2.y - p1.y) / (p2.x - p1.x)) * 180.0 / M_PI);
    return true;
}

/**
* @brief 给定线段AB，返回AB延长dy距离后，垂直于AB，长度为2dx的CD线段
*         C______D
*             B                | dy
*            |            dx___|
*            |
*             A
*
* @tparam T 点的类型
* @param A 线段AB的起点
* @param B 线段AB的终点
* @param dx CD线段长度的一半（即CD线段中点到C或D的距离）
* @param dy AB线段延长的距离
* @return LINE_SEG CD线段
*/
LINE_SEG verticalLineByDist(
    const geometry::POINT& A,
    const geometry::POINT& B,
    double dx,
    double dy) {
    geometry::POINT uab = geometry::normalize(B - A); // AB的单位向量
    geometry::POINT M = mid(A, B);                    // AB的中点
    geometry::POINT midCD = M + uab * (dist(A, B) / 2 + dy); // CD线段的中点
    geometry::POINT u1 =
        geometry::verticalCCWNormalized(A, B); // AB的逆时针垂直单位向量
    geometry::POINT u2 =
        geometry::verticalCWNormalized(A, B); // AB的顺时针垂直单位向量
    return {midCD + u1 * dx, midCD + u2 * dx};
}

/**
* @brief 将N边形转换为轮廓线的点集合
* @param nPlg N边形对象
* @param gap 轮廓线的偏移距离
* @param expand 是否向外扩展（true表示向外扩展，false表示向内收缩）
* @return 轮廓线的点集合
*/
std::vector<geometry::POINT>
    nPolygon2outline(N_POLYGON nPlg, double gap, bool expand) {
    gap = expand ? gap : -gap;
    auto points = nPlg.getNVertex();
    for (auto& point : points) {
        geometry::POINT movVec = geometry::normalize(point - nPlg.center) * gap;
        point = point + movVec;
    }
    return points;
}

/**
 * @brief 获取给定边集中离给定点最近的边
 * @param p 给定点
 * @param segs 给定边集
 * @return 给定边集中离给定点最近的边
*/
geometry::POINT getClosestPoint(geometry::POINT p, std::vector<LINE_SEG> segs) {
    double minDistance = 0;
    geometry::POINT nearestPoint;

    if (!segs.empty()) {
        auto seg = segs.front();
        minDistance = geometry::ptolinesegdist(p, seg, nearestPoint);
        // 找出里p最近的seg
        for (int i = 1; i < segs.size(); ++i) {
            seg = segs[i];
            geometry::POINT pos;
            auto dist = geometry::ptolinesegdist(p, seg, pos);
            if (dist < minDistance) {
                minDistance = dist;
                nearestPoint = pos;
            }
        }
    }
    return nearestPoint;
}

/**
 * @brief 计算点p和多边形poly的距离，并返回最近的点
 * @param poly 多边形
 * @param p 给定点
 * @param retP 传出参数，多边形上里p最近的点
 * @return p和poly的距离
*/
double pointToPolygonDist(
    geometry::POLYGON poly,
    geometry::POINT p,
    geometry::POINT& retP) {
    double minDist = G_INF;
    retP = {G_INF, G_INF};
    for (int i = 0; i < poly.size(); ++i) {
        geometry::POINT nearP;
        double d = ptolinesegdist(
            p,
            LINE_SEG {poly[i], poly[(i + 1) % poly.size()]},
            nearP);
        if (d < minDist) {
            minDist = d;
            retP = nearP;
            // qDebug() << "nearestP on line seg on (" << poly[i].x << ", " << poly[i].y << ")-->(" << poly[(i + 1) % poly.size()].x << ", " << poly[(i + 1) % poly.size()].y << ") is ("
            //     << nearP.x << ", " << nearP.y << ")";
        }
    }
    // qDebug() << "nearestP  is (" << retP.x << ", " << retP.y << ")";
    return minDist;
}

// 大于0，b在a的逆时针方向
double crossProduct2D(const POINT& a, const POINT& b) {
    return a.x * b.y - a.y * b.x;
}

double dotProduct(const POINT& a, const POINT& b) {
    return a.x * b.x + a.y * b.y;
}

double length(const POINT& a) {
    return std::sqrt(a.x * a.x + a.y * a.y);
}

// a在b上的投影
double Proj(const POINT& a, const POINT& b) {
    return dotProduct(a, b) / length(b);
}

/**
 * @brief 在逆时针存储的点集中，给出连续的三个点，判断是否是凹点
 * @details
 *      
 * @param prevP
 * @param currP
 * @param nextP
 * @return
 */
bool isConcace(POINT prevP, POINT currP, POINT nextP) {
    POINT currToPrev = prevP - currP;
    POINT currToNext = nextP - currP;
    return crossProduct2D(currToPrev, currToNext) < 0;
}

bool isConvex(POINT prevP, POINT currP, POINT nextP) {
    POINT currToPrev = prevP - currP;
    POINT currToNext = nextP - currP;
    return crossProduct2D(currToPrev, currToNext) > 0;
}

bool isConcave(const POLYGON poly, int ind) {
    int sz = poly.size();
    int prevInd = (ind - 1 + sz) % sz;
    int nextInd = (ind + 1) % sz;
    return isConcace(poly[prevInd], poly[ind], poly[nextInd]);
}

bool isConvex(const POLYGON poly, int ind) {
    int sz = poly.size();
    int prevInd = (ind - 1 + sz) % sz;
    int nextInd = (ind + 1) % sz;
    return isConvex(poly[prevInd], poly[ind], poly[nextInd]);
}

void printPoints(std::vector<POINT>& polygon) {
    // qDebug() << "=======================print points=====================";
    // for (auto p : polygon) {
    //     qDebug() << "(" << p.x << ", " << p.y << ")->";
    // }
    // qDebug()
    //     << "========================end print points======================";
}

/**
 * @brief p1 p2 p3 是否是圆弧上连续三点
 * @param p1 第一个点
 * @param p2 中间的点
 * @param p3 最后一个点
 * @return p1p2p3 是否满足形成圆弧的条件
*/
bool isConsecutiveChords2(const POINT& p1, const POINT& p2, const POINT& p3) {
    // 计算两条线段夹角
    double angle = sweepAngle(p1, p3, p2);
    // 容差范围内等于178，认为是在圆弧上
    // bool res = std::abs(angle - 178.0) < geometry::G_EP_REVERT_DOTS;
    double r;
    auto center = trip2circle(p1, p2, p3, r);
    auto twoDegArcLengthSquare = std::pow(2.0 / 180.0 * M_PI * r, 2);
    if (std::isnan(twoDegArcLengthSquare)) {
        return false;
    }
    return angle > LOOSE_SEG_INCLUDED_ANGLE &&
           squaredDistance(p1, p2) < twoDegArcLengthSquare &&
           squaredDistance(p2, p3) < twoDegArcLengthSquare;
}

/**
 * @brief ptInd 在 poly 上 是否是圆弧中【非起点非终点】 一点，定义圆弧至少要5个点
 * @param ptInd 要判断是否在圆弧中的点在poly中的下标
 * @param poly  传入的多边形
 * @return 在poly中下标为ptInd的点是否是圆弧中的一点
*/
bool isConsecutiveChords(int ptInd, const POLYGON& poly) {
    int sz = poly.size();
    if (sz < 5) {
        return false;
    }

    int prevInd = (ptInd - 1 + sz) % sz;
    int nextInd = (ptInd + 1) % sz;
    auto baseCpd = geometry::crossProduct2D(
        poly[ptInd] - poly[prevInd],
        poly[nextInd] - poly[ptInd]);
    for (int ind = prevInd; ind != (nextInd + 1) % sz; ind = (ind + 1) % sz) {
        auto cpd = geometry::crossProduct2D(
            poly[ind] - poly[(ind - 1 + sz) % sz],
            poly[(ind + 1) % sz] - poly[ind]);
        auto angle = sweepAngle(
            poly[(ind - 1 + sz) % sz],
            poly[(ind + 1) % sz],
            poly[ind]);
        const double angleFloor = 177.0 - 1e-5;
        const double angleCeil = 180.0 - 1e-2;
        if (cpd * baseCpd <= 0 || angle < angleFloor || angle > angleCeil) {
            return false;
        }
    }
    // 已知 如果是圆上的点，每两个相邻点之间的夹角不会小于2度
    double r1, r2;
    geometry::POINT center1 = trip2circle(
        poly[(prevInd - 1 + sz) % sz],
        poly[prevInd],
        poly[ptInd],
        r1);
    geometry::POINT center2 =
        trip2circle(poly[ptInd], poly[nextInd], poly[(nextInd + 1) % sz], r2);
    auto r = std::min(r1, r2);
    auto arcMax = M_PI / 90 * r;
    auto arcMaxSquare = arcMax * arcMax;
    for (int ind = prevInd; ind != (nextInd + 2) % sz; ind = (ind + 1) % sz) {
        if (squaredDistance(poly[ind], poly[(ind - 1 + sz) % sz]) >=
            arcMaxSquare) {
            return false;
        }
    }
    return true;
}

/**
 * @brief 判断poly某个下标的点和圆弧的关系
*/
POINT_POLY_RELATION ptPolyRelation(int ptInd, const POLYGON& poly) {
    int sz = poly.size();
    int prevInd = (ptInd - 1 + sz) % sz;
    int nextInd = (ptInd + 1) % sz;

    if (isConsecutiveChords(ptInd, poly)) {
        return POINT_POLY_RELATION::ON_ARC_MID;
        // 前两个点和后两个点在圆上，但是不满足这里，在else里处理
    } else {
        bool isPtAfterStart = isConsecutiveChords((ptInd + 1) % sz, poly);
        if (isPtAfterStart) {
            return POINT_POLY_RELATION::ON_ARC_MID;
        }
        bool isPtBeforeEnd = isConsecutiveChords((ptInd - 1 + sz) % sz, poly);
        if (isPtBeforeEnd) {
            return POINT_POLY_RELATION::ON_ARC_MID;
        }
        bool canBeStart = isConsecutiveChords((ptInd + 2) % sz, poly);
        bool canBeEnd = isConsecutiveChords((ptInd - 2 + sz) % sz, poly);
        if (canBeStart && canBeEnd) {
            return POINT_POLY_RELATION::BETWEEN_TWO_ARC;
        } else if (canBeStart) {
            return POINT_POLY_RELATION::ON_ARC_START;
        } else if (canBeEnd) {
            return POINT_POLY_RELATION::ON_ARC_END;
        }
    }
    return POINT_POLY_RELATION::NOT_ON_ARC;
}

//  void removeAbnormalPoints(std::vector<POINT>& polygon) {
//     std::vector<POINT> modifiedPolygon;
//     for (int i = 0; i < 2; i++) {
//         int sz = polygon.size();
//         for (int ind = 0; ind < sz; ++ind) {
//             int prevInd = (ind - 1 + sz) % sz;
//             int nextInd = (ind + 1) % sz;
//             if (sweepAngle(polygon[prevInd], polygon[nextInd], polygon[ind]) < 7) {
//                 continue;
//             } else {
//                 modifiedPolygon.push_back(polygon[ind]);
//             }
//         }
//         polygon = modifiedPolygon;
//         modifiedPolygon.clear();
//     }
// }

void smoothPolygon2(std::vector<POINT>& polygon, double radius) {
    //    // qDebug() << "*********************start smooth "
    //    //             "polygon*********************************";
    //    // 纠正多边形顺序
    //    if (geometry::polygonArea(polygon) > 0.0) {
    //        std::reverse(polygon.begin(), polygon.end());
    //    }
    //
    //    std::vector<POINT> newPolygon; // 用于存储平滑后的多边形
    //    bool half_edge_rounded = false;
    //
    //    int sz = polygon.size();
    //    // 寻找一个合适的起点, 长度合适，且不是弧中的点，【可以是弧起点，也可以是弧终点】
    //    int validStInd = 0;
    //    bool findValidSt = false;
    //    for (; validStInd < sz; ++validStInd) {
    //        int prevInd = (validStInd - 1 + sz) % sz;
    //        int nextInd = (validStInd + 1) % sz;
    //        if (sweepAngle(
    //                polygon[prevInd],
    //                polygon[nextInd],
    //                polygon[validStInd]) <= LOOSE_SEG_INCLUDED_ANGLE &&
    //            dist(polygon[validStInd], polygon[prevInd]) >= radius) {
    //            findValidSt = true;
    //            break;
    //        }
    //    }
    //
    //    if (!findValidSt)
    //        return;
    //
    //    // qDebug() << "find a valid start";
    //    newPolygon.push_back(polygon[(validStInd - 1 + sz) % sz]);
    //    rededa::pcbcommon::shapeutils::addPointPreview(
    //        newPolygon.back(),
    //        Qt::green);
    //    // qDebug() << "(" << newPolygon.back().x << ", " << newPolygon.back().y
    //    //          << ") is pushed back";
    //    newPolygon.push_back(polygon[validStInd]);
    //    rededa::pcbcommon::shapeutils::addPointPreview(
    //        newPolygon.back(),
    //        Qt::green);
    //    // qDebug() << "(" << newPolygon.back().x << ", " << newPolygon.back().y
    //    //          << ") is pushed back";
    //    // printPoints(newPolygon);
    //    bool expectRound = false;
    //    for (int currInd = validStInd + 1; currInd != (validStInd - 1 + sz) % sz;
    //         currInd = (currInd + 1) % sz) {
    //        // qDebug() << "----------------processing ind " << currInd
    //        //          << "-----------------------------";
    //        if (polygon[currInd] == newPolygon.back()) {
    //            // qDebug() << "current pt is same as newPolygon.back(), skip this "
    //            //             "point and continue";
    //            continue;
    //        }
    //        // qDebug() << "current pt = (" << polygon[currInd].x << ", "
    //        //          << polygon[currInd].y << ")";
    //        // qDebug() << "newPolygon.back() = (" << newPolygon.back().x << ", "
    //        //          << newPolygon.back().y << ")";
    //        int prevInd = (currInd - 1 + sz) % sz;
    //        int nextInd = (currInd + 1) % sz;
    //        // qDebug() << "next Point = (" << polygon[nextInd].x << ", "
    //        //          << polygon[nextInd].y << ")";
    //        if (expectRound) {
    //            // qDebug() << "-------------------currInd = " << currInd
    //            //          << " is expected to go round";
    //            if (isConsecutiveChords2(
    //                    newPolygon[newPolygon.size() - 2],
    //                    newPolygon.back(),
    //                    polygon[currInd])) {
    //                // > LOOSE_SEG_INCLUDED_ANGLE 的不用倒
    //                expectRound = false;
    //                newPolygon.push_back(polygon[currInd]);
    //                // qDebug() << "current point (" << polygon[currInd].x << ", "
    //                //          << polygon[currInd].y
    //                //          << ") is a big angle point, directly push back";
    //                rededa::pcbcommon::shapeutils::addPointPreview(
    //                    newPolygon.back(),
    //                    Qt::green);
    //                continue;
    //            }
    //            auto lendist = dist(newPolygon.back(), polygon[currInd]);
    //            if (lendist < radius) {
    //                // 长度不够，继续下一个
    //                // qDebug() << "lendist = " << lendist << " < radius = " << radius;
    //                continue;
    //            }
    //
    //            // 长度够了，可以导了
    //            auto prev = newPolygon[newPolygon.size() - 2];
    //            auto curr = newPolygon.back();
    //            auto next = polygon[currInd];
    //            // qDebug() << "start rounding.............(" << prev.x << ", "
    //            //          << prev.y << ")->(" << curr.x << ", " << curr.y << ")->("
    //            //          << next.x << ", " << next.y << ")";
    //            int segNum = sweepAngle(prev, next, curr) / 2;
    //            std::vector<POINT> dots;
    //            auto currToPrev = prev - curr;
    //            auto currToNext = next - curr;
    //            newPolygon.pop_back();
    //            if (multiply<POINT>(currToPrev, currToNext) < 0) {
    //                dots = smoothAngle<geometry::POINT>(
    //                    curr,
    //                    prev,
    //                    next,
    //                    radius,
    //                    segNum);
    //                for (auto p : dots) {
    //                    rededa::pcbcommon::shapeutils::addPointPreview(
    //                        p,
    //                        Qt::green);
    //                }
    //                newPolygon.insert(newPolygon.end(), dots.begin(), dots.end());
    //
    //            } else {
    //                dots = smoothAngle<geometry::POINT>(
    //                    curr,
    //                    next,
    //                    prev,
    //                    radius,
    //                    segNum);
    //                std::reverse(dots.begin(), dots.end());
    //                for (auto p : dots) {
    //                    rededa::pcbcommon::shapeutils::addPointPreview(
    //                        p,
    //                        Qt::green);
    //                }
    //                newPolygon.insert(newPolygon.end(), dots.begin(), dots.end());
    //            }
    //            expectRound = false;
    //
    //            continue;
    //        }
    //
    //        // qDebug() << "-------------------currInd = " << currInd
    //        //          << " is not expected to go round";
    //        if (isConsecutiveChords2(
    //                newPolygon.back(),
    //                polygon[currInd],
    //                polygon[nextInd]) ||
    //            isConsecutiveChords2(
    //                newPolygon[newPolygon.size() - 2],
    //                newPolygon.back(),
    //                polygon[currInd]) ||
    //            isConsecutiveChords2(
    //                polygon[currInd],
    //                polygon[nextInd],
    //                polygon[(nextInd + 1) % sz])) {
    //            // 认为是圆弧上的点
    //            // qDebug() << "pt on arc, push back, and continue;";
    //            newPolygon.push_back(polygon[currInd]);
    //            rededa::pcbcommon::shapeutils::addPointPreview(
    //                newPolygon.back(),
    //                Qt::green);
    //            // qDebug() << "(" << newPolygon.back().x << ", "
    //            //          << newPolygon.back().y << ") is pushed back";
    //            // currInd = (currInd + 1) % sz;
    //            // newPolygon.push_back(polygon[currInd]);
    //            // rededa::pcbcommon::shapeutils::addPointPreview(newPolygon.back(), Qt::green);
    //            // qDebug() << "(" << newPolygon.back().x << ", " << newPolygon.back().y << ") is pushed back";
    //            continue;
    //        }
    //
    //        // auto dummyIntersection = getIntersection(
    //        //     LINE(newPolygon.back(), newPolygon[newPolygon.size() - 2]),
    //        //     LINE(polygon[nextInd], polygon[currInd]));
    //        // if (dummyIntersection == INVALID_POINT) {
    //        //     qDebug() << "dummyIntersection == INVALID_POINT, push back current point into new polygon";
    //        //     // 说明点重复或平行了，直接push_back，再continue
    //        //     newPolygon.push_back(polygon[currInd]);
    //        //     rededa::pcbcommon::shapeutils::addPointPreview(newPolygon.back(), Qt::green);
    //        //     qDebug() << "(" << newPolygon.back().x << ", " << newPolygon.back().y << ") is pushed back";
    //        //     continue;
    //        // }
    //        // qDebug() << "dummyIntersection = (" << dummyIntersection.x << ", " << dummyIntersection.y << ")";
    //        // rededa::pcbcommon::shapeutils::addPointPreview(dummyIntersection, Qt::yellow);
    //        // auto angle =
    //        //     sweepAngle(newPolygon.back(), polygon[currInd], dummyIntersection);
    //        // qDebug() << "angle(newPolygon.back()<-dummyIntersection->polygon[currInd]) = " << angle;
    //
    //        double nextSegLength = dist(polygon[currInd], polygon[nextInd]);
    //        // qDebug() << "next seg length = " << nextSegLength
    //        //          << ", radius = " << radius;
    //        if (nextSegLength < radius) {
    //            // 长度不够，跳过这个点，找下一个
    //            newPolygon.push_back(polygon[currInd]);
    //            rededa::pcbcommon::shapeutils::addPointPreview(
    //                newPolygon.back(),
    //                Qt::cyan);
    //            expectRound = true;
    //            // qDebug() << "length < radius, (" << newPolygon.back().x << ", "
    //            //          << newPolygon.back().y << ") just push and continue;";
    //            continue;
    //        }
    //
    //        // if (angle <= LOOSE_SEG_INCLUDED_ANGLE) {
    //        // std::vector<POINT> dots;
    //        // int segNum = angle / 2;
    //        // auto currToPrev = newPolygon.back() - dummyIntersection;
    //        // auto currToNext = polygon[currInd] - dummyIntersection;
    //        // qDebug() << "start round angle";
    //        // if (multiply<POINT>(currToPrev, currToNext) < 0) {
    //        //     qDebug() << "multiply<POINT>(currToPrev, currToNext) < 0";
    //        //     dots = smoothAngle<geometry::POINT>(
    //        //         dummyIntersection,
    //        //         newPolygon.back(),
    //        //         polygon[currInd],
    //        //         radius,
    //        //         segNum);
    //        //     if (
    //        //         pointIsOnLineSeg(
    //        //             dots.front(),
    //        //             LINE_SEG(dummyIntersection, newPolygon.back())) ||
    //        //         pointIsOnLineSeg(
    //        //             dots.back(),
    //        //             LINE_SEG(dummyIntersection, polygon[currInd])) ||
    //        //         pointIsOnLineSeg(
    //        //             dots.front(),
    //        //             LINE_SEG(dummyIntersection, polygon[currInd])) ||
    //        //         pointIsOnLineSeg(
    //        //             dots.back(),
    //        //             LINE_SEG(dummyIntersection, newPolygon.back()))
    //        //     ) {
    //        //         qDebug() << "arc is on dummy seg, just push 2 points";
    //        //         newPolygon.push_back(polygon[currInd]);
    //        //         rededa::pcbcommon::shapeutils::addPointPreview(newPolygon.back(), Qt::green);
    //        //         qDebug() << "(" << newPolygon.back().x << ", " << newPolygon.back().y << ") is pushed back";
    //        //         newPolygon.push_back(polygon[nextInd]);
    //        //         rededa::pcbcommon::shapeutils::addPointPreview(newPolygon.back(), Qt::green);
    //        //         qDebug() << "(" << newPolygon.back().x << ", " << newPolygon.back().y << ") is pushed back";
    //        //     } else {
    //        //         rededa::pcbcommon::shapeutils::addPointPreview(newPolygon.back(), Qt::red);
    //        //         qDebug() << "(" << newPolygon.back().x << ", " << newPolygon.back().y << ") is poped";
    //        //         newPolygon.pop_back();
    //        //         newPolygon.insert(
    //        //             newPolygon.end(),
    //        //             dots.begin(),
    //        //             dots.end());
    //        //         for (auto p : dots) {
    //        //             rededa::pcbcommon::shapeutils::addPointPreview(p, Qt::green);
    //        //         }
    //        //     }78
    //        //     // printPoints(newPolygon);
    //        // } else {
    //        //     qDebug() << "multiply<POINT>(currToPrev, currToNext) >= 0";
    //        //     dots = smoothAngle<geometry::POINT>(
    //        //         dummyIntersection,
    //        //         polygon[currInd],
    //        //         newPolygon.back(),
    //        //         radius,
    //        //         segNum);
    //        //     std::reverse(dots.begin(), dots.end());
    //        //     if (
    //        //         pointIsOnLineSeg(
    //        //             dots.front(),
    //        //             LINE_SEG(dummyIntersection, newPolygon.back())) ||
    //        //         pointIsOnLineSeg(
    //        //             dots.back(),
    //        //             LINE_SEG(dummyIntersection, polygon[currInd])) ||
    //        //         pointIsOnLineSeg(
    //        //             dots.front(),
    //        //             LINE_SEG(dummyIntersection, polygon[currInd])) ||
    //        //         pointIsOnLineSeg(
    //        //             dots.back(),
    //        //             LINE_SEG(dummyIntersection, newPolygon.back()))
    //        //         ) {
    //        //         qDebug() << "arc is on dummy seg, just push 2 points";
    //        //         newPolygon.push_back(polygon[currInd]);
    //        //         rededa::pcbcommon::shapeutils::addPointPreview(newPolygon.back(), Qt::green);
    //        //         qDebug() << "(" << newPolygon.back().x << ", " << newPolygon.back().y << ") is pushed back";
    //        //         newPolygon.push_back(polygon[nextInd]);
    //        //         rededa::pcbcommon::shapeutils::addPointPreview(newPolygon.back(), Qt::green);
    //        //         qDebug() << "(" << newPolygon.back().x << ", " << newPolygon.back().y << ") is pushed back";
    //        //     } else {
    //        //         rededa::pcbcommon::shapeutils::addPointPreview(newPolygon.back(), Qt::red);
    //        //         qDebug() << "(" << newPolygon.back().x << ", " << newPolygon.back().y << ") is poped";
    //        //         newPolygon.pop_back();
    //        //         newPolygon.insert(
    //        //             newPolygon.end(),
    //        //             dots.begin(),
    //        //             dots.end());
    //        //         for (auto p : dots) {
    //        //             rededa::pcbcommon::shapeutils::addPointPreview(p, Qt::green);
    //        //         }
    //        //     }
    //
    //        // }
    //
    //        auto prev = newPolygon[newPolygon.size() - 2];
    //        auto curr = newPolygon.back();
    //        newPolygon.pop_back();
    //        auto next = polygon[currInd];
    //        // qDebug() << "start rounding.............(" << prev.x << ", " << prev.y
    //        //          << ")->(" << curr.x << ", " << curr.y << ")->(" << next.x
    //        //          << ", " << next.y << ")";
    //        int segNum = sweepAngle(prev, next, curr) / 2;
    //        std::vector<POINT> dots;
    //        auto currToPrev = prev - curr;
    //        auto currToNext = next - curr;
    //        if (multiply<POINT>(currToPrev, currToNext) < 0) {
    //            dots =
    //                smoothAngle<geometry::POINT>(curr, prev, next, radius, segNum);
    //
    //            // qDebug()
    //            //     << "multiply<POINT>(currToPrev, currToNext) < 0, print dots: ";
    //            for (auto p : dots) {
    //                // qDebug() << "(" << p.x << ", " << p.y << ")";
    //                rededa::pcbcommon::shapeutils::addPointPreview(p, Qt::green);
    //            }
    //            newPolygon.insert(newPolygon.end(), dots.begin(), dots.end());
    //        } else {
    //            dots =
    //                smoothAngle<geometry::POINT>(curr, next, prev, radius, segNum);
    //            std::reverse(dots.begin(), dots.end());
    //            // qDebug()
    //            //     << "multiply<POINT>(currToPrev, currToNext) >= 0, print dots: ";
    //            for (auto p : dots) {
    //                // qDebug() << "(" << p.x << ", " << p.y << ")";
    //                rededa::pcbcommon::shapeutils::addPointPreview(p, Qt::green);
    //            }
    //            newPolygon.insert(newPolygon.end(), dots.begin(), dots.end());
    //        }
    //        newPolygon.push_back(next);
    //    }
    //    polygon = newPolygon; // 更新原多边形为平滑后的多边形
    //    // qDebug() << "******************************end smooth "
    //    //             "polygon*****************************************";
    //    // printPoints(newPolygon);
}

POLYGON getBoundingBox(const POLYGON& poly) {
    double leftMost = G_INF;
    double rightMost = -G_INF;
    double topMost = -G_INF;
    double bottomMost = G_INF;

    for (const auto& p : poly) {
        leftMost = std::min(p.x, leftMost);
        rightMost = std::max(p.x, rightMost);
        topMost = std::max(p.y, topMost);
        bottomMost = std::min(p.y, bottomMost);
    }

    POLYGON rv;
    rv.emplace_back(leftMost, topMost);
    rv.emplace_back(leftMost, bottomMost);
    rv.emplace_back(rightMost, bottomMost);
    rv.emplace_back(rightMost, topMost);

    return rv;
}

POLYGON getBoundingBox(const POINT& p1, const POINT& p2) {
    // 添加注释，说明这里是在确保 boundingBox 最小的边长为 1e-5。
    // 不过经过这段代码的处理，实际上最小会变成 2e-5。
    double minX = std::min(p1.x, p2.x);
    double minY = std::min(p1.y, p2.y);
    double maxX = std::max(p1.x, p2.x);
    double maxY = std::max(p1.y, p2.y);

    if (std::abs(minX - maxX) < BOUNDING_BOX_TOLERANCE) {
        minX -= BOUNDING_BOX_TOLERANCE;
        maxX += BOUNDING_BOX_TOLERANCE;
    }

    if (std::abs(minY - maxY) < BOUNDING_BOX_TOLERANCE) {
        minY -= BOUNDING_BOX_TOLERANCE;
        maxY += BOUNDING_BOX_TOLERANCE;
    }

    POLYGON rv;
    rv.emplace_back(minX, maxY);
    rv.emplace_back(minX, minY);
    rv.emplace_back(maxX, minY);
    rv.emplace_back(maxX, maxY);

    return rv;
}

bool judgeCollinear(const POINT& p1, const POINT& p2, const POINT& p3) {
    if (std::abs(p2.x - p1.x) < G_EP) {
        return std::abs(p3.x - p1.x) < G_EP;
    }

    double slope = (p2.y - p1.y) / (p2.x - p1.x);
    double collinearP3Y = p1.y + slope * (p3.x - p1.x);
    auto rv = std::abs(collinearP3Y - p3.y) < G_EP;
    return rv;
}

/**
 * @brief 纠正多边形
*/
bool correctGPolygon(const POLYGON& poly, POLYGON& rv) {
    if (poly.size() < 3) {
        rv = poly;
        return false;
    }
    auto origin = geometry::deduplication2(poly);
    int sz = origin.size();
    int startInd = -1;
    for (int i = 0; i < sz; ++i) {
        int prevInd = (i - 1 + sz) % sz;
        int nextInd = (i + 1) % sz;
        int nnxtInd = (i + 2) % sz;
        if (!judgeCollinear(origin[i], origin[nextInd], origin[nnxtInd]) &&
            !judgeCollinear(origin[prevInd], origin[i], origin[nextInd])) {
            startInd = i;
            break;
        }
    }

    if (startInd == -1) {
        rv = poly;
        return false;
    }

    rv.clear();
    rv.push_back(origin[startInd]);

    for (int i = startInd + 1; i != startInd; i = (i + 1) % sz) {
        int nextInd = (i + 1) % sz;

        if (!judgeCollinear(rv.back(), origin[i], origin[nextInd])) {
            rv.push_back(origin[i]);
        }
    }
    return rv.size() > 2;
}

/**
 * @brief 获取圆上点的切向量
 * @param center 圆心
 * @param st 起始点
 * @param direction 旋转方向
 * @return 切向量
*/
VECTOR2 getTangentVec(
    const geometry::POINT& center,
    const geometry::POINT& st,
    geometry::DIRECTION direction) {
    // 一些向量
    geometry::VECTOR2 ArcCenter2StVec = st - center;
    geometry::VECTOR2 Arc90Vec;

    // 获取圆弧切线向量，根据之前的方向来判断
    if (direction == geometry::DIRECTION::CW) {
        Arc90Vec = {-ArcCenter2StVec.y, ArcCenter2StVec.x};
    } else {
        Arc90Vec = {ArcCenter2StVec.y, -ArcCenter2StVec.x};
    }
    return Arc90Vec;
}
} // namespace geometry

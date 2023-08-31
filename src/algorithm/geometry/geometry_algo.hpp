#ifndef __GEOMETRY_ALGRO_HPP__
#define __GEOMETRY_ALGRO_HPP__

#include "geometry.h"

namespace geometry {

/*********************************点的基本运算**********************************/

/**
 * @brief 返回两点之间欧氏距离
 *        d = √((x2 - x1)² + (y2 - y1)²)
 * @param p1 点1
 * @param p2 点2
 * @return double 两点之间欧氏距离
 */
template<typename T>
double dist(T v1, T v2) {
    return (sqrt((v1.x - v2.x) * (v1.x - v2.x) + (v1.y - v2.y) * (v1.y - v2.y)));
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
template<typename T>
double geoAngleBetweenPoints(const T& A, const T& B, const T& C) {
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
template<typename T>
T mid(const T& A, const T& B) {
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
template<typename T>
bool collinear(const T& p1, const T& p2, const T& p3) {
    double p1X = p1.x;
    double p1Y = p1.y;
    return !((p3.y - p1Y) * (p2.x - p1X) - (p2.y - p1Y) * (p3.x - p1X));
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
template<typename V, typename T>
V computePolarAngle(const T& p1, const T& p2) {
    if (p1 == p2) {
        return 0;
    }
    V dx = p1.x - p2.x;
    V dy = p1.y - p2.y;
    V angle = std::atan2(dy, dx);
    if (angle < 0) {
        angle += 2 * M_PI;
    }
    return angle;
}

static POINT rotatePoint(const POINT& p, const POINT& fixed, double angle) {
    return {};
}
/******************************************************************************/

/**********************************向量基本运算*********************************/
/**
 * @brief 计算两个向量之间的夹角，即 p1-op 和 p2-op 之间的夹角
 *
 * @param p1 向量1的端点
 * @param p2 向量2的端点
 * @param op 共同的起点
 * @return double 夹角的大小（单位：度）
 */
template<typename T>
double sweepAngle(const T& p1, const T& p2, const T& op) {
    double theta = std::atan2(p1.y - op.y, p1.x - op.x) - std::atan2(p2.y - op.y, p2.x - op.x);
    if (theta > M_PI)
        theta -= 2 * M_PI;
    if (theta < -M_PI)
        theta += 2 * M_PI;

    theta = std::abs(theta * 180.0 / M_PI);
    return theta;
}
/**
 * @brief 向量的点积
 *        r = (p1-op)和(p2-op)的点积，如果两个向量都非零向量
 *        r<0：两矢量夹角为锐角
 *        r=0：两矢量夹角为直角
 *        r>0：两矢量夹角为钝角
 * @tparam T 向量类型
 * @param sp 向量SP的坐标
 * @param ep 向量EP的坐标
 * @param op 向量OP的坐标
 * @return double 两个向量的点积结果
 */
template<typename T>
double dotmultiply(T sp, T ep, T op) {
    return ((sp.x - op.x) * (ep.x - op.x) + (sp.y - op.y) * (ep.y - op.y));
}

template<typename T>
double dotMultiply(const T& vec1, const T& vec2) {
    return vec1.x * vec2.x + vec1.y * vec2.y;
}

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
 * @brief 向量的模
 *
 * @param sp
 * @param ep
 * @return double
 */
template<typename T>
double g_module(T sp, T ep = {0, 0}) {
    return sqrt((ep.x - sp.x) * (ep.x - sp.x) + (ep.y - sp.y) * (ep.y - sp.y));
}

/**
 * @brief 计算两个二维向量的夹角，参数为两个二维向量的 x, y 分量
 *
 * @tparam T 向量类型
 * @param p1 第一个二维向量
 * @param p2 第二个二维向量
 * @return double 夹角的度数
 */
template<typename T>
double calcAngleBetweenTwoVectors(const T& p1, const T& p2) {
    /* 计算点积 */
    double dot_product = p1.x * p2.x + p1.y * p2.y;
    /* 计算两个向量的模长 */
    double len_vec1 = std::hypot(p1.x, p1.y);
    double len_vec2 = std::hypot(p2.x, p2.y);
    /* 检查点积是否超出有效范围 */
    if (dot_product <= -len_vec1 * len_vec2) {
        return 180.0; // 夹角为180度
    } else if (dot_product >= len_vec1 * len_vec2) {
        return 0.0;   // 夹角为0度
    }
    /* 计算夹角的余弦值 */
    double cos_angle = dot_product / (len_vec1 * len_vec2);
    /* 计算夹角的弧度值 */
    double angle = std::acos(cos_angle);
    /* 转换为角度值并返回结果 */
    return angle / M_PI * 180.0;
}

/**
 * @brief 求线段 AB 顺时针方向的单位向量
 *
 * @tparam T
 * @param A
 * @param B
 * @return T
 */
template<typename T>
T verticalCWNormalized(const T& A, const T& B) {
    return rotateCW90Degree(normalize(B - A));
}

/**
 * @brief 求线段 AB 逆时针方向的单位向量
 *
 * @tparam T
 * @param A
 * @param B
 * @return T
 */
template<typename T>
T verticalCCWNormalized(const T& A, const T& B) {
    auto v = verticalCWNormalized(A, B);
    return {-v.x, -v.y};
}

/**
 * @brief 将点相对(0,0)点顺时针旋转90°
 * @tparam T 点的数据类型
 * @param v 要旋转的点
 * @return T 旋转后的点
 */
template<typename T>
T rotateCW90Degree(const T& v) {
    return {v.y, -v.x};
}

/**
 * @brief 将点相对(0,0)点逆时针旋转90°
 * @tparam T 点的数据类型
 * @param v 要旋转的点
 * @return T 旋转后的点
 */
template<typename T>
T rotateCCW90Degree(const T& v) {
    return {-v.y, v.x};
}

/******************************************************************************/

/********************************线段、直线基本运算*******************************/

/**
 * @brief 判断点与线段的关系,用途很广泛
 *        本函数是根据下面的公式写的，P是点C到线段AB所在直线的垂足
 *                       AC dot AB
 *        r =            ---------
 *                       ||AB||^2
 *             (Cx-Ax)(Bx-Ax) + (Cy-Ay)(By-Ay)
 *          = -------------------------------
 *                          L^2
 *        r 有下列含义:
 *          r=0    P = A
 *          r=1    P = B
 *          r<0    P 在AB的背面延伸
 *          r>1    P 在AB的前方延伸
 *          0<r<1  P 在AB范围内
 *
 * @param p
 * @param l
 * @return double
 */
static double relation(POINT p, LINE_SEG l) {
    return dotmultiply(p, l._et, l._st) / (dist(l._st, l._et) * dist(l._st, l._et));
}

/**
 * @brief 判断一点是否在线段上
 * @param p 待判断的点
 * @param l 线段
 * @return bool 如果点在线段上返回true，否则返回false
 */
static bool pointIsOnLineSeg(POINT p, LINE_SEG l) {
    return fabs(dist(p, l._st) + dist(p, l._et) - dist(l._st, l._et)) < G_EP;
}

/**
 * @brief 求点到线段所在直线垂线的垂足
 *
 * @param p
 * @param l
 * @return POINT
 */
static POINT perpendicular(POINT p, LINE_SEG l) {
    double r = relation(p, l);
    POINT tp;
    tp.x = l._st.x + r * (l._et.x - l._st.x);
    tp.y = l._st.y + r * (l._et.y - l._st.y);
    return tp;
}

/**
 * @brief 求点p到线段l的最短距离,并返回线段上距该点最近的点np
 * 注意：np是线段l上到点p最近的点，不一定是垂足
 * @param p 点的坐标
 * @param l 线段的起点和终点
 * @param np 最近点的坐标
 * @return double 最短距离
 */
static double ptolinesegdist(const POINT& p, const LINE_SEG& l, POINT& np) {
    double r = relation(p, l);
    if (r < 0) {
        np = l._st;
        return dist(p, l._st);
    }
    if (r > 1) {
        np = l._et;
        return dist(p, l._et);
    }
    np = perpendicular(p, l);
    return dist(p, np);
}

/**
 * @brief 判断两线段是否相交(包括端点)
 * 判断P1P2跨立Q1Q2的依据是：(P1 - Q1) × (Q2 - Q1) * (Q2 - Q1) × (P2 - Q1) >= 0
 * 判断Q1Q2跨立P1P2的依据是：(Q1 - P1) × (P2 - P1) * (P2 - P1) × (Q2 - P1) >= 0
 * @param u 第一条线段
 * @param v 第二条线段
 * @return true 相交
 * @return false 不相交
 */
static bool intersect(const LINE_SEG& u, const LINE_SEG& v) {
    return (
        std::max(u._st.x, u._et.x) >= std::min(v._st.x, v._et.x) && // 排斥实验
        std::max(v._st.x, v._et.x) >= std::min(u._st.x, u._et.x) &&
        std::max(u._st.y, u._et.y) >= std::min(v._st.y, v._et.y) &&
        std::max(v._st.y, v._et.y) >= std::min(u._st.y, u._et.y) &&
        multiply(v._st, u._et, u._st) * multiply(u._et, v._et, u._st) >= 0 && // 跨立实验
        multiply(u._st, v._et, v._st) * multiply(v._et, u._et, v._st) >= 0);
}

/**
 * @brief 判断两条直线是否相交，相交则给出交点
 *        x = (b1 * c2 - b2 * c1) / (a1 * b2 - a2 * b1)
 *        y = (a2 * c1 - a1 * c2) / (a1 * b2 - a2 * b1)
 * @param u 第一条直线
 * @param v 第二条直线
 * @param l 相交点的坐标
 * @return true 相交
 * @return false 不相交
 */
static bool intersect(const LINE& u, const LINE& v, POINT& l) {
    double denominator = u.a * v.b - v.a * u.b;
    if (denominator != 0) {
        l.x = (u.b * v.c - v.b * u.c) / denominator;
        l.y = (v.a * u.c - u.a * v.c) / denominator;
        return true;
    }
    return false;
}

/**
 * @brief 判断两条线段是否相交，相交则给出交点
 * @param u 第一条线段
 * @param v 第二条线段
 * @param l 相交点的坐标
 * @return true 相交
 * @return false 不相交
 */
static bool intersect(const LINE_SEG& u, const LINE_SEG& v, POINT& l) {
    bool res = intersect(LINE(u._st, u._et), LINE(v._st, v._et), l);
    if (res) {
        if (pointIsOnLineSeg(l, u) && pointIsOnLineSeg(l, v)) {
            return true;
        }
    }
    return false;
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
static bool pointsOnSameSide(const POINT& p1, const POINT& p2, const POINT& a, const POINT& b) {
    auto dir1 = normalize(perpendicular(p1, {a, b}) - p1);
    auto dir2 = normalize(perpendicular(p2, {a, b}) - p2);
    return dir1.x * dir2.x >= 0 && dir1.y * dir2.y >= 0;
}

/**
 * @brief 给定一个点和一组线段，求离这个点最近的线段
 * @param p 给定的点
 * @param segs 线段集合
 * @return LINE_SEG 最近的线段
 */
static LINE_SEG nearestLineSeg(const POINT& p, const std::vector<LINE_SEG>& segs) {
    if (segs.empty()) {
        throw "empty set";
    }
    POINT np;
    double minDist = ptolinesegdist(p, segs[0], np);
    LINE_SEG minDistSeg = segs[0];
    for (const LINE_SEG& seg : segs) {
        double dist = ptolinesegdist(p, seg, np);
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
template<typename T>
static LINE pt2line(const T& p1, const T& p2) {
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
template<typename T>
static bool includedAngle(const T& p1, const T& p2, double& angle) {
    if (p2.x == p1.x)
        return false;
    angle = std::abs(std::atan((p2.y - p1.y) / (p2.x - p1.x)) * 180.0 / M_PI);
    return true;
}

/**
 * @brief 计算两个点所在直线与x轴夹角的函数
 *
 * @param start 起点
 * @param end 终点
 * @return double 夹角的大小（单位：度）
 */
template<typename T>
double getAngleWithXAxis(const T& start, const T& end) {
    auto actual = end - start;
    double angle = std::atan2(actual.y, actual.x) * 180 / M_PI;
    if (angle < 0) {
        angle += 360;
    }
    return angle;
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
template<typename T>
LINE_SEG verticalLineByDist(const T& A, const T& B, double dx, double dy) {
    T uab = normalize(B - A);                  // AB的单位向量
    T M = mid(A, B);                           // AB的中点
    T midCD = M + uab * (dist(A, B) / 2 + dy); // CD线段的中点
    T u1 = verticalCCWNormalized(A, B);        // AB的逆时针垂直单位向量
    T u2 = verticalCWNormalized(A, B);         // AB的顺时针垂直单位向量
    return {midCD + u1 * dx, midCD + u2 * dx};
}

/**
 * @brief 平滑化 线段1 和 线段2 组成的角
 *          st /\                   _ _
 *            /  \        ---->    /   \
 *       e2  /    \ e1            /     \
 *
 * @param st 起点
 * @param e1 终点1
 * @param e2 终点2
 * @param r 倒角的半径
 * @param segNum 平滑的段数
 * @return std::vector<geometry::POINT> 结果点集
 */
template<typename T>
std::vector<T> smoothAngle(const T& st, const T& e1, const T& e2, double r, int segNum) {
    /**
     * 逻辑：
     * 1. 求出两条线段角平分线的单位向量
     * 2. 求出倒角的圆心
     * 3. 求出圆心到线段的两个垂足
     * 4. 根据分割段数，用第一个垂足绕圆心旋转得到若干点
     * 5. 返回垂足和若干点组成的点集
     */
    VECTOR2 SE1(e1 - st);
    VECTOR2 SE2(e2 - st);
    VECTOR2 normalSE1 = normalize(SE1);
    VECTOR2 normalSE2 = normalize(SE2);
    POINT normalRay = POINT(normalSE1.x + normalSE2.x, normalSE1.y + normalSE2.y);

    double sa = sweepAngle(e1, e2, st);
    if (sa <= 0.0) {
        // 三点共线或者是退化角，返回起点即可
        return {st};
    }

    // 计算圆心
    POINT center = st + normalRay * r;
    // 求垂足
    T pd1 = perpendicular(center, {st, e1});
    T pd2 = perpendicular(center, {st, e2});

    // 产生点
    std::vector<T> dots;
    dots.emplace_back(pd1);
    // 防止除0
    if (segNum - 0 > G_EP) {
        double supplementSA = 180 - sa;
        double portion = supplementSA / segNum;
        T pt = pd1;
        while (segNum != 1) {
            pt = rotatePoint(pt, center, portion);
            dots.emplace_back(pt);
            segNum--;
        }
    }
    dots.emplace_back(pd2);
    return dots;
}

/******************************************************************************/

/*********************************多边形运算************************************/

/**
 * @brief 判断多边形是否相交（线段法）
 * @tparam T 多边形点的类型
 * @param A 第一个多边形的点集合
 * @param B 第二个多边形的点集合
 * @return 是否相交（true表示相交，false表示不相交）
 */
template<typename T>
bool polygonIntersect(const T& A, const T& B) {
    for (int i = 0; i < A.size(); ++i) {
        int endA = i == A.size() - 1 ? 0 : i + 1;
        LINE_SEG seg1 = {POINT(A[i].x, A[i].y), POINT(A[endA].x, A[endA].y)};
        for (int j = 0; j < B.size(); ++j) {
            int endB = j == B.size() - 1 ? 0 : j + 1;
            LINE_SEG seg2 = {POINT(B[j].x, B[j].y), POINT(B[endB].x, B[endB].y)};
            if (intersect(seg1, seg2)) {
                return true;
            }
        }
    }
    return false;
}

/**
 * @brief 多边形有向面积
 *        顺时针排序，结果为正
 *        逆时针排序，结果为负
 *
 * @tparam T
 * @param polygon
 * @return double
 */
template<typename T>
double polygonArea(const std::vector<T>& polygon) {
    double area = 0.0;
    int n = polygon.size();
    for (int i = 0; i < n; ++i) {
        const T& p1 = polygon[i];
        const T& p2 = polygon[(i + 1) % n];
        area += (p2.x - p1.x) * (p2.y + p1.y);
    }
    return area / 2.0;
}

/**
 * @brief 多边形外扩/内缩算法
 * @tparam T 多边形点的类型
 * @param polygon 多边形的点集合
 * @param gap 外扩/内缩距离
 * @param expand true 表示外扩，false 表示内缩
 * @return 操作是否成功（true 表示成功，false 表示失败）
 *
 * 1. 计算顶点邻边向量（注意向量方向，外扩与内缩向量方向相反）
 * 2. 利用邻边向量（单位化）叉乘得到 SinA 值
 * 3. 判断顶点是凹点还是凸点（邻边向量叉乘得到的法向量与顶点法线同向为凸点，否则为凹点）
 * 4. 如果 SinA 值为 0，则表示邻边向量共线，可以舍弃该顶点（对多边形顶点做简化操作）
 * 5. 对于凸点：Q = P + d / sinA * (Normalize(QN) + Normalize(QM))
 *    对于凹点：Q = P - d / sinA * (Normalize(QN) + Normalize(QM))
 * 注意事项：1. 要保证多边形是逆时针，否则顶点判断将会失效
 *         2. 要保证传入的多边形没有重复的点
 */
template<typename T>
bool polygonOffset(T& polygon, double gap, bool expand) {
    if (polygonArea(polygon) > 0.0f) {
        std::reverse(polygon.begin(), polygon.end());
    }
    size_t vertexCnt = polygon.size();
    T oldPolygon = polygon;
    polygon.clear();
    std::vector<ADJACENT_VECTOR2> adjVectors {};
    int vertexSize = (int)vertexCnt - 1;
    // 根据顶点生成向量数据
    for (size_t i = 0; i < vertexCnt; ++i) {
        VECTOR2 vp1 = oldPolygon[i] - oldPolygon[i == vertexSize ? 0 : i + 1];
        VECTOR2 vp2 = oldPolygon[i] - oldPolygon[i == 0 ? vertexSize : i - 1];
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
        auto sin = std::abs(multiply(normalize(v1), normalize(v2)));
        // sin 等于 0 时，向量共线
        if (sin > 0) {
            // 凹点取反
            if (!adjVectors.at(i).convex) {
                sin = -sin;
            }
            VECTOR2 offset = (normalize(v1) + normalize(v2)) * (gap / sin);
            auto vec = POINT(oldPolygon[i].x, oldPolygon[i].y);
            // 内缩取反
            if (!expand) {
                offset = {-offset.x, -offset.y};
            }
            polygon.emplace_back(vec + offset);
        }
    }
    return true;
}

template<typename T>
void polygonSmooth(std::vector<T>& polygon, double radius, int segNum) {
    // 纠正多边形顺序
    if (polygonArea(polygon) > 0.0) {
        std::reverse(polygon.begin(), polygon.end());
    }

    std::vector<T> newPolygon; // 用于存储平滑后的多边形

    for (size_t i = 0; i < polygon.size(); ++i) {
        T curr = polygon[i];
        T prev = polygon[(i == 0) ? (polygon.size() - 1) : (i - 1)];
        T next = polygon[(i + 1) % polygon.size()];

        double minSeg = std::min(dist(curr, next), dist(curr, prev));

        // 线段长度小于倒角半径就不让倒
        if (minSeg <= radius) {
            newPolygon.push_back(curr);
            continue;
        }

        double angle = sweepAngle(prev, next, curr);
        if (angle <= 90.0) {
            std::vector<T> dots = smoothAngle<geometry::POINT>(curr, prev, next, radius, segNum);
            newPolygon.insert(newPolygon.end(), dots.begin(), dots.end());
        } else {
            newPolygon.push_back(curr);
        }
    }

    polygon = newPolygon; // 更新原多边形为平滑后的多边形
}

/**
 * @brief 检查点是否在多边形内部
 * @tparam T 点类型
 * @param point 待检查的点
 * @param polygon 多边形点集
 * @return true 如果点在多边形内部
 * @return false 如果点在多边形外部或边界上
 */
template<typename T>
bool ptInPolygon(const T& point, const std::vector<T>& polygon) {
    int intersectCnt = 0;
    int numVertices = polygon.size();
    for (int i = 0; i < numVertices; i++) {
        const T& vertex1 = polygon[i];
        const T& vertex2 = polygon[(i + 1) % numVertices];
        // 检查点是否在顶点之间的水平射线路径上
        if ((vertex1.y > point.y) != (vertex2.y > point.y)) {
            // 计算水平射线与边的交点的 x 坐标
            double intersectX = (vertex2.x - vertex1.x) * (point.y - vertex1.y) / (vertex2.y - vertex1.y) + vertex1.x;
            // 检查交点是否在射线的右侧
            if (point.x < intersectX) {
                intersectCnt++;
            }
        }
    }
    // 如果交点数是奇数，则点在多边形内部
    return (intersectCnt % 2) == 1;
}

/**
 * @brief 检查点是否在多边形内部
 *
 * @param pt 待检查的点
 * @param polygon 多边形点集
 * @return true 点在多边形内部
 * @return false 点在多边形外部或者边界上
 */
static bool ptInSegPolygon(const POINT& pt, const std::vector<LINE_SEG>& polygon) {
    int intersections = 0;

    for (const LINE_SEG& edge : polygon) {
        // 点在边的起点或终点上
        if ((edge._st.x == pt.x && edge._st.y == pt.y) || (edge._et.x == pt.x && edge._et.y == pt.y)) {
            return true; // 点在多边形的顶点上
        }
        // 点的 y 坐标在边的 y 范围内
        if ((edge._st.y > pt.y) != (edge._et.y > pt.y)) {
            // 计算射线和边的交点的 x 坐标
            double intersectX =
                (edge._et.x - edge._st.x) * (pt.y - edge._st.y) / (edge._et.y - edge._st.y) + edge._st.x;
            // 射线与边的交点在点的右侧
            if (intersectX > pt.x) {
                intersections++;
            }
        }
    }
    // 如果交点数为奇数，则点在多边形内部
    return (intersections % 2 == 1);
}

/**
 * @brief 检查多边形是否在多边形内部
 * @tparam T
 * @param innerPolygon
 * @param outerPolygon
 * @return
 */
template<typename T>
bool polygonInsidePolygon(const std::vector<T>& innerPolygon, const std::vector<T>& outerPolygon) {
    for (const T& point : innerPolygon) {
        if (!ptInPolygon(point, outerPolygon)) {
            return false;
        }
    }
    return true;
}

/**
 * @brief 检查点是否在多边形边界上
 * @tparam T 点类型
 * @param point 待检查的点
 * @param polygon 多边形点集
 * @return true 如果点在多边形边界上
 * @return false 如果点在多边形内部或外部
 */
template<typename T>
bool ptOnPolygon(const T& point, const std::vector<T>& polygon) {
    size_t numVertices = polygon.size();
    for (size_t i = 0; i < numVertices; ++i) {
        const T& vertex1 = polygon[i];
        const T& vertex2 = polygon[(i + 1) % numVertices];
        // 检查点是否在当前边的水平范围内
        if (abs(dist(point, vertex1) + dist(point, vertex2) - dist(vertex1, vertex2)) < G_EP) {
            return true;
        }
    }
    return false; // 点不在边界上，返回 false
}

/**
 * @brief p1和p2是否在polygon的同一条边上
 */
template<typename T>
bool ptOnSameSide(const T& p1, const T& p2, const std::vector<T>& polygon) {
    int sz = polygon.size();
    for (int i = 0; i < sz; ++i) {
        LINE_SEG seg {polygon[i], polygon[(i + 1) % sz]};
        if (pointIsOnLineSeg(p1, seg) && pointIsOnLineSeg(p2, seg)) {
            return true;
        }
    }
    return false;
}

/**
 * 计算多边形和给定线段的交点
 * @param polygon 多边形点向量
 * @param start 起点坐标
 * @param end 终点坐标
 * @return 一个点向量，表示多边形和线段的交点
 */
template<typename T>
std::vector<T> getPolygonSegmentIntersections(const std::vector<T>& polygon, const T& start, const T& end) {
    std::vector<T> res;
    T ipt;
    int sz = polygon.size();
    for (int i = 0; i < sz; ++i) {
        LINE_SEG seg(polygon.at(i), polygon.at((i + 1) % sz));
        if (intersect(seg, {start, end}, ipt)) {
            res.emplace_back(ipt);
        }
    }
    return res;
}

/**
 * @brief 去重
 * @tparam T 点
 * @param polygon 多边形
 * @return std::vector<T> 去重后的多边形
 */
template<typename T>
std::vector<T> deduplication(const std::vector<T>& polygon) {
    std::set<std::pair<double, double>> pts;
    std::vector<T> res;
    for (const T& pt : polygon) {
        if (pts.insert(std::make_pair(pt.x, pt.y)).second) { // 插入成功
            res.emplace_back(pt);
        }
    }
    return res;
}

/**
 * @brief 计算正多边形的内接距
 *        ---
 *      /     \
 *      \     /
 *        ---
 * @tparam T 外接圆半径的类型
 * @param n 正n边形
 * @param radius 外接圆半径
 * @return T 内接距
 */
template<typename T>
T nPolygonInscribedDistance(int n, T radius) {
    return radius * std::cos(M_PI / n);
}

/**
 * @brief 计算多边形重心
 *
 * @tparam T
 * @param points
 * @return T
 */
template<typename T>
T computeCentroid(const std::vector<T>& points) {
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

/**
 * @brief 纠正多边形，使点集按逆时针序排列
 *
 * @tparam T
 * @param points
 * @return std::vector<T>
 */
template<typename T>
std::vector<T> sortPointsInCounterClockwise(std::vector<T>& points) {
    // Not enough points to form a polygon
    if (points.size() < 3) {
        return points;
    }
    // Find the leftmost and bottommost point as the starting point
    T startingPoint = points[0];
    for (const T& point : points) {
        if (point.y < startingPoint.y || (point.y == startingPoint.y && point.x < startingPoint.x)) {
            startingPoint = point;
        }
    }

    // Sort the points based on the angle with the starting point
    std::sort(points.begin(), points.end(), [&](const T& p1, const T& p2) {
        double crossProduct = multiply(p1, p2, startingPoint);
        if (std::abs(crossProduct) < G_EP) {
            // Points are collinear, compare distance from the starting point
            double dist1 = std::hypot(p1.x - startingPoint.x, p1.y - startingPoint.y);
            double dist2 = std::hypot(p2.x - startingPoint.x, p2.y - startingPoint.y);
            return dist1 < dist2;
        }
        return crossProduct > 0;
    });

    return points;
}

/**
 * @brief 求两个多边形是否相交
 */
static bool isPolyPolyIntersect(POLYGON p1, POLYGON p2) {
    for (auto p : p1) {
        if (ptInPolygon(p, p2)) {
            return true;
        }
    }

    for (auto p : p2) {
        if (ptInPolygon(p, p1)) {
            return true;
        }
    }

    for (int i = 0; i < p1.size(); ++i) {
        for (int j = 0; j < p2.size(); ++j) {
            if (intersect({p1[i], p1[(i + 1) % p1.size()]}, {p2[j], p2[(j + 1) % p2.size()]})) {
                return true;
            }
        }
    }
    return false;
}

/**
 * @brief 移动一个多边形
 */
static POLYGON movePolygonBy(const POLYGON& plg, const POINT& dir) {
    POLYGON ret = plg;
    for (auto& p : ret) {
        p.x += dir.x;
        p.y += dir.y;
    }
    return ret;
}

/**
 * @brief 判断一个多边形是否是凸多边形
 * @param polygon
 * @return
 */
static bool isConcavePolygon(const POLYGON& polygon) {
    int vertexCount = polygon.size();

    for (int i = 0; i < vertexCount; ++i) {
        const geometry::POINT& current = polygon[i];
        const geometry::POINT& previous = polygon[(i - 1 + vertexCount) % vertexCount];
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

/******************************************************************************/

/*********************************圆弧、圆基本运算********************************/

/**
 * @brief 角度转弧度
 *
 * @param angle 角度
 * @return double 弧度
 */
static double degree2Radian(double angle) {
    return angle * M_PI / 180.0;
}

/**
 * @brief 弧度转角度
 *
 * @param angle 弧度
 * @return double 角度
 */
static double radian2degree(double angle) {
    return angle * 180 / M_PI;
}

/**
 * @brief 求点到圆弧线段的最近距离
 *        如果点到圆心的射线与圆弧有交点，则返回交点
 * @param p 点P
 * @param a 圆弧
 * @param np 最近点
 * @return double 最近距离
 */
static double ptoarcsegdist(POINT p, ARC a, POINT& np) {
    // using namespace rededa::Vector;
    // LINE_SEG se(a._st, a._et);
    // double   spdist = dist(a._st, p);
    // double   epdist = dist(a._et, p);
    // if (intersect(pt2line(a._ct, p), pt2line(a._st, a._et), np)) {
    //     auto r = relation(np, se);
    //     if (r >= 0 && r <= 1 && dotmultiply(p, np, a._ct) > 0) {
    //         auto cp(p - a._ct);
    //         auto normalCP = normalized(cp);
    //         np            = a._r * normalCP + a._ct;
    //         return std::abs(module(cp) - a._r);
    //     }
    // }
    // np = spdist < epdist ? a._st : a._et;
    // return std::min(spdist, epdist);
    return 0.0;
}

/**
 * @brief 求点P到圆弧的最近点
 * @param p 点P
 * @param arc 圆弧
 * @param np 最近点
 * @return double 最近点到P的距离
 */
static double closestPointToArc(POINT p, ARC arc, POINT& np) {
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

/**
 * @brief 判断点是否在圆内（包括边界）
 *        因为圆为凸集，所以判断点集、折线、多边形是否在圆内时，
 *        只需要逐一判断点是否在圆内即可。
 * @tparam T 点的类型，可以是自定义的 Point 类型
 * @param o 目标点
 * @param r 半径
 * @param p 圆心
 * @return true 如果点在圆内（包括边界）
 * @return false 如果点不在圆内
 */
template<typename T>
bool pincircle(T o, double r, T p) {
    // 计算点到圆心的距离的平方
    double d2 = (p.x - o.x) * (p.x - o.x) + (p.y - o.y) * (p.y - o.y);
    // 计算圆的半径的平方
    double r2 = r * r;
    // 判断点是否在圆内（包括边界）
    return d2 < r2 || std::abs(d2 - r2) < G_EP;
}

/**
 * @brief 判断两个圆的关系
 * @tparam T 圆心的类型，可以是自定义的 Point 类型
 * @param p1 圆1的圆心
 * @param r1 圆1的半径
 * @param p2 圆2的圆心
 * @param r2 圆2的半径
 * @return 1 两个圆相离
 * @return 2 圆2在圆1外切
 * @return 3 两个圆相交
 * @return 4 圆2在圆1内切
 * @return 5 圆2在圆1内含
 */
template<typename T>
int circlerelation(T p1, double r1, T p2, double r2) {
    double d = sqrt((p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y));

    if (std::fabs(d - r1 - r2) < G_EP) // 必须保证前两个条件先被判定！
        return 2;
    if (std::fabs(d - std::fabs(r1 - r2)) < G_EP)
        return 4;
    if (d > r1 + r2)
        return 1;
    if (d < std::fabs(r1 - r2))
        return 5;
    if (std::fabs(r1 - r2) < d && d < r1 + r2)
        return 3;
    return 0; // 表示错误！
}

/**
 * @brief 根据三个点确定外接圆
 * @tparam T 点的类型，可以是自定义的 Point 类型
 * @param p1 第一个点
 * @param p2 第二个点
 * @param p3 第三个点
 * @param r 外接圆的半径
 * @return T 外接圆的圆心
 */
template<typename T>
T trip2circle(T p1, T p2, T p3, double& r) {
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
    return std::move(T(X, Y));
}

/**
 * @brief 将圆弧转换为由多个线段组成的点序列
 * @tparam T 点的类型，可以是自定义的 Point 类型
 * @param cx 圆心的 x 坐标
 * @param cy 圆心的 y 坐标
 * @param r 圆的半径
 * @param startAngle 起始角度（弧度）
 * @param endAngle 终止角度（弧度）
 * @param numSegments 线段的数量
 * @return std::vector<T> 包含多个线段点的容器
 */
template<typename T>
std::vector<T> arc2points(double cx, double cy, double r, double startAngle, double endAngle, int numSegments) {
    std::vector<T> pts;
    double theta = (endAngle - startAngle) / numSegments;
    double tan_factor = tan(theta);
    double radialFactor = cos(theta);
    double x = r * cos(startAngle);
    double y = r * sin(startAngle);
    for (int i = 0; i < numSegments; i++) {
        pts.emplace_back(cx + x, cy + y);
        double tx = -y;
        double ty = x;
        x += tx * tan_factor;
        y += ty * tan_factor;
        x *= radialFactor;
        y *= radialFactor;
    }
    pts.emplace_back(cx + r * cos(endAngle), cy + r * sin(endAngle));
    return pts;
}

/**
 * @brief 判断点是否在圆上
 * @param c 圆
 * @param p 点
 * @return bool 如果点在圆上返回 true，否则返回 false
 */
static bool pointIsOnCircle(CIRCLE c, POINT p) {
    return dist(c.c, p) - c.r < G_EP;
}

/**
 * @brief 获取两条直线的交点
 */
static POINT getIntersection(const LINE& l1, const LINE& l2) {
    double denominator = l1.a * l2.b - l1.b * l2.a;

    // 如果分母为 0，则两条直线平行，返回一个特殊值表示无穷远点
    if (denominator == 0) {
        return POINT(G_INF, G_INF);
    }

    // 计算交点的 x 和 y 坐标
    double x = (l1.b * l2.c - l1.c * l2.b) / denominator;
    double y = (l1.c * l2.a - l1.a * l2.c) / denominator;

    // 返回交点坐标
    return POINT(x, y);
}

/**
 * @brief
 * 给定一个圆、一个方向向量和一个角度，求出圆心在这个方向向量上的射线上的圆外一点，这个点与圆形成的两条切线形成的角度正好是给定角度
 * @param circle 圆
 * @param dir 方向向量
 * @param angle 角度
 * @return POINT 圆外的点
 */
static POINT getPointOutsideCircleWithDesiredAngle(const CIRCLE& circle, VECTOR2 dir, double angle) {
    double alpha = angle / 2;
    double len = circle.r / sin(alpha / 180 * M_PI);
    return circle.c + normalize(dir) * len;
}

/**
 * @brief 给定一个圆、一个方向向量和一个距离，求边缘朝这个方向移动这些距离+r后的点
 * @param circle 圆
 * @param dir 方向向量
 * @param dist 距离
 * @return POINT 移动后的点
 */
static POINT getPointMoveFromEdge(const CIRCLE& circle, VECTOR2 dir, double dist) {
    double moveDist = circle.r + dist;
    return circle.c + normalize(dir) * moveDist;
}

/**
 * @brief 给定一个点和一个方向向量，一个距离，求这个点沿着这个方向向量移动这个距离之后的点
 * @param p 点
 * @param dir 方向向量
 * @param dist 距离
 * @return POINT 移动后的点
 */
static POINT getPointMoveFromPoint(const POINT& p, VECTOR2 dir, double dist) {
    dir = normalize(dir);
    return p + dir * dist;
}

/**
 * @brief 判断一个点是否在圆内
 * @param circle 圆
 * @param p 点
 * @return bool 如果点在圆内，返回 true；否则，返回 false。
 */
static bool pointIsInCircle(const CIRCLE& circle, POINT p) {
    return dist(circle.c, p) <= circle.r + G_EP;
}

/******************************************************************************/

} // namespace geometry

#endif // __GEOMETRY_ALGRO_HPP__
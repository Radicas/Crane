/**
 * @file geometry_algo_core.h
 * @author Radica
 * @brief 几何库核心
 * @version 0.1
 * @date 2023-09-17
 * @note TODO: 需要进一步优化，去掉其他外部依赖
 * @copyright Copyright (c) 2023 上海弘快科技有限公司
 * 
 */
#ifndef GEOMETRY_ALGO_CORE_H
#define GEOMETRY_ALGO_CORE_H
#include <QRectF>
#include <cmath>
#include <set>
#include <unordered_set>

#include <iostream>
#include <vector>
#include <cmath>
#include <QPointF>

namespace geometry {

// clang-format off
const double G_INF = 1E200; // 极大值 infinity
const double G_EP = 1E-5;   // 极小值 epsilon
const double G_EP_REVERT_DOTS = 1; // 经过测试得到的合适的逆转线段到圆弧的阈值
const int G_MAX_L = 300;                  // 最大长度
const double G_PI = 3.14159265;           // 圆周率
constexpr const double VALID_MIN = -28347.0; // dpi为72的情况下10米转为像素的数值
constexpr const double VALID_MAX = 28347.0;  // dpi为72的情况下10米转为像素的数值
// clang-format on

/**
* @brief 计算两个向量之间的夹角，即 p1-op 和 p2-op 之间的夹角
*
* @param p1 向量1的端点
* @param p2 向量2的端点
* @param op 共同的起点
* @return double 夹角的大小（单位：角度）
*/
template<typename T>
double sweepAngle(const T& p1, const T& p2, const T& op) {
    double theta = std::atan2(p1.y - op.y, p1.x - op.x) -
                   std::atan2(p2.y - op.y, p2.x - op.x);
    if (theta > M_PI)
        theta -= 2 * M_PI;
    if (theta < -M_PI)
        theta += 2 * M_PI;

    theta = std::abs(theta * 180.0 / M_PI);
    return theta;
}

/**
* @brief 点
* @note 笛卡尔坐标系和qt坐标系原点和x轴方向相同情况下，geometry的x和y是笛卡尔坐标系的
*/
struct POINT {
    double x;
    double y;

    POINT(double a = 0, double b = 0) {
        x = a;
        y = b;
    } //constructor

    POINT(const POINT& rhs) :
        x(rhs.x),
        y(rhs.y) {
#ifdef DEBUG_BUILD
        if (std::isnan(x) || std::isnan(y)) {
            int a = 1;
        }
#endif
    }

    POINT& operator=(const POINT& rhs) {
        if (&rhs == this) {
            return *this;
        }
        x = rhs.x;
        y = rhs.y;

#ifdef DEBUG_BUILD
        if (std::isnan(x) || std::isnan(y)) {
            int a = 1;
        }
#endif
        return *this;
    }

    // [[deprecated]]
    POINT(const QPointF& point) :
        x(point.x()),
        y(-point.y()) {
    }

    // [[deprecated]]
    operator QPointF() const {
        return QPointF(x, -y);
    }

    // [[deprecated]]
    //    POINT(const rededa::Point& point) :
    //        x(point.getX().toPx()),
    //        y(-point.getY().toPx()) {
    //    }

    // [[deprecated]]
    //    rededa::Point toPoint() {
    //        return rededa::Point::fromPx(x, -y);
    //    }

    // [[deprecated]]
    QPointF toPointF() const {
        return {x, -y};
    }

    inline bool isValid() const {
        return VALID_MIN < x && x < VALID_MAX && VALID_MIN < y && y < VALID_MAX;
    }

    static POINT getDefault() {
        return POINT(G_INF, G_INF);
    }

    POINT operator+(const POINT& rhs) const {
        return POINT(this->x + rhs.x, this->y + rhs.y);
    }

    bool isAlmostSame(const POINT& other) {
        return this->x - other.x < 1e-3 && this->y - other.y < 1e-3;
        // return dist(*this, other) < 0.01;
    }

    POINT& operator+=(const POINT& rhs) {
        this->x += rhs.x;
        this->y += rhs.y;
        return *this;
    }

    POINT operator*(const double& multiplier) const {
        return POINT(this->x * multiplier, this->y * multiplier);
    }

    double operator*(const POINT& rhs) const {
        return x * rhs.x + y * rhs.y;
    }

    POINT& operator*=(const double& multiplier) {
        this->x *= multiplier;
        this->y *= multiplier;
        return *this;
    }

    POINT operator-(const POINT& rhs) const {
        return POINT(this->x - rhs.x, this->y - rhs.y);
    }

    POINT& operator-=(const POINT& rhs) {
        this->x -= rhs.x;
        this->y -= rhs.y;
        return *this;
    }

    bool operator==(const POINT& rhs) const {
        return abs(this->x - rhs.x) < G_EP && abs(this->y - rhs.y) < G_EP;
    }

    bool operator!=(const POINT& rhs) const {
        return abs(this->x - rhs.x) > G_EP || abs(this->y - rhs.y) > G_EP;
    }

    bool operator<(const POINT& rhs) const {
        if (abs(this->x - rhs.x) < G_EP) {
            return this->y < rhs.y;
        }
        return this->x < rhs.x;
    }

    POINT operator-() const {
        return {-x, -y};
    }

    static POINT fromDegree(double angle) {
        POINT unitVector;
        unitVector.x = cos(angle / 180 * M_PI);
        unitVector.y = sin(angle / 180 * M_PI);
        return unitVector;
    }

    double getAngle() {
        double angle = atan2(y, x) * 180.0 /
                       M_PI; // atan2函数返回从正x轴逆时针旋转到点(x,y)处的角度
        if (angle < 0) {
            angle += 360.0; // 如果角度小于0，加上360度
        }
        return angle;
    }
};

const POINT INVALID_POINT = POINT(G_INF, G_INF);

/**
* @brief 向量
*
*/
typedef POINT VECTOR2;

/**
 * @brief 两个相邻向量，以及两个向量组成的是否凸点
 *
 *           /\ 凸点
 *          /  \
 *         /    \______
 *         |     凹点  |
 *         |___________|
 */
struct ADJACENT_VECTOR2 {
    VECTOR2 v1;  ///< 向量1
    VECTOR2 v2;  ///< 向量2
    bool convex; ///< 是否是凸点
    ADJACENT_VECTOR2(VECTOR2 v1, VECTOR2 v2, bool convex = false) {
        this->v1 = v1;
        this->v2 = v2;
        this->convex = convex;
    }
};

/**
 * @brief 多边形
 */
typedef std::vector<geometry::POINT> POLYGON;

enum class LINE_CHAIN_TYPE {
    TYPE_SEG = 0,
    TYPE_ARC,
};

/**
* @brief 方向
*
*/
enum class DIRECTION {
    CW = 0, // 顺时针 Clockwise
    CCW,    // 逆时针 Counterclockwise
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

static double calculateArcAngle(
    const POINT& p1,
    const POINT& p2,
    const POINT& op,
    bool clockwise) {
    double include_angle = sweepAngle(p1, p2, op);
    double mul = multiply(p1, p2, op);
    // 由于POINT存的是Qt坐标系，这里的y是反的。叉积为正代表顺时针。
    if (mul > 0) {
        if (clockwise) {
            return include_angle * M_PI / 180;
        } else {
            return (360 - include_angle) * M_PI / 180;
        }
    } else if (mul < 0) {
        if (clockwise) {
            return (360 - include_angle) * M_PI / 180;
        } else {
            return include_angle * M_PI / 180;
        }
    }
    return M_PI;
}

struct LINE_CHAIN {
    POINT _st;
    POINT _et;
    POINT _ct;
    double _r {};
    double _sta {};
    double _spa {};
    DIRECTION _d = DIRECTION::CW;
    LINE_CHAIN_TYPE _type;

    LINE_CHAIN() :
        _type(LINE_CHAIN_TYPE::TYPE_SEG) {
    }

    LINE_CHAIN(LINE_CHAIN* rhs) :
        _st(rhs->_st),
        _et(rhs->_et),
        _ct(rhs->_ct),
        _r(rhs->_r),
        _sta(rhs->_sta),
        _spa(rhs->_spa),
        _d(rhs->_d),
        _type(rhs->_type) {
    }

    LINE_CHAIN(POINT a, POINT b) :
        LINE_CHAIN(LINE_CHAIN_TYPE::TYPE_SEG) {
        _st = a;
        _et = b;
    }

    //    LINE_CHAIN(rededa::pcbcommon::BI_TrackSeg seg) :
    //        LINE_CHAIN(LINE_CHAIN_TYPE::TYPE_SEG) {
    //        _st = POINT(seg.getEndPoint());
    //        _et = POINT(seg.getEndPoint());
    //    }

    LINE_CHAIN& operator=(const LINE_CHAIN& rhs) {
        _st = rhs._st;
        _et = rhs._et;
        _ct = rhs._ct;
        _r = rhs._r;
        _sta = rhs._sta;
        _spa = rhs._spa;
        _d = rhs._d;
        _type = rhs._type;
        return *this;
    }

    bool operator==(const LINE_CHAIN& rhs) {
        return _st == rhs._st && _et == rhs._et && _ct == rhs._ct &&
               _r == rhs._r && _sta == rhs._sta && _spa == rhs._spa &&
               _d == rhs._d && _type == rhs._type;
    }

    bool operator<(const LINE_CHAIN& rhs) const {
        if (this->_st == rhs._st) {
            return this->_et < rhs._et;
        }
        return this->_st < rhs._st;
    }

    explicit LINE_CHAIN(LINE_CHAIN_TYPE type) :
        _type(type) {
    }

    virtual ~LINE_CHAIN() = default;

    void updateStartPoint(const POINT& point) {
        _st = point;
        if (_type == LINE_CHAIN_TYPE::TYPE_ARC) {
            recalculateAngle();
        }
    }

    void updateEndPoint(const POINT& point) {
        _et = point;
        if (_type == LINE_CHAIN_TYPE::TYPE_ARC) {
            recalculateAngle();
        }
    }

    [[nodiscard]] QRectF calcBorder() const;

    /**
     * @brief 将弧度制转为角度值
     * 输入弧度制，返回角度制
     */
    double radian2Degree(double radian) {
        return std::fmod(radian * 180 / M_PI, 360);
    }

    double p2pDistance(POINT p1, POINT p2) {
        return sqrt(
            (p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y));
    }

    /**
     * @brief 叉积
     * 
     * @param v1 
     * @param v2 
     * @return double 
     */
    double crossProduct(POINT v1, POINT v2) {
        return v1.x * v2.y - v1.y * v2.x;
    }

    /**
     * @brief 计算两个线段的叉积
     * 
     * @param l1 
     * @param l2 
     * @return true 
     * @return false 
     */
    bool onDifferentSide(LINE_CHAIN l1, LINE_CHAIN l2) {
        // l2.p2 - l2.p1
        POINT v1 {l2._et.x - l2._st.x, l2._et.y - l2._st.y};
        // this.p1 - l2.p1
        POINT v2 {l1._st.x - l2._st.x, l1._st.y - l2._st.y};
        // this.p1 - l2.p2
        POINT v3 {l1._st.x - l2._et.x, l1._st.y - l2._et.y};

        double cross1 = crossProduct(v1, v2);
        double cross2 = crossProduct(v1, v3);

        // 如果两个线段的两个端点分别位于另一个线段的两侧，那么它们的叉积乘积会小于零。这个条件就表示两个线段相交。
        return cross1 * cross2 < 0;
    }

    /**
     * @brief 判断两个线段是否相交
     * 
     * @param l1 
     * @return true 
     * @return false 
     */
    bool segmentsIntersect(LINE_CHAIN line) {
        return onDifferentSide(this, line) && onDifferentSide(line, this);
    }

    double dotProduct(const POINT& v1, const POINT& v2) {
        return v1.x * v2.x + v1.y * v2.y;
    }

    double lengthSquared(const POINT& v) {
        return v.x * v.x + v.y * v.y;
    }

    double distanceSquared(const POINT& p1, const POINT& p2) {
        POINT v {p1.x - p2.x, p1.y - p2.y};
        return lengthSquared(v);
    }

    double distanceToSegmentSquared(const POINT& p, const LINE_CHAIN& line) {
        POINT v {line._et.x - line._st.x, line._et.y - line._st.y};
        POINT w {p.x - line._st.x, p.y - line._st.y};
        double c1 = dotProduct(w, v);
        if (c1 <= 0) {
            return distanceSquared(p, line._st);
        }
        double c2 = lengthSquared(v);
        if (c2 <= c1) {
            return distanceSquared(p, line._et);
        }
        double b = c1 / c2;
        POINT pb {line._st.x + v.x * b, line._st.y + v.y * b};
        return distanceSquared(p, pb);
    }

    /**
     * @brief 计算不相交的情况下两个线段的最短距离
     * 
     * @param line 
     * @return double 
     */
    double distanceLine2Line(const LINE_CHAIN& line2) {
        double d1 = distanceToSegmentSquared(_st, line2);
        double d2 = distanceToSegmentSquared(_et, line2);
        double d3 = distanceToSegmentSquared(line2._st, this);
        double d4 = distanceToSegmentSquared(line2._et, this);
        return sqrt(std::min({d1, d2, d3, d4}));
    }

    /**
     * @brief 计算线段到圆弧的最短距离
     *
     * @param lineStart
     * @param lineEnd
     * @param circleCenter
     * @param radius
     * @param arcStart
     * @param arcEnd
     * @return
     */
    double lineToARCDistance(
        geometry::POINT lineStart,
        geometry::POINT lineEnd,
        geometry::POINT circleCenter,
        double radius,
        geometry::POINT arcStart,
        geometry::POINT arcEnd) {
        // 计算线段端点到圆心的距离
        double dist1 = p2pDistance(lineStart, circleCenter);
        double dist2 = p2pDistance(lineEnd, circleCenter);

        // 如果线段的两个端点都在圆内，那么线段到圆弧的距离为0
        if (dist1 < radius && dist2 < radius) {
            return 0;
        }

        // 计算线段端点到圆弧起点和终点的距离
        double dist3 = p2pDistance(lineStart, arcStart);
        double dist4 = p2pDistance(lineStart, arcEnd);
        double dist5 = p2pDistance(lineEnd, arcStart);
        double dist6 = p2pDistance(lineEnd, arcEnd);

        // 如果线段的一个端点在圆内，另一个端点在圆外，那么线段到圆弧的距离为线段在圆外的端点到圆弧起点和终点的最小距离
        if (dist1 < radius) {
            return std::min(dist5, dist6);
        }
        if (dist2 < radius) {
            return std::min(dist3, dist4);
        }

        // 如果线段的两个端点都在圆外，那么线段到圆弧的距离为线段的两个端点到圆弧起点和终点的最小距离
        return std::min({dist3, dist4, dist5, dist6});
    }

    /**
     * @brief 计算两个线段的最短距离
     * 
     * @param line 
     * @return double 
     */
    double calculateLine2LineShortestDistance(const LINE_CHAIN& line) {
        if (segmentsIntersect(line)) {
            return -1;
        } else {
            return distanceLine2Line(line);
        }
    }

    /**
     * @brief 计算点到圆弧的最短距离
     * 
     * @param point 
     * @return double 
     */
    double claculatePoint2ArcShortestDistance(const POINT& point) {
        // 计算点到圆心的连线与 x 轴的夹角
        double p2CtRadian = std::atan2(point.y - _ct.y, point.x - _ct.x);
        double p2CtDegree = radian2Degree(p2CtRadian);

        double staDegree = radian2Degree(_sta);
        double endDegree = radian2Degree(_spa) + staDegree;

        double p2CtDistance = p2pDistance(point, _ct);

        if (staDegree <= p2CtDegree && p2CtDegree <= endDegree) {
            return p2CtDistance <= _r ? _r - p2CtDistance : p2CtDistance - _r;
        } else {
            double p2stDistance = p2pDistance(point, _st);
            double p2etDistance = p2pDistance(point, _et);
            return p2stDistance < p2etDistance ? p2stDistance : p2etDistance;
        }
    }

    /**
     * @brief 计算点到直线的最短距离
     * 
     * @param point 
     * @return double 
     */
    double calculatePoint2LineShortestDistance(const POINT& point) {
        double cross = (_et.x - _st.x) * (point.x - _st.x) +
                       (_et.y - _st.y) * (point.y - _st.y);
        if (cross <= 0)
            return sqrt(
                (point.x - _st.x) * (point.x - _st.x) +
                (point.y - _st.y) * (point.y - _st.y));

        double d2 = (_et.x - _st.x) * (_et.x - _st.x) +
                    (_et.y - _st.y) * (_et.y - _st.y);
        if (cross >= d2)
            return sqrt(
                (point.x - _et.x) * (point.x - _et.x) +
                (point.y - _et.y) * (point.y - _et.y));

        double r = cross / d2;
        double px = _st.x + (_et.x - _st.x) * r;
        double py = _st.y + (_et.y - _st.y) * r;
        return sqrt(
            (point.x - px) * (point.x - px) + (point.y - py) * (point.y - py));
    }

    /**
     * @brief 计算点到 LINE_CHAIN 的最短距离 
     */
    double calculateShortestDistance(const POINT& point) {
        // 点到圆弧的最短距离
        if (_type == LINE_CHAIN_TYPE::TYPE_ARC) {
            return claculatePoint2ArcShortestDistance(point);
            // 点到线的最短距离
        } else if (_type == LINE_CHAIN_TYPE::TYPE_SEG) {
            return calculatePoint2LineShortestDistance(point);
        }
        return -1;
    }

    /**
     * @brief 根据已有的 start point, center point, end point, start angle 来重新计算span angle
     * @param _st 弧线的起始点
     * @param _ct 弧线的圆心
     * @param _sta 起始点的角度
     * @details 最终目标是计算span angle， 就是弧从起始点扫到终止点扫了多少角度
    */
    void recalculateAngle() {
        auto p = _st - _ct;
        // 如果直接计算，是笛卡尔坐标系下的。这里要转成Qt坐标系。
        p = {p.x, -p.y};
        // atan2 只能表示 -pi/2 ~ pi/2 的范围i
        _sta = std::atan2(p.y, p.x);
        if (_sta < 0) {
            _sta += 2 * M_PI;
        }
        // 360度要变成0度
        if (std::abs(_sta - 2 * M_PI) < G_EP) {
            _sta = 0;
        }
        // 然后在转为弧度
        // _spa = sweepAngle(_et, _st, _ct) / 180 * M_PI;
        bool cw = _d == DIRECTION::CW;
        _spa = calculateArcAngle(
            {_st.x, _st.y},
            {_et.x, _et.y},
            {_ct.x, _ct.y},
            cw);
    }

    static LINE_CHAIN getDefault() {
        LINE_CHAIN lc(LINE_CHAIN_TYPE::TYPE_SEG);
        lc._st = lc._et = POINT(G_INF, G_INF);
        return lc;
    }

    operator bool() const {
        return !(
            this->_st == getDefault()._st && this->_et == getDefault()._et);
    }

    //    QDebug operator<<(QDebug debug) const {
    //        if (_type == LINE_CHAIN_TYPE::TYPE_SEG) {
    //            debug << "type: seg"
    //                  << " st: (" << _st.x << "," << _st.y << ")"
    //                  << " et: (" << _et.x << "," << _et.y << ")";
    //        } else {
    //            debug << "type: arc,"
    //                  << " st: (" << _st.x << "," << _st.y << ")"
    //                  << " et: (" << _et.x << "," << _et.y << ")"
    //                  << " ct: (" << _ct.x << "," << _ct.y << ")"
    //                  << " sta: " << _sta * 180 / M_PI
    //                  << " spa: " << _spa * 180 / M_PI
    //                  << " direction: " << (_d == DIRECTION::CCW ? "CCW" : "CW");
    //        }
    //        return debug;
    //    }

    //    std::ostream& operator<<(std::ostream& os) const {
    //        os << "type: " << (int)_type << " st: (" << _st.x << "," << _st.y << ")"
    //           << " et: (" << _et.x << "," << _et.y << ")"
    //           << " ct: (" << _ct.x << "," << _ct.y << ")"
    //           << " sta: " << _sta * 180 / M_PI << " spa: " << _spa * 180 / M_PI
    //           << " direction: " << (int)_d << std::endl;
    //        return os;
    //    }
};
typedef LINE_CHAIN LINE_SEG;

/**
 * @brief 直线
 *        直线一般式: ax + by + cz = 0
 */
struct LINE {
    double a;
    double b;
    double c;

    LINE(double d1 = 1, double d2 = -1, double d3 = 0) {
        a = d1;
        b = d2;
        c = d3;
    }

    LINE(POINT s, POINT e) {
        a = s.y - e.y;
        b = e.x - s.x;
        c = s.x * e.y - e.x * s.y;
    }

    /***
    * @brief 给定一条线段，返回一条直线
   */
    static LINE fromLineSeg(LINE_SEG seg) {
        return {
            seg._st.y - seg._et.y,
            seg._et.x - seg._st.x,
            seg._st.x * seg._et.y - seg._et.x * seg._st.y};
    }
    /**
    * @brief 给定一个x，返回直线上的横坐标为x的点
   */
    POINT getPoint(double x) const {
        if (b) {
            return POINT(x, (-a * x - c) / b);
        } else {
            if (abs(x + c / a) < G_EP) {
                return {x, 0};
            } else {
                return {G_INF, G_INF};
            }
        }
    }
    /**
    * @brief 获取直线上一条线段
   */
    LINE_SEG getALineSeg() const {
        if (b) {
            return LINE_SEG(getPoint(1), getPoint(2));
        } else {
            return LINE_SEG({-c / a, 0}, {-c / a, 1});
        }
    }
};

/**
* @brief 圆弧
*
*/
struct ARC : public LINE_CHAIN {
    ARC() :
        LINE_CHAIN(LINE_CHAIN_TYPE::TYPE_ARC) {
    }
    ARC(POINT c,
        double r,
        POINT st,
        POINT et,
        double sta,
        double spa,
        DIRECTION d) :
        LINE_CHAIN(LINE_CHAIN_TYPE::TYPE_ARC) {
        _ct = c;
        _st = st;
        _et = et;
        _r = r;
        _sta = sta;
        _spa = spa;
        _d = d;
    }
    //    explicit ARC(const rededa::Arc& arc) :
    //        LINE_CHAIN(LINE_CHAIN_TYPE::TYPE_ARC) {
    //        _d = arc.direction == rededa::RotateDirection::CW ? DIRECTION::CW :
    //                                                            DIRECTION::CCW;
    //        _ct = arc.center.toPxQPointF();
    //        _st = arc.startPoint.toPxQPointF();
    //        _et = arc.endPoint.toPxQPointF();
    //        _r = arc.radius.toPx();
    //        _sta = arc.startAngle.toDeg();
    //        _spa = arc.spanAngle.toDeg();
    //    }
};

/**
* @brief 圆
*/
struct CIRCLE {
    POINT c;  // 圆心
    double r; // 半径

    CIRCLE(POINT c, double r) :
        c(c),
        r(r) {
    }
};

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
 * @brief 给定一个要旋转的点p和一个固定点fixed以及一个角度angle，返回p绕fixed逆时针旋转angle角度后的点
 *        TODO: 后面需要从 POINT 抽离，并且传入引用以加快速度
 * @param p 要旋转的点
 * @param fixed 固定点
 * @param angle 旋转角度（以度为单位）
 * @return POINT 旋转后的点
 */
static POINT rotatePoint(const POINT& p, const POINT& fixed, double angle) {
    POINT fixedToP = p - fixed;
    auto mod = g_module(p, fixed);
    auto angleAfterRotation = fixedToP.getAngle() + angle;
    POINT unitDir = POINT::fromDegree(angleAfterRotation);
    return fixed + unitDir * mod;

    /* 方案二 */
    // double x = p.x;          // 将p的x坐标保存到变量x中
    // double y = p.y;          // 将p的y坐标保存到变量y中
    // double dx = fixed.x - x; // 计算dx,即fixed点的x坐标与p点的x坐标之差
    // double dy = fixed.y - y; // 计算dy,即fixed点的y坐标与p点的y坐标之差
    // double newX = dx * cos(degree2Radian(angle))
    //               - dy * sin(degree2Radian(angle)); // 根据公式计算新的x坐标
    // double newY = dx * sin(degree2Radian(angle))
    //               + dy * cos(degree2Radian(angle)); // 根据公式计算新的y坐标
    // return { newX, newY };                          // 返回新的点
}
/**
* @brief 矩形
*/
struct RECTANGLE {
    double width;       // 和水平面同方向的叫做长
    double height;      // 宽
    POINT center;       // 中心
    double rotateAngle; // 旋转角度(角度制)

    RECTANGLE(double w, double h, POINT p, double angle) :
        width(w),
        height(h),
        center(p),
        rotateAngle(angle) {
    }

    /**
    * @brief 获取4个顶点
   */
    POLYGON getFourVertex() const {
        /*
           B ------- A
             |      |
           C ------- D
        */
        std::vector<POINT> points(4);
        POINT horizontalA = POINT(center.x + width / 2, center.y + height / 2);
        POINT horizontalB = POINT(center.x - width / 2, center.y + height / 2);
        POINT horizontalC = POINT(center.x - width / 2, center.y - height / 2);
        POINT horizontalD = POINT(center.x + width / 2, center.y - height / 2);

        points[0] = rotatePoint(horizontalA, center, rotateAngle);
        points[1] = rotatePoint(horizontalB, center, rotateAngle);
        points[2] = rotatePoint(horizontalC, center, rotateAngle);
        points[3] = rotatePoint(horizontalD, center, rotateAngle);

        return std::move(points);
    }

    /**
    * @brief 获取四条边
   */
    std::vector<LINE_SEG> getFourEdge() const {
        auto points = getFourVertex();
        std::vector<LINE_SEG> segs(4);
        segs[0] = LINE_SEG(points[0], points[1]);
        segs[1] = LINE_SEG(points[1], points[2]);
        segs[2] = LINE_SEG(points[2], points[3]);
        segs[3] = LINE_SEG(points[3], points[0]);
        return std::move(segs);
    }
};

/**
* @brief 八边形，不建议使用了，建议使用N_POLYGON
*/
struct OCTAGON {
    POINT center;      // 中心
    double sideLength; // 边长
    double rotation;   // 旋转角度

    OCTAGON(POINT c, double l, double rot) :
        center(c),
        sideLength(l),
        rotation(rot) {
    }

    /**
     * @brief 获取外切圆半径
    */
    double getOuterCircumradius() {
        return sideLength / 2 / cos(67.5 / 180 * M_PI);
    }

    /**
     * @brief 获取内切圆半径
    */
    double getInnerCircumradius() {
        return tan(67.5 / 180 * M_PI) * sideLength / 2;
    }

    /**
     * @brief 获取8个顶点
    */
    POLYGON getEightVertex() {
        std::vector<POINT> ret(8);
        double len = getOuterCircumradius();
        double deg = 22.5 + rotation;
        for (int i = 0; i < 8; i++) {
            ret[i] = center + POINT::fromDegree(deg) * len;
            deg = deg + 45;
        }
        return ret;
    }

    /**
     * @brief 获取8条边
    */
    std::vector<LINE_SEG> getEightEdge() {
        auto points = getEightVertex();
        std::vector<LINE_SEG> ret(8);
        for (int i = 1; i < 8; i++) {
            ret[i - 1] = LINE_SEG(points[i - 1], points[i]);
        }
        ret[7] = LINE_SEG(points[7], points[0]);
        return ret;
    }
};

// 正多边形
struct N_POLYGON {
    POINT center;      // 中心
    double sideLength; // n条边每条边的边长
    double rotation;   // 旋转角度
    int n;             // 边数，n边形

    N_POLYGON(POINT c, double l, double rot, int nLines) :
        center(c),
        sideLength(l),
        rotation(rot),
        n(nLines) {
    }

    /**
    * @brief 获取外切圆半径
   */
    double getOuterCircumradius() {
        double angle = 90.0 - 0.5 * (360.0 / n);
        return sideLength / 2 / cos(angle / 180 * M_PI);
    }

    /**
    * @brief 获取内切圆半径
   */
    double getInnerCircumradius() {
        double angle = 90.0 - 0.5 * (360.0 / n);
        return tan(angle / 180 * M_PI) * sideLength / 2;
    }

    /**
    * @brief 获取N个顶点
   */
    POLYGON getNVertex() {
        std::vector<POINT> ret(n);
        double angle = 360.0 / n;
        double len = getOuterCircumradius();
        geometry::POINT onePontWhenZeroRotate {
            center.x + 0.5 * sideLength,
            center.y - getInnerCircumradius()};
        //        double deg = rotation + getAngleWithXAxis(center, onePontWhenZeroRotate);
        auto actual = onePontWhenZeroRotate - center;
        double anglee = atan2(actual.y, actual.x) * 180 / M_PI;
        if (anglee < 0) {
            anglee += 360;
        }
        double deg = rotation + anglee;
        for (int i = 0; i < n; i++) {
            ret[i] = center + POINT::fromDegree(deg) * len;
            deg = deg + angle;
        }
        return ret;
    }

    /**
    * @brief 获取8条边
   */
    std::vector<LINE_SEG> getNEdge() {
        auto points = getNVertex();
        std::vector<LINE_SEG> ret(n);
        for (int i = 1; i < n; i++) {
            ret[i - 1] = LINE_SEG(points[i - 1], points[i]);
        }
        ret[n - 1] = LINE_SEG(points[n - 1], points[0]);
        return ret;
    }
};

/**
* @brief 椭圆形
*        这里仅仅针对标准的椭圆形，即两端是半圆，直径就是宽度
*/
struct OVAL {
    POINT center;    // 中心
    double length;   // 长度
    double width;    // 宽度
    double rotation; // 旋转角度

    /**
     * @brief 获取椭圆的矩形
     * @return RECTANGLE 矩形
     */
    RECTANGLE getRect() {
        return RECTANGLE(length - width, width, center, rotation);
    }

    /**
     * @brief 获取两个半圆的圆心
     * @return std::vector<POINT> 两个圆心
     */
    std::vector<POINT> getTwoSemiCircleCenter() {
        std::vector<POINT> ret(2);
        VECTOR2 dir = POINT::fromDegree(rotation);
        double rectLen = length - width;
        ret[0] = center + dir * 0.5 * rectLen;
        ret[1] = center - dir * 0.5 * rectLen;
        return std::move(ret);
    }

    /**
     * @brief 获取椭圆的顶点和底点
     * @return std::vector<POINT> 顶点和底点
     */
    std::vector<POINT> getTwoEndPoints() {
        std::vector<POINT> ret(2);
        VECTOR2 dir = POINT::fromDegree(rotation);
        ret[0] = center + dir * 0.5 * length;
        ret[1] = center - dir * 0.5 * length;
        return std::move(ret);
    }

    /**
     * @brief 获取椭圆的两条线段
     * @return std::vector<LINE_SEG> 椭圆的两条线段
     */
    std::vector<LINE_SEG> getTwoLineSeg() {
        std::vector<LINE_SEG> ret(2);
        auto centers = getTwoSemiCircleCenter();
        auto dist = width / 2;
        auto dir1 = POINT::fromDegree(rotation + 90);
        ret[0] = LINE_SEG(centers[0] + dir1 * dist, centers[1] + dir1 * dist);
        auto dir2 = POINT::fromDegree(rotation - 90);
        ret[1] = LINE_SEG(centers[0] + dir2 * dist, centers[1] + dir2 * dist);
        return ret;
    }

    /* 获取椭圆的两条arc */
};

/**
 * @brief 图的边
 * 
 */
struct Edge {
    int from;
    int to;
};

// 点在多边形中的位置
enum POINT_POLY_RELATION {
    ON_ARC_MID, // 在圆弧中，但不是起点，也不是终点，是中间的点
    ON_ARC_START,    // 是圆弧起点
    ON_ARC_END,      // 是圆弧终点
    BETWEEN_TWO_ARC, // 是前面圆弧的终点，后面圆弧的起点
    NOT_ON_ARC,      // 不在圆弧上，就是线段的点
    NOT_ON_POLY      // 不在多边形上
};

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
* @brief 返回两点之间欧氏距离
*        d = √((x2 - x1)² + (y2 - y1)²)
* @param p1 点1
* @param p2 点2
* @return double 两点之间欧氏距离
*/
template<typename T>
double dist(T v1, T v2) {
    return (
        sqrt((v1.x - v2.x) * (v1.x - v2.x) + (v1.y - v2.y) * (v1.y - v2.y)));
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
        multiply(v._st, u._et, u._st) * multiply(u._et, v._et, u._st) >=
            0 && // 跨立实验
        multiply(u._st, v._et, v._st) * multiply(v._et, u._et, v._st) >= 0);
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
    bool res = intersect(LINE::fromLineSeg(u), LINE::fromLineSeg(v), l);
    if (res) {
        if (pointIsOnLineSeg(l, u) && pointIsOnLineSeg(l, v)) {
            return true;
        }
    }
    return false;
}
/**
* @brief 计算笛卡尔坐标系下，start->end向量从x轴正方向 逆时针旋转的夹角
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
    return dotmultiply(p, l._et, l._st) /
           (dist(l._st, l._et) * dist(l._st, l._et));
}

/**
* @brief 求点到线段所在直线垂线的垂足
*
* @param p 点
* @param l 线段
* @return POINT 点到 线段所在直线 垂线的垂足
*/
static POINT perpendicular(POINT p, LINE_SEG l) {
    double r = relation(p, l);
    POINT tp;
    tp.x = l._st.x + r * (l._et.x - l._st.x);
    tp.y = l._st.y + r * (l._et.y - l._st.y);
    return tp;
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
std::vector<T>
    smoothAngle(const T& st, const T& e1, const T& e2, double r, int segNum) {
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
    POINT normalRay =
        POINT(normalSE1.x + normalSE2.x, normalSE1.y + normalSE2.y);

    double sa = sweepAngle(e1, e2, st);
    if (sa <= 0.0 || sa > 175) {
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
} // namespace geometry

#endif // GEOMETRY_ALGO_CORE_H
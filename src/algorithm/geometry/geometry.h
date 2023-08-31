#include <algorithm>
#include <cmath>
#include <iostream>
#include <numeric>
#include <set>
#include <unordered_set>
#include <vector>

namespace geometry {

/**********************************常量定义************************************/

const double G_INF = 1E200;        // 极大值 infinity
const double G_EP = 1E-10;         // 极小值 epsilon
const double G_EP_REVERT_DOTS = 1; // 经过测试得到的合适的逆转线段到圆弧的阈值
const int G_MAX_L = 300;           // 最大长度
const double G_PI = 3.14159265;    // 圆周率

/******************************************************************************/

/*********************************基本几何结构**********************************/

/*
    TODO: 为了保证本算法库的简洁性和纯粹性，后续将会去除基础结构中定义的函数,
          同时非标准库中的其他类型也将会被移除
*/

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
    } // constructor

    POINT operator+(const POINT& rhs) const {
        return {this->x + rhs.x, this->y + rhs.y};
    }

    POINT operator-(const POINT& rhs) const {
        return {this->x - rhs.x, this->y - rhs.y};
    }

    POINT operator*(double multiplier) const {
        return {this->x * multiplier, this->y * multiplier};
    }

    POINT operator/(double multiplier) const {
        return {this->x / multiplier, this->y / multiplier};
    }

    POINT& operator+=(const POINT& rhs) {
        this->x += rhs.x;
        this->y += rhs.y;
        return *this;
    }

    POINT& operator-=(const POINT& rhs) {
        this->x -= rhs.x;
        this->y -= rhs.y;
        return *this;
    }

    POINT& operator*=(double multiplier) {
        this->x *= multiplier;
        this->y *= multiplier;
        return *this;
    }

    POINT& operator/=(double multiplier) {
        this->x /= multiplier;
        this->y /= multiplier;
        return *this;
    }

    bool operator==(const POINT& rhs) const {
        return std::abs(this->x - rhs.x) < G_EP && std::abs(this->y - rhs.y) < G_EP;
    }

    bool operator!=(const POINT& rhs) const {
        return std::abs(this->x - rhs.x) > G_EP || std::abs(this->y - rhs.y) > G_EP;
    }

    bool operator<(const POINT& other) const {
        if (x != other.x) {
            return x < other.x;
        }
        return y < other.y;
    }
};

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

    explicit LINE_CHAIN(LINE_CHAIN_TYPE type) :
        _type(type) {
    }
    virtual ~LINE_CHAIN() = default;
};

/**
 * @brief 线段
 *
 */
struct LINE_SEG : public LINE_CHAIN {
    LINE_SEG(POINT a, POINT b) :
        LINE_CHAIN(LINE_CHAIN_TYPE::TYPE_SEG) {
        _st = a;
        _et = b;
    }

    LINE_SEG() :
        LINE_CHAIN(LINE_CHAIN_TYPE::TYPE_SEG) {
    }
};

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
};

/**
 * @brief 圆弧
 *
 */
struct ARC : public LINE_CHAIN {
    ARC() :
        LINE_CHAIN(LINE_CHAIN_TYPE::TYPE_ARC) {
    }
    ARC(POINT c, double r, POINT st, POINT et, double sta, double spa, DIRECTION d) :
        LINE_CHAIN(LINE_CHAIN_TYPE::TYPE_ARC) {
        _ct = c;
        _st = st;
        _et = et;
        _r = r;
        _sta = sta;
        _spa = spa;
        _d = d;
    }
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
};

/**
 * @brief 正多边形
 *
 */
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
};

/**
 * @brief 图的边
 *
 */
struct Edge {
    int from;
    int to;
};
/******************************************************************************/

} // namespace geometry
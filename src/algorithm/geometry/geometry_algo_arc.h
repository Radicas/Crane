/**
 * @file geometry_algo_arc.h
 * @author Radica
 * @brief 弧线算法
 * @version 0.1
 * @date 2023-09-17
 * 
 * @copyright Copyright (c) 2023 上海弘快科技有限公司
 * 
 */
#ifndef GEOMETRY_ALGO_ARC_H
#define GEOMETRY_ALGO_ARC_H

#include "geometry_algo_core.h"
#include <vector>

namespace geometry {

/**
 * @brief 角度转弧度
 * 
 * @param angle 角度
 * @return double 弧度
 */
double degree2Radian(double angle);

/**
 * @brief 弧度转角度
 * 
 * @param angle 弧度
 * @return double 角度
 */
double radian2degree(double angle);

/**
 * @brief 求点P到圆弧的最近点
 * @param p 点P
 * @param arc 圆弧
 * @param np 最近点
 * @return double 最近点到P的距离
 */
double closestPointToArc(POINT p, ARC arc, POINT& np);

/**
 * @brief 判断点是否在圆内（包括边界）
 *        因为圆为凸集，所以判断点集、折线、多边形是否在圆内时，
 *        只需要逐一判断点是否在圆内即可。
 * @param o 目标点
 * @param r 半径
 * @param p 圆心
 * @return true 如果点在圆内（包括边界）
 * @return false 如果点不在圆内
 */
bool pincircle(geometry::POINT o, double r, geometry::POINT p);

// /**
//  * @brief 判断两个圆的关系
//  * @param p1 圆1的圆心
//  * @param r1 圆1的半径
//  * @param p2 圆2的圆心
//  * @param r2 圆2的半径
//  * @return 1 两个圆相离
//  * @return 2 圆2在圆1外切
//  * @return 3 两个圆相交
//  * @return 4 圆2在圆1内切
//  * @return 5 圆2在圆1内含
//  */
// int circlerelation(
//     geometry::POINT p1,
//     double r1,
//     geometry::POINT p2,
//     double r2);

/**
 * @brief 根据三个点确定外接圆
 * @param p1 第一个点
 * @param p2 第二个点
 * @param p3 第三个点
 * @param r 外接圆的半径
 * @return geometry::POINT 外接圆的圆心
 */
geometry::POINT trip2circle(
    geometry::POINT p1,
    geometry::POINT p2,
    geometry::POINT p3,
    double& r);

/**
 * @brief 根据圆生成外接矩形
 * 
 * @param c 圆心坐标
 * @param r 圆的半径
 * @return QRectF 外接矩形
 */
QRectF circle2QRectF(geometry::POINT c, double r);

/**
 * @brief 判断点是否在圆上
 * @param c 圆
 * @param p 点
 * @return bool 如果点在圆上返回 true，否则返回 false
 */
bool pointIsOnCircle(CIRCLE c, POINT p);

/**
 * @brief 给定一个圆与圆外一点，返回经过该点的两条切线的切点
 * @param c 圆
 * @param p 圆外一点
 * @return std::vector<POINT> 经过点 p 的两条切线的切点
 */
std::vector<POINT> tangentPoints(CIRCLE c, POINT p);

/**
 * @brief 计算直线与圆的交点
 * @param line 直线
 * @param circle 圆
 * @return std::vector<POINT> 直线与圆的交点
 */
std::vector<POINT> getIntersection(const LINE& line, const CIRCLE& circle);

double squaredDistance(const POINT& p1, const POINT& p2);

/**
 * @brief 获取两条直线的交点
 * 
 * @param l1 直线1
 * @param l2 直线2
 * @return POINT 交点
 */
POINT getIntersection(const LINE& l1, const LINE& l2);

/**
 * @brief 获取两条线段的交点
 * @return POINT 不存在或平行的话，返回 POINT(G_INF, G_INF)
*/
POINT getIntersection(const LINE_SEG& seg1, const LINE_SEG& seg2);

/**
 * @brief 计算圆内点 a 和圆外点 b 所形成的线段与圆的交点
 * @param lineSeg 线段
 * @param circle 圆
 * @return std::vector<POINT> 线段与圆的交点
 */
std::vector<POINT> getIntersection(
    const LINE_SEG& lineSeg,
    const CIRCLE& circle);

/**
 * @brief 求线段和多边形的交点
 * @param lineSeg 线段
 * @param polygon 多边形
 * @return std::vector<POINT> 线段和多边形的交点
*/
std::vector<POINT> getIntersection(
    const LINE_SEG& lineSeg,
    const POLYGON& polygon);

/**
 * @brief 求线段和直线的交点
 * @param lineSeg 线段
 * @param line 直线
 * @return std::vector<POINT> 线段和直线的交点
 */
std::vector<POINT> getIntersection(const LINE_SEG& lineSeg, const LINE& line);

/**
 * @brief 求直线和矩形的交点
 * @param line 直线
 * @param rect 矩形
 * @return std::vector<POINT> 直线和矩形的交点
 */
std::vector<POINT> getIntersection(const LINE& line, const RECTANGLE& rect);

/**
 * @brief 给定一个圆、一个方向向量和一个角度，求出圆心在这个方向向量上的射线上的圆外一点，这个点与圆形成的两条切线形成的角度正好是给定角度
 * @param circle 圆
 * @param dir 方向向量
 * @param angle 角度
 * @return POINT 圆外的点
 */
POINT getPointOutsideCircleWithDesiredAngle(
    const CIRCLE& circle,
    VECTOR2 dir,
    double angle);

/**
 * @brief 给定一个圆、一个方向向量和一个距离，求边缘朝这个方向移动这些距离+r后的点
 * @param circle 圆
 * @param dir 方向向量
 * @param dist 距离
 * @return POINT 移动后的点
 */
POINT getPointMoveFromEdge(const CIRCLE& circle, VECTOR2 dir, double dist);

/**
 * @brief 给定一个点和一个方向向量，一个距离，求这个点沿着这个方向向量移动这个距离之后的点
 * @param p 点
 * @param dir 方向向量
 * @param dist 距离
 * @return POINT 移动后的点
 */
POINT getPointMoveFromPoint(const POINT& p, VECTOR2 dir, double dist);

/**
 * @brief 判断一个点是否在圆内
 * @param circle 圆
 * @param p 点
 * @return bool 如果点在圆内，返回 true；否则，返回 false。
 */
bool pointIsInCircle(const CIRCLE& circle, POINT p);

/************************** TEMPLATE **************************/
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
std::vector<T> arc2points(
    double cx,
    double cy,
    double r,
    double startAngle,
    double endAngle,
    int numSegments) {
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
/***************************************************************/

} // namespace geometry

#endif // GEOMETRY_ALGO_ARC_H
/**
 * @file geometry_algo_polygon.h
 * @author Radica
 * @brief 多边形相关算法
 * @version 0.1
 * @date 2023-09-17
 * 
 * @copyright Copyright (c) 2023 上海弘快科技有限公司
 * 
 */
#ifndef GEOMETRY_ALGO_POLYGON_H
#define GEOMETRY_ALGO_POLYGON_H

#include "geometry_algo_core.h"
#include <vector>

namespace geometry {

extern const double CAPSULE_TO_DOTS_SEG_NUMS; // 操场形一端转成点数量

/**
 * @brief 判断多边形是否相交（线段法）
 * @tparam T 多边形点的类型
 * @param A 第一个多边形的点集合
 * @param B 第二个多边形的点集合
 * @return 是否相交（true表示相交，false表示不相交）
 */
bool polygonIntersect(const geometry::POLYGON& A, const geometry::POLYGON& B);

/**
 * @brief 多边形有向面积
 *        笛卡尔坐标系下，顺时针排序，结果为正
 *                      逆时针排序，结果为负
 *        Qt坐标系下，   顺时针排序，结果为负
 *                      逆时针排序，结果为正
 * @tparam T 
 * @param polygon 
 * @return double 
 */
double polygonArea(const std::vector<geometry::POINT>& polygon);

/**
 * @brief 多边形外扩/内缩
 * @details
 *          注意，传入的间距要大于等于0
 *          三个参数是由于历史遗留问题
 *          
 * @param polygon 多边形点集
 * @param gap 外扩/内缩间距
 * @param expand 是否外扩
 * @return true 外扩/内缩成功
 * @return false 外扩/内缩失败
 */
bool polygonOffset(geometry::POLYGON& polygon, double gap, bool expand);

/**
 * @brief 凸多边形外扩/内缩
 * @param polygon 凸多边形
 * @param gap 外扩/内缩间距
 * @param expand 是否外扩/内缩
 * @return true 外扩/内缩成功
 * @return false 外扩/内缩失败
 */
bool polygonOffsetByClipper2(
    geometry::POLYGON& polygon,
    double gap,
    bool expand = true);

/**
 * @brief 去重
 * @tparam T 点
 * @param polygon 多边形
 * @return std::vector<T> 去重后的多边形
 */
void deduplication(std::vector<geometry::POINT>& polygon);

/**
 * @brief 平滑一个多边形, 将多边形中符合条件的角导成圆弧
 * @param polygon 要倒角的多边形
 * @param radius 倒角的半径
*/
void polygonSmooth(std::vector<geometry::POINT>& polygon, double radius);

/**
 * @brief 将操场形（由两个圆心组成）转换为外轮廓，支持外扩或内缩操作
 * @param seg 草场形的两个圆心构成的线段
 * @param width 轮廓的宽度
 * @param partition 半圆分割段数
 * @param gap 偏移量
 * @param expand true 表示外扩，false 表示内缩
 * @note WARNING: 这个接口目前返回的不是落在外轮廓上的点
 *             临时增加一段 "圆弧外接圆到内接圆的差值"，以解决测量值偏小的问题
 * @return 外轮廓点集
 */
std::vector<geometry::POINT> capsule2outline(
    LINE_SEG seg,
    double width,
    int partition,
    double gap,
    bool expand);

/**
 * @brief 检查点是否在多边形内部
 * @tparam T 点类型
 * @param point 待检查的点
 * @param polygon 多边形点集
 * @return true 如果点在多边形内部
 * @return false 如果点在多边形外部或边界上
 */
bool ptInPolygon(
    const geometry::POINT& point,
    const std::vector<geometry::POINT>& polygon);

/**
 * @brief 检查多边形是否在多边形内部
 * @tparam T
 * @param innerPolygon
 * @param outerPolygon
 * @return
 */
bool polygonInsidePolygon(
    const std::vector<geometry::POINT>& innerPolygon,
    const std::vector<geometry::POINT>& outerPolygon);

/**
 * @brief 检查点是否在多边形边界上
 * @tparam T 点类型
 * @param point 待检查的点
 * @param polygon 多边形点集
 * @return true 如果点在多边形边界上
 * @return false 如果点在多边形内部或外部
 */
bool ptOnPolygon(
    const geometry::POINT& point,
    const std::vector<geometry::POINT>& polygon);

/**
 * @brief p1和p2是否在polygon的同一条边上
 */
bool ptOnSameSide(
    const geometry::POINT& p1,
    const geometry::POINT& p2,
    const std::vector<geometry::POINT>& polygon);

/**
 * 计算多边形和给定线段的交点
 * @param polygon 多边形点向量
 * @param start 起点坐标
 * @param end 终点坐标
 * @return 一个点向量，表示多边形和线段的交点
 */
std::vector<geometry::POINT> getPolygonSegmentIntersections(
    const std::vector<geometry::POINT>& polygon,
    const geometry::POINT& start,
    const geometry::POINT& end);

std::vector<geometry::POINT> deduplication2(
    const std::vector<geometry::POINT>& polygon);

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
geometry::POINT nPolygonInscribedDistance(int n, geometry::POINT radius);

/**
 * @brief 计算多边形重心
 * 
 * @tparam T 
 * @param points 
 * @return T 
 */
geometry::POINT computeCentroid(const std::vector<geometry::POINT>& points);

/**
 * @brief 纠正多边形，使点集按逆时针序排列
 * 
 * @tparam T 
 * @param points 
 * @return std::vector<T> 
 */
std::vector<geometry::POINT> sortPointsInCounterClockwise(
    std::vector<geometry::POINT>& points);

/**
 * @brief 求两个多边形是否相交
*/
bool isPolyPolyIntersect(POLYGON p1, POLYGON p2);

/**
 * @brief 移动一个多边形
*/
POLYGON movePolygonBy(const POLYGON& plg, const POINT& dir);

/**
 * @brief 判断一个多边形是否是凸多边形
 * @param polygon
 * @return
 */
bool isConcavePolygon(const POLYGON& polygon);

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
    const geometry::POINT& C);

/**
* @brief 返回两点的中点
*
* @tparam T 点
* @param A 点A
* @param B 点B
* @return T 中心点
*/
geometry::POINT mid(const geometry::POINT& A, const geometry::POINT& B);

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
    const geometry::POINT& p3);

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
    const geometry::POINT& targetPoint);

/**
 * @brief 计算极角
 * 
 * @tparam V 
 * @tparam T 
 * @param p 
 * @param centroid 
 * @return V 
 */
double computePolarAngle(const geometry::POINT& p1, const geometry::POINT& p2);
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
double ptolinesegdist(const POINT& p, const LINE_SEG& l, POINT& np);

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
    const POINT& b);

/**
* @brief 判断点p是否在矩形r内
* @param p 点p
* @param r 矩形r
* @return true 在矩形内
* @return false 不在矩形内
*/
bool pointIsInRectangle(const POINT& p, const RECTANGLE& r);

/**
* @brief 判断点p是否在椭圆oct内
*/
bool pointIsInOctagon(POINT p, OCTAGON oct);

/**
* @brief 判断点p是否在椭圆oct内
*/
bool pointIsInPolygon(POINT p, POLYGON plg);

/**
* @brief 给定一个点和一组线段，求离这个点最近的线段
* @param p 给定的点
* @param segs 线段集合
* @return LINE_SEG 最近的线段
*/
LINE_SEG nearestLineSeg(const POINT& p, const std::vector<LINE_SEG>& segs);

/**
* @brief 通过两点创建直线
*        已知两个点A(x1, y1)和B(x2, y2)时，
*        直线的一般方程为：a * x + b * y + c = 0
*        其中，a = y2 - y1，b = x1 - x2，c = x2 * y1 - x1 * y2
* @param p1 第一个点
* @param p2 第二个点
* @return LINE 创建的直线
*/
LINE pt2line(const geometry::POINT& p1, const geometry::POINT& p2);

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
    double& angle);

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
    double dy);

/**
* @brief 将N边形转换为轮廓线的点集合
* @param nPlg N边形对象
* @param gap 轮廓线的偏移距离
* @param expand 是否向外扩展（true表示向外扩展，false表示向内收缩）
* @return 轮廓线的点集合
*/
std::vector<geometry::POINT>
    nPolygon2outline(N_POLYGON nPlg, double gap, bool expand);

/**
 * @brief 获取给定边集中离给定点最近的边
 * @param p 给定点
 * @param segs 给定边集
 * @return 给定边集中离给定点最近的边
*/
geometry::POINT getClosestPoint(geometry::POINT p, std::vector<LINE_SEG> segs);

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
    geometry::POINT& retP);

// 大于0，b在a的逆时针方向
double crossProduct2D(const POINT& a, const POINT& b);

double dotProduct(const POINT& a, const POINT& b);

double length(const POINT& a);

// a在b上的投影
double Proj(const POINT& a, const POINT& b);

/**
 * @brief 在逆时针存储的点集中，给出连续的三个点，判断是否是凹点
 * @details
 *      
 * @param prevP
 * @param currP
 * @param nextP
 * @return
 */
bool isConcace(POINT prevP, POINT currP, POINT nextP);

bool isConvex(POINT prevP, POINT currP, POINT nextP);

bool isConcave(const POLYGON poly, int ind);

bool isConvex(const POLYGON poly, int ind);

void printPoints(std::vector<POINT>& polygon);

/**
 * @brief p1 p2 p3 是否是圆弧上连续三点
 * @param p1 第一个点
 * @param p2 中间的点
 * @param p3 最后一个点
 * @return p1p2p3 是否满足形成圆弧的条件
*/
bool isConsecutiveChords2(const POINT& p1, const POINT& p2, const POINT& p3);

/**
 * @brief ptInd 在 poly 上 是否是圆弧中【非起点非终点】 一点，定义圆弧至少要5个点
 * @param ptInd 要判断是否在圆弧中的点在poly中的下标
 * @param poly  传入的多边形
 * @return 在poly中下标为ptInd的点是否是圆弧中的一点
*/
bool isConsecutiveChords(int ptInd, const POLYGON& poly);

/**
 * @brief 判断poly某个下标的点和圆弧的关系
*/
POINT_POLY_RELATION ptPolyRelation(int ptInd, const POLYGON& poly);

void smoothPolygon2(std::vector<POINT>& polygon, double radius);

POLYGON getBoundingBox(const POLYGON& poly);

POLYGON getBoundingBox(const POINT& p1, const POINT& p2);

bool judgeCollinear(const POINT& p1, const POINT& p2, const POINT& p3);

/**
 * @brief 纠正多边形
*/
bool correctGPolygon(const POLYGON& poly, POLYGON& rv);

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
    geometry::DIRECTION direction);

} // namespace geometry

#endif // GEOMETRY_ALGO_POLYGON_H
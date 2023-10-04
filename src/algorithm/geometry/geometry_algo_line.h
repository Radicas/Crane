/**
 * @file geometry_algo_line.h
 * @author Radica
 * @brief 向量、线段、直线算法
 * @version 0.1
 * @date 2023-09-17
 * 
 * @copyright Copyright (c) 2023 上海弘快科技有限公司
 * 
 */
#ifndef GEOMETRY_ALGO_LINE_H
#define GEOMETRY_ALGO_LINE_H

#include "geometry_algo_core.h"
#include <vector>

namespace geometry {

/**
* @brief 计算两个二维向量的夹角，参数为两个二维向量的 x, y 分量
*
* @param p1 第一个二维向量
* @param p2 第二个二维向量
* @return double 夹角的度数
*/
double calcAngleBetweenTwoVectors(
    const geometry::POINT& p1,
    const geometry::POINT& p2);

/**
* @brief 求线段 AB 逆时针方向的单位向量
*
* @param A 点A
* @param B 点B
* @return geometry::VECTOR2 AB 逆时针方向的单位向量
*/
geometry::VECTOR2 verticalCCWNormalized(
    const geometry::POINT& A,
    const geometry::POINT& B);

/**
* @brief 将点相对(0,0)点顺时针旋转90°
*
* @param v 要旋转的点
* @return geometry::POINT 旋转后的点
*/
geometry::POINT rotateCW90Degree(const geometry::POINT& v);

/**
* @brief 将点相对(0,0)点逆时针旋转90°
* 
* @tparam 点的数据类型
* @param v 要旋转的点
* @return 旋转后的点
*/
geometry::POINT rotateCCW90Degree(const geometry::POINT& v);

/**
* @brief 求线段 AB 顺时针方向的单位向量
*
* @tparam T 线段类型
* @param A 点A
* @param B 点B
* @return T AB 顺时针方向的单位向量
*/
geometry::POINT verticalCWNormalized(
    const geometry::POINT& A,
    const geometry::POINT& B);

/**
* @brief 求线段 AB 逆时针方向的单位向量
*
* @param A 点A
* @param B 点B
* @return AB 逆时针方向的单位向量
*/
geometry::POINT verticalCCWNormalized(
    const geometry::POINT& A,
    const geometry::POINT& B);

} // namespace geometry

#endif // GEOMETRY_ALGO_LINE_H
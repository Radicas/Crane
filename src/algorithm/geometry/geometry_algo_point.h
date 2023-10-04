/**
 * @file geometry_algo_point.h
 * @author Radica
 * @brief 点相关的算法接口
 * @version 0.1
 * @date 2023-09-19
 * 
 * @copyright Copyright (c) 2023 上海弘快科技有限公司
 * 
 */
#ifndef GEOMETRY_ALGO_POINT_H
#define GEOMETRY_ALGO_POINT_H

#include "geometry_algo_core.h"
#include "geometry_algo_math.h"

namespace geometry {

/**
 * @brief 两个点是否过于接近，接近的标准为EP
 * 
 * @param p1 点1
 * @param p2 点2
 * @param EP 极小值
 * @return true 两个点过于接近
 * @return false 两个点不接近
 */
bool twoPointsIsMuchClose(
    const geometry::POINT& p1,
    const geometry::POINT& p2,
    double EP);

} // namespace geometry

#endif // GEOMETRY_ALGO_POINT_H
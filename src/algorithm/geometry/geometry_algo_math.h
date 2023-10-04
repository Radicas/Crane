/**
 * @file geometry_algo_math.h
 * @author Radica
 * @brief 一些数学计算相关的接口
 * @version 0.1
 * @date 2023-09-19
 * 
 * @copyright Copyright (c) 2023 上海弘快科技有限公司
 * 
 */
#ifndef GEOMETRY_ALGO_MATH_H
#define GEOMETRY_ALGO_MATH_H

#include <math.h>

namespace geometry {

/**
 * @brief 如果两个数值的差的绝对值小于1E-3，则返回true，否则返回false
 * 
 * @param num1 数值1
 * @param num2 数值2
 * @param EP 极小值
 * @return true 
 * @return false 
 */
bool lessThanEpsilon(double num1, double num2, double EP);

} // namespace geometry

#endif // GEOMETRY_ALGO_MATH_H
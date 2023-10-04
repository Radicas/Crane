#include "geometry_algo_line.h"

namespace geometry {

double calcAngleBetweenTwoVectors(
    const geometry::POINT& p1,
    const geometry::POINT& p2) {
    /* 计算点积 */
    double dot_product = p1.x * p2.x + p1.y * p2.y;
    /* 计算两个向量的模长 */
    double len_vec1 = std::hypot(p1.x, p1.y);
    double len_vec2 = std::hypot(p2.x, p2.y);
    /* 检查点积是否超出有效范围 */
    if (dot_product <= -len_vec1 * len_vec2) {
        return 180.0; // 夹角为180度
    } else if (dot_product >= len_vec1 * len_vec2) {
        return 0.0; // 夹角为0度
    }
    /* 计算夹角的余弦值 */
    double cos_angle = dot_product / (len_vec1 * len_vec2);
    /* 计算夹角的弧度值 */
    double angle = std::acos(cos_angle);
    /* 转换为角度值并返回结果 */
    return angle / M_PI * 180.0;
}

geometry::POINT rotateCW90Degree(const geometry::POINT& v) {
    return {v.y, -v.x};
}

geometry::POINT verticalCWNormalized(
    const geometry::POINT& A,
    const geometry::POINT& B) {
    return rotateCW90Degree(normalize(B - A));
}

geometry::POINT verticalCCWNormalized(
    const geometry::POINT& A,
    const geometry::POINT& B) {
    auto v = geometry::verticalCWNormalized(A, B);
    return {-v.x, -v.y};
}

geometry::POINT rotateCCW90Degree(const geometry::POINT& v) {
    return {-v.y, v.x};
}

} // namespace geometry

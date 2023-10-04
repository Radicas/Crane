#include <gtest/gtest.h>

#include "algorithm/geometry/geometry_algo_core.h"

// 数据源
geometry::POINT p1;
geometry::POINT p2(1, 2);
geometry::POINT p3(p2);

geometry::VECTOR2D v1(5, 5);
geometry::VECTOR2D v2(-5, -5);

// 构造函数
TEST(POINT, construction) {
    // defualt construction
    geometry::POINT test_p1;
    ASSERT_EQ(test_p1.x, 0);
    ASSERT_EQ(test_p1.y, 0);

    // user defined construction
    geometry::POINT test_p2(1, 2);
    ASSERT_EQ(test_p2.x, 1);
    ASSERT_EQ(test_p2.y, 2);

    // copy construction
    geometry::POINT test_p3(p2);
    ASSERT_EQ(test_p3.x, 1);
    ASSERT_EQ(test_p3.y, 2);
}

//  操作符重载
TEST(POINT, operators) {
    // +=
    p3 += p2;
    ASSERT_EQ(p3.x, 2);
    ASSERT_EQ(p3.y, 4);

    // -=
    p3 -= p2;
    ASSERT_EQ(p3.x, 1);
    ASSERT_EQ(p3.y, 2);

    // 取反
    p3 = -p3;
    ASSERT_EQ(p3.x, -1);
    ASSERT_EQ(p3.y, -2);

    // +
    auto p4 = p1 + p3;
    ASSERT_EQ(p4.x, -1);
    ASSERT_EQ(p4.y, -2);

    // -
    auto p5 = p1 - p3;
    ASSERT_EQ(p5.x, 1);
    ASSERT_EQ(p5.y, 2);

    // *
    auto p6 = p5 * 3;
    ASSERT_EQ(p6.x, 3);
    ASSERT_EQ(p6.y, 6);

    // /
    auto p7 = p6 / 3;
    ASSERT_EQ(p7.x, 1);
    ASSERT_EQ(p7.y, 2);
}

TEST(VECTOR2D, multiply) {
    double res = geometry::multiply(v1, v2);
    ASSERT_EQ(res, 0);
}

TEST(VECTOR2D, normalize) {
    auto res = geometry::normalize(v1);
    ASSERT_EQ(res.x, 5 / std::sqrt(50));
    ASSERT_EQ(res.y, 5 / std::sqrt(50));
}
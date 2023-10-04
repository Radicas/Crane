#ifndef TEST_GEOMETRY_ALGO_CORE_H
#define TEST_GEOMETRY_ALGO_CORE_H

#include <gtest/gtest.h>

#include "algorithm/geometry/geometry_algo_core.h"

// 定义一个测试套件(Test Suite)，用于测试点(Point)类的功能
class PointTest : public ::testing::Test {
protected:
    // 在每个测试用例之前执行的操作
    void SetUp() override {
        // 可以在这里进行一些初始化操作
    }

    // 在每个测试用例之后执行的操作
    void TearDown() override {
        // 可以在这里进行一些清理操作
    }

    // 可以在这里定义一些辅助函数，供测试用例使用
};

// 定义一个测试用例(Test Case)，测试点(Point)类的构造函数和成员函数
TEST_F(PointTest, TestConstructorAndMembers) {
    // 创建一个点对象
    geometry::POINT p(3.0, 4.0);

    // 使用断言检查点的坐标是否正确
    ASSERT_EQ(p.getX(), 3.0);
    ASSERT_EQ(p.getY(), 4.0);

    // 使用点对象的成员函数进行一些操作，并使用断言检查结果
    p.setX(5.0);
    p.setY(6.0);
    ASSERT_EQ(p.getX(), 5.0);
    ASSERT_EQ(p.getY(), 6.0);

    double distance = p.distanceToOrigin();
    ASSERT_DOUBLE_EQ(distance, 7.810249675906654);
}

#endif // TEST_GEOMETRY_ALGO_CORE_H

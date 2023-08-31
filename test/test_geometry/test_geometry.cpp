#include "geometry_algo.h"
#include <gtest/gtest.h>

TEST(geometry, sweepAngle) {
    double angle =
        geometry::sweep_angle(geometry::POINT(5.0, 0.0), geometry::POINT(0.0, 5.0), geometry::POINT(0.0, 0.0));
    ASSERT_NEAR(angle, 90.0, 1e-6);
}

TEST(geometry, calculateArcAngleCounterclockwise) {
    double angle1 = geometry::calculateArcAngle(geometry::POINT(5.0, 0.0), geometry::POINT(0.0, 5.0),
                                                geometry::POINT(0.0, 0.0), false);
    ASSERT_NEAR(angle1, 90.0, 1e-6);

    double angle2 = geometry::calculateArcAngle(geometry::POINT(5.0, 0.0), geometry::POINT(0.0, -5.0),
                                                geometry::POINT(0.0, 0.0), false);
    ASSERT_NEAR(angle2, 270.0, 1e-6);

    double angle3 = geometry::calculateArcAngle(geometry::POINT(0.0, -5.0), geometry::POINT(5.0, 0.0),
                                                geometry::POINT(0.0, 0.0), true);
    ASSERT_NEAR(angle3, 270.0, 1e-6);
}

TEST(geometry, calculateIntersection) {
    geometry::POINT p1     = { 3.0, 0.0 };
    geometry::POINT p2     = { 7.0, 0.0 };
    double          radius = 5.0;
    geometry::POINT center = { 0.0, 0.0 };
    auto            res    = geometry::segArcIntersectPoints(p1, p2, center, radius);
    ASSERT_NEAR(res[0].x, 5, 1e-6);
    ASSERT_NEAR(res[0].y, 0, 1e-6);
}
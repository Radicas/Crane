#include "geometry_algo_polygon.h"

namespace geometry {

bool polygonOffset(geometry::POLYGON& polygon, double gap, bool expand) {
    // // 多边形顶点数量
    // int vertexCnt = polygon.size();
    // // 拷贝一份
    // auto oldPolygon = polygon;
    // // 旧多边形清空
    // polygon.clear();

    // std::vector<geometry::ADJACENT_VECTOR2D> adjVectors {};
    // int vertexSize = vertexCnt - 1;
    // // 根据顶点生成向量数据
    // for (int i = 0; i < vertexCnt; ++i) {
    //     int prev = (i - 1 + vertexCnt) % vertexCnt;
    //     int next = (i + 1) % vertexCnt;

    //     geometry::VECTOR2D vp1 = oldPolygon[i] - oldPolygon[next];
    //     geometry::VECTOR2D vp2 = oldPolygon[i] - oldPolygon[prev];

    //     bool convex = multiply(vp1, vp2) > 0;
    //     adjVectors.emplace_back(vp1, vp2, convex);
    // }
    // // 产生新顶点
    // for (int i = 0; i < vertexCnt; ++i) {
    //     auto v1 = adjVectors.at(i)._v1;
    //     auto v2 = adjVectors.at(i)._v2;
    //     auto sin = std::abs(multiply(geometry::normalize(v1), geometry::normalize(v2)));
    //     // sin 等于 0 时，向量共线
    //     if (sin > 0) {
    //         // 凹点取反
    //         if (!adjVectors.at(i)._is_convex) {
    //             sin = -sin;
    //         }
    //         geometry::VECTOR2D offset = (gap / sin) * (normalize(v1) + normalize(v2));
    //         auto vec = geometry::POINT(oldPolygon[i].x, oldPolygon[i].y);
    //         // 内缩取反
    //         if (!expand) {
    //             offset = -offset;
    //         }
    //         polygon.emplace_back(vec + offset);
    //     }
    // }
    // int i = 1;
    // return true;
}

} // namespace geometry

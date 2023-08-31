#include "geometry_algo.hpp"
#include "qtlaboratory/labscene.h"

#include <list>
#include <stack>
#include <set>
#include <map>
#include <QLineF>
#include <QGraphicsLineItem>
#include <QPainterPath>
#include <QColor>

namespace geometry {

static int posMod(int value, int modulus) {
    return value < 0 ? value % modulus + modulus : value % modulus;
}

/**
 * @brief 判断多边形是否为顺时针方向的函数
 *        https://stackoverflow.com/questions/1165647/how-to-determine-if-a-list-of-polygon-points-are-in-clockwise-order
 *
 * @param polygon
 * @return
 */
bool isClockwise(const std::vector<geometry::POINT>& polygon) {
    double sum = 0;
    for (int i = 0; i < polygon.size(); i++) {
        int next = (i + 1) % (int)polygon.size();
        double x1 = polygon[i].x;
        double y1 = polygon[i].y;
        double x2 = polygon[next].x;
        double y2 = polygon[next].y;
        sum += (x2 - x1) * (y1 + y2);
    }
    if (sum > 0) {
        return true;
    }
    return false;
}

/**
 * @brief 确定一个轮廓向内和向外的缩进方向
 *
 * @param points
 * @return map.get(true) 向内缩进的方向，map.get(false) 外扩的方向
 */
std::vector<geometry::VECTOR2> getDirection(const std::vector<geometry::POINT>& polygon) {
    std::vector<geometry::VECTOR2> directions;
    int size = (int)polygon.size();
    for (int i = 0; i < size; i++) {
        int prev = posMod(i - 1, size);
        int next = posMod(i + 1, size);
        // TODO: 这边方向是否要反一下需要进一步确定
        // 相邻的两条反向向量
        geometry::VECTOR2 vector1 = geometry::normalize(polygon[prev] - polygon[i]);
        geometry::VECTOR2 vector2 = geometry::normalize(polygon[next] - polygon[i]);
        // 向量和，也就是缩进方向
        geometry::VECTOR2 vector = vector1 + vector2;
        // 得到单位向量
        vector = geometry::normalize(vector);
        // 结果点
        geometry::POINT resPt(polygon[i].x + vector.x, polygon[i].y + vector.y);
        bool inPolygon = geometry::ptInPolygon(resPt, polygon);
        if (inPolygon) {
            directions.emplace_back(vector);
        } else {
            directions.emplace_back(-vector.x, -vector.y);
        }
    }
    return directions;
}

/**
 * @brief 获取轮廓的内缩点
 *
 * @param points
 * @param in
 * @param distance
 * @return
 */
std::vector<geometry::POINT> getInfillPoints(
    const std::vector<geometry::POINT>& polygon,
    const std::vector<geometry::VECTOR2>& in,
    double distance) {
    std::vector<geometry::POINT> res;
    int size = (int)polygon.size();
    for (int i = 0; i < size; ++i) {
        int prev = posMod(i - 1, size);
        int next = posMod(i + 1, size);
        auto v1 = geometry::normalize(polygon[prev] - polygon[i]);
        auto v2 = geometry::normalize(polygon[next] - polygon[i]);
        auto sin = std::abs(multiply(v1, v2));
        //        sin = distance > 0 ? sin : -sin;
        VECTOR2 offset = in[i] * (distance / sin);
        auto vec = POINT(polygon[i].x, polygon[i].y);
        //        if (geometry::dotMultiply(offset, in[i]) < 0) {
        //            offset = {-offset.x, -offset.y};
        //        }
        res.emplace_back(vec + offset);
    }
    return res;
}

/**
 * @brief 验证有效性
 *        这段代码的目的是检查缩进后的轮廓线段是否有效
 *        即线段是否在原始线段的一侧，而不是相交或超出了原始线段
 *        具体来说，它逐个比较原始线段的法向量与缩进后线段的法向量之间的点积（内积）
 *        点积的正负可以帮助判断两个向量是否指向相同的方向
 * @param points
 * @param offset
 * @return
 */
std::vector<bool> isValidOffsetLine(
    const std::vector<geometry::POINT>& points,
    const std::vector<geometry::POINT>& offset) {
    std::vector<bool> res;
    for (size_t i = 0; i < points.size(); i++) {
        size_t next = (i + 1) % points.size();
        auto originalVector = geometry::verticalCWNormalized(points[i], points[next]);
        auto offsetVector = geometry::verticalCWNormalized(offset[i], offset[next]);
        double mul = geometry::dotMultiply(originalVector, offsetVector);
        if (mul < 0) {
            res.push_back(false);
        } else {
            res.push_back(true);
        }
    }
    return res;
}

/**
 * @brief 获取所有的自交点
 *
 * @param points
 * @return
 */
std::map<geometry::POINT, std::vector<geometry::POINT>> getIntersectionPoints(
    const std::vector<geometry::POINT>& polygon) {
    std::map<geometry::POINT, std::vector<geometry::POINT>> intersections;

    for (size_t i = 0; i < polygon.size(); i++) {
        geometry::LINE_SEG aSeg(polygon[i], polygon[(i + 1) % polygon.size()]);

        for (size_t j = 0; j < polygon.size(); j++) {
            if (std::abs(static_cast<int>(i) - static_cast<int>(j)) > 1) {
                geometry::LINE_SEG bSeg(polygon[j], polygon[(j + 1) % polygon.size()]);

                // TODO: 不知道他的 OffsetUtil 内容是什么，暂时用 geometry 的 intersect 处理下
                geometry::POINT point;
                //                if (i < j) {
                //                    point = geometry::intersect(aSeg, a, point);
                //                } else {
                //                    point = geometry::intersect(a, aSeg, point);
                //                }
                //
                //                if (point != nullptr) {
                //                    if (point != lineSegment.getStartPoint() && point != lineSegment.getEndPoint() &&
                //                        point != a.getStartPoint() && point != a.getEndPoint()) {
                //                        if (intersections.find(polygon[i]) != intersections.end()) {
                //                            intersections[polygon[i]].push_back(point);
                //                        } else {
                //                            intersections[polygon[i]] = std::vector<geometry::POINT> {point};
                //                        }
                //
                //                        if (intersections.find(polygon[j]) != intersections.end()) {
                //                            intersections[polygon[j]].push_back(point);
                //                        } else {
                //                            intersections[polygon[j]] = std::vector<geometry::POINT> {point};
                //                        }
                //                    }
                //                }

                if (geometry::intersect(aSeg, bSeg, point)) {
                    if (point != aSeg._st && point != aSeg._et && point != bSeg._st && point != bSeg._et) {
                        if (intersections.find(polygon[i]) != intersections.end()) {
                            intersections[polygon[i]].push_back(point);
                        } else {
                            intersections[polygon[i]] = std::vector<geometry::POINT> {point};
                        }

                        if (intersections.find(polygon[j]) != intersections.end()) {
                            intersections[polygon[j]].push_back(point);
                        } else {
                            intersections[polygon[j]] = std::vector<geometry::POINT> {point};
                        }
                    }
                }
            }
        }
    }

    // 去除重复的点
    for (auto& entry : intersections) {
        std::set<geometry::POINT> uniquePoints(entry.second.begin(), entry.second.end());
        entry.second.assign(uniquePoints.begin(), uniquePoints.end());
    }

    // 自交点排序
    for (auto& entry : intersections) {
        std::sort(
            entry.second.begin(),
            entry.second.end(),
            [&entry](const geometry::POINT& p1, const geometry::POINT& p2) {
                double distance1 = geometry::dist(entry.first, p1);
                double distance2 = geometry::dist(entry.first, p2);
                return distance1 < distance2;
            });
    }

    return intersections;
}

/**
 * @brief 消除全局无效环
 *
 * @param contour 原轮廓
 * @param allPoints 包含自交点的缩进轮廓
 * @param isIntersectionPoint 判断allPoints是否为自交点
 * @return
 */
std::vector<std::vector<POINT>> processGlobalInvalidLoop(
    const std::vector<POINT>& contour,
    const std::vector<POINT>& allPoints,
    const std::vector<bool>& isIntersectionPoint) {
    bool isClockwiseOfContour = isClockwise(contour);
    std::vector<bool> isVisited(allPoints.size(), false);
    std::stack<POINT> stack;
    std::stack<int> indexOfStartPointStack;

    POINT start;
    int indexOfStartPoint = -1;
    for (int i = 0; i < allPoints.size(); i++) {
        if (!isIntersectionPoint[i]) {
            start = allPoints[i];
            indexOfStartPoint = i;
            break;
        }
    }

    stack.push(start);
    indexOfStartPointStack.push(indexOfStartPoint);

    std::vector<std::vector<POINT>> res;

    while (!stack.empty()) {
        POINT startPoint = stack.top();
        stack.pop();
        int index = indexOfStartPointStack.top();
        indexOfStartPointStack.pop();
        isVisited[index] = true;

        std::vector<POINT> loop;
        loop.push_back(startPoint);

        POINT nextPoint = allPoints[(index + 1) % allPoints.size()];
        int indexOfNextPoint = (index + 1) % (int)allPoints.size();

        while (!(nextPoint.x == startPoint.x && nextPoint.y == startPoint.y)) {
            if (isVisited[indexOfNextPoint]) {
                // 遇到重复访问的点，直接返回空向量
                return {};
            } else {
                if (isIntersectionPoint[indexOfNextPoint]) {
                    stack.push(nextPoint);
                    indexOfStartPointStack.push(indexOfNextPoint);

                    for (int k = 0; k < allPoints.size(); k++) {
                        if (nextPoint.x == allPoints[k].x && nextPoint.y == allPoints[k].y) {
                            if (k != indexOfNextPoint) {
                                isVisited[k] = true;
                                loop.push_back(allPoints[k]);
                                nextPoint = allPoints[(k + 1) % allPoints.size()];
                                indexOfNextPoint = (k + 1) % (int)allPoints.size();
                                break;
                            }
                        }
                    }
                } else {
                    loop.push_back(nextPoint);
                    isVisited[indexOfNextPoint] = true;
                    nextPoint = allPoints[(indexOfNextPoint + 1) % allPoints.size()];
                    indexOfNextPoint = (indexOfNextPoint + 1) % (int)allPoints.size();
                }
            }
        }

        if (isClockwise(loop) == isClockwiseOfContour && loop.size() >= 3) {
            res.push_back(loop);
        }
    }

    return res;
}

/**
 * @brief 多边形偏移算法(可处理自相交问题)
 *
 * @param polygon
 * @param distance
 * @return
 */
std::vector<geometry::POLYGON> polygonOffset(std::vector<geometry::POINT>& polygon, double distance, LabScene* scene) {
    auto drawLine = [scene](const geometry::POINT& p1, const geometry::POINT& p2) {
        auto* item = new QGraphicsLineItem();
        item->setLine(p1.x, p1.y, p2.x, p2.y);
        QPen pen(Qt::red);
        pen.setWidth(0);
        item->setPen(pen);
        scene->addItem(item);
    };

    // 传入的点数小于3，认为构不成多边形
    if (polygon.empty() || polygon.size() < 3 || std::abs(distance - 0.0) < geometry::G_EP) {
        return {};
    }
    // 去重方法一
    //    auto pointEqual = [](const geometry::POINT& p1, const geometry::POINT& p2) {
    //        return p1.x == p2.x && p1.y == p2.y;
    //    };
    //    auto newEnd = std::unique(polygon.begin(), polygon.end(), pointEqual);
    //    polygon.erase(newEnd, polygon.end());
    // 去重方法二
    polygon = geometry::deduplication(polygon);
    // 计算缩进方向
    auto direction = getDirection(polygon);
    // 确定内缩点，由此看来in只是偏移方向的单位向量
    auto inShell = getInfillPoints(polygon, direction, distance);
    // 确定每条缩进线段的有效性
    std::vector<bool> validOffsetLine = isValidOffsetLine(polygon, inShell);
    // 是否访问
    std::vector<bool> isVisited(inShell.size(), false);
    // 无效内缩线段的处理
    std::vector<geometry::POINT> finalInShell {};
    for (int i = 0; i < inShell.size();) {
        if (validOffsetLine[i]) {
            // 以i点开头的线段是有效边
            finalInShell.emplace_back(inShell[i]);
            isVisited[i] = true;
            i++;
        } else {
            // 以i点开头的线段是无效边
            if (isVisited[i]) {
                // 访问过了，不再处理
                continue;
            }
            // 确定case1 或 case2
            int vSize = (int)validOffsetLine.size();
            int backIndex = posMod((i - 1), vSize);
            while (!validOffsetLine[backIndex]) {
                backIndex = posMod((backIndex - 1), vSize);
                backIndex = (backIndex - 1) % vSize;
                if (backIndex == i) {
                    break;
                }
            }
            int current = i;
            int next = (i + 1) % vSize;
            while (!validOffsetLine[next]) {
                next = (next + 1) % vSize;
                if (next == i) {
                    break;
                }
            }
            // 根据case 1 和case 2寻找交点
            if (next == backIndex) {
                // all edges are invalid edges
                return {};
            } else if (
                next - backIndex == 2 || (next == 1 && backIndex == vSize - 1) ||
                (next == 0 && backIndex == vSize - 2)) {
                // case 1 只有一条无效边
                // 1. 计算backward edge 和forward edge的交点
                geometry::LINE backwardEdge(inShell[backIndex], inShell[current]);
                geometry::LINE forwardEdge(inShell[next], inShell[(next + 1) % vSize]);

                geometry::POINT intersection;
                geometry::intersect(backwardEdge, forwardEdge, intersection);
                // 2. 更新数据
                if (next == 0) {
                    finalInShell[next] = intersection;
                } else {
                    inShell[next] = intersection;
                }
                isVisited[i] = true;
                i++;
            } else {
                // case 2: 有连续的无效边
                auto backwardEdge = geometry::LINE_SEG(inShell[backIndex], inShell[current]);
                auto forwardEdge = geometry::LINE_SEG(inShell[next], inShell[(next + 1) % inShell.size()]);
                // 计算backward和forward与轮廓是否有交点

                for (int j = current; j != next; j = (j + 1) % (int)vSize) {
                    isVisited[j] = true;
                    auto boundaryEdge = geometry::LINE_SEG(polygon[j], polygon[(j + 1) % polygon.size()]);

                    // 计算boundary的pair-wise offset
                    // 1. 偏移方向
                    auto normalVector = geometry::normalize(boundaryEdge._et - boundaryEdge._st);
                    double mulRes = geometry::multiply(normalVector, direction[j]);
                    if (std::abs(mulRes - 0) < G_EP) {
                        normalVector = {-normalVector.x, -normalVector.y};
                    }
                    normalVector = {3 * normalVector.x, 3 * normalVector.y};
                    // 2. 计算偏移线段
                    auto first =
                        geometry::POINT(boundaryEdge._st.x + normalVector.x, boundaryEdge._st.y + normalVector.y);
                    auto second =
                        geometry::POINT(boundaryEdge._et.x + normalVector.x, boundaryEdge._et.y + normalVector.y);
                    geometry::LINE_SEG offsetEdgeOfPairwise(first, second);
                    //                            drawLineSegment(offsetEdgeOfPairwise,g);
                    // 计算偏移线段与backward、forward的交点，并更新
                    // 此处应该计算偏移线段offsetEdgeOfPairwise 与所有有效偏移边的交点
                    // 不只是backward 和 forward
                    //
                    // System.out.println("offsetEdge="+offsetEdgeOfPairwise+",
                    // backwardEdge="+backwardEdge);
                    geometry::POINT point2 {};
                    geometry::intersect(offsetEdgeOfPairwise, backwardEdge, point2);
                    backwardEdge._et = point2;

                    // System.out.println("offsetEdge="+offsetEdgeOfPairwise+",
                    // forwardEdge="+forwardEdge);
                    geometry::POINT point1 {};
                    geometry::intersect(offsetEdgeOfPairwise, forwardEdge, point1);
                    forwardEdge._st = point1;
                }
                // 所有的无效边处理完成后
                // 计算backward 和forward的交点

                geometry::POINT point;
                geometry::intersect(backwardEdge, forwardEdge, point);
                auto preForwardStartPoint = forwardEdge._st;
                backwardEdge._et = point;
                forwardEdge._st = point;

                finalInShell.emplace_back(backwardEdge._et);

                if (next < current) {
                    i = (int)inShell.size();
                    for (auto& k : finalInShell) {
                        if (k == preForwardStartPoint) {
                            k = point;
                            break;
                        }
                    }
                } else {
                    inShell[next] = forwardEdge._st;
                    i = next;
                }
            }
        }
    }

    // 全局无效环的处理
    if (finalInShell.size() >= 3) {
        // 将finalInShell中处于轮廓外的点消除
        std::vector<geometry::POINT> finalFinalInShell {};
        std::vector<geometry::LINE_SEG> contour {};
        for (int m = 0; m < polygon.size(); m++) {
            auto lineSegment = geometry::LINE_SEG(polygon[m], polygon[(m + 1) % polygon.size()]);
            contour.emplace_back(lineSegment);
        }
        for (auto& point : finalInShell) {
            finalFinalInShell.emplace_back(point);

            //            if (distance - 0 > G_EP) {
            //                // 向内缩进
            //                if (geometry::ptInSegPolygon(point, contour)) {
            //                    finalFinalInShell.emplace_back(point);
            //                }
            //            } else if (distance - 0 < G_EP) {
            //                // 向外扩张
            //                if (!geometry::ptInSegPolygon(point, contour)) {
            //                    finalFinalInShell.emplace_back(point);
            //                }
            //            }
        }
        // finalInShell 可能有相同的点，去除重复点
        finalInShell = geometry::deduplication(finalFinalInShell);
        // 消除全局无效环
        // 1.获取所有的自交点
        auto intersection = getIntersectionPoints(finalInShell);

        if (intersection.empty()) {
            // 如果没有自交点，说明不存在全局无效环，不用处理
            std::vector<std::vector<geometry::POINT>> res {};
            res.emplace_back(finalInShell);
            return res;
        } else {
            // 有自交点，需要消除全局无效环
            std::vector<geometry::POINT> allPoints {};
            std::vector<bool> isIntersectionPoint;
            for (auto& i : finalInShell) {
                allPoints.emplace_back(i);
                isIntersectionPoint.emplace_back(false);

                if (!intersection[i].empty()) {
                    for (auto& j : intersection[i]) {
                        allPoints.emplace_back(j);
                    }
                }
            }
            std::vector<std::vector<geometry::POINT>> lists =
                processGlobalInvalidLoop(polygon, allPoints, isIntersectionPoint);
            return lists;
        }
    }
    return {};
}

} // namespace geometry
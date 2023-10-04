#ifndef GEOMETRY_ALGO_EXTRA_UTILS_H
#define GEOMETRY_ALGO_EXTRA_UTILS_H

#include "geometry_algo_core.h"
namespace rededa {

class Arc;
class Point;

namespace pcbcommon {

class BI_LineSegment;

} // namespace pcbcommon

} // namespace rededa
namespace geometry {

// TODO: 注意：这几段代码之前只在finger里用的，后面需要集中改掉

/**
 * @brief rededa的arc转换geometry的arc
 * @param arc rededa的arc
 * @return geometry的arc
 */
geometry::ARC rededaArc2GeometryArc(rededa::Arc* arc);

/**
* @brief BI线段转换标准线段
*
* @param seg 线段
* @return geometry::LINE_SEG 线段
*/
geometry::LINE_SEG normalizedBISeg(rededa::pcbcommon::BI_LineSegment* seg);

/**
 * @brief BI线段转换标准圆弧
 * 
 * @param seg 线段
 * @return geometry::ARC 圆弧
 */
geometry::ARC normalizedBIArcSeg(rededa::pcbcommon::BI_LineSegment* seg);

/**
 * @brief geometry POINT 转 rededa Point
 * 
 * @param pt geometry POINT
 * @return rededa::Point rededa Point
 */
rededa::Point geometryPt2Point(const geometry::POINT& pt);

} // namespace geometry

#endif // GEOMETRY_ALGO_EXTRA_UTILS_H
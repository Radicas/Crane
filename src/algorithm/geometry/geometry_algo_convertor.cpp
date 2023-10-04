#include "geometry_algo_convertor.h"
#include "items/bi_linesegment.h"
#include "units/lengthunit.h"
#include "units/point.h"
#include "units/angle.h"

namespace geometry {

geometry::ARC rededaArc2GeometryArc(rededa::Arc* arc) {
    if (!arc) {
        return {};
    }
    geometry::ARC newArc {};
    newArc._d = arc->direction == rededa::RotateDirection::CW ? DIRECTION::CW :
                                                                DIRECTION::CCW;
    newArc._ct = arc->center.toPxQPointF();
    newArc._st = arc->startPoint.toPxQPointF();
    newArc._et = arc->endPoint.toPxQPointF();
    newArc._r = arc->radius.toPx();
    newArc._sta = arc->startAngle.toDeg();
    newArc._spa = arc->spanAngle.toDeg();
    return newArc;
}

geometry::LINE_SEG normalizedBISeg(rededa::pcbcommon::BI_LineSegment* seg) {
    return {
        seg->getStartPoint().toPxQPointF(),
        seg->getEndPoint().toPxQPointF()};
}

geometry::ARC normalizedBIArcSeg(rededa::pcbcommon::BI_LineSegment* seg) {
    auto arc = dynamic_cast<rededa::Arc*>(seg->getGeometry());
    return rededaArc2GeometryArc(arc);
}

rededa::Point geometryPt2Point(const geometry::POINT& pt) {
    return rededa::Point::fromPx(pt.x, pt.y);
}

} // namespace geometry

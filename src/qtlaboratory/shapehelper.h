//
// Created by Radica on 2023/3/21.
//

#ifndef CRANE_SHAPEHELPER_H
#define CRANE_SHAPEHELPER_H

#include "shape.h"
#include <QPainterPath>

namespace Crane {

QVector<PathElement> convertToPathElements(const QPainterPath& path) {
//    QVector<PathElement> elements;
//
//    for (int i = 0; i < path.elementCount(); ++i) {
//        QPainterPath::Element elem = path.elementAt(i);
//
//        switch (elem.type) {
//        case QPainterPath::MoveToElement: {
//            // do nothing
//            break;
//        }
//        case QPainterPath::LineToElement: {
//            PathElement pe(elem.startPoint, elem.endPoint);
//            elements.append(pe);
//            break;
//        }
//        case QPainterPath::CurveToElement: {
//            // compute control point
//            QPointF ctrlPt = elem.controlPoint;
//            if (i > 0 && path.elementAt(i - 1).type == QPainterPath::CurveToElement) {
//                ctrlPt = 2 * path.elementAt(i - 1).endPoint - path.elementAt(i - 1).controlPoint;
//            }
//
//            // compute path element
//            PathElement pe(path.elementAt(i - 1).endPoint, elem.endPoint, ctrlPt, 0, 0, 0);
//            elements.append(pe);
//            break;
//        }
//        case QPainterPath::CurveToDataElement: {
//            // compute path element
//            PathElement pe(path.elementAt(i - 1).endPoint, elem.endPoint, elem.controlPoint, 0, 0, 0);
//            elements.append(pe);
//            break;
//        }
//        case QPainterPath::ArcToElement: {
//            // compute path element
//            PathElement pe(elem.startPoint, elem.endPoint, elem.rect.center(), elem.rect.width() / 2, elem.startAngle,
//                           elem.spanAngle);
//            elements.append(pe);
//            break;
//        }
//        case QPainterPath::CloseSubpathElement: {
//            // do nothing
//            break;
//        }
//        }
//    }
//
//    return elements;
}

}  // namespace Crane

#endif  // CRANE_SHAPEHELPER_H

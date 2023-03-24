#include "qtlaboratory.h"
#include "geometryitem.h"
#include "labscene.h"
#include "labview.h"
#include "ui_qtlaboratory.h"

#include <QDebug>
#include <QTableView>
#include <boost/geometry/geometries/point_xy.hpp>
#include <boost/geometry/geometries/polygon.hpp>
#include <boost/geometry/geometry.hpp>
#include <boost/polygon/polygon.hpp>
#include <vector>

/* region Constructors / Destructor */
QtLaboratory::QtLaboratory(QWidget* parent)
    : QDialog(parent)
    , m_ui(new Ui::QtLabDlg)
    , m_scene(new LabScene()) {
    initScene();
    initConnects();
}

QtLaboratory::~QtLaboratory() = default;
/* endregion */

/* region Private Methods */
void QtLaboratory::initScene() {
    m_ui->setupUi(this);
    auto layout = new QVBoxLayout();
    auto view   = new LabView(m_scene.get(), this);
    view->setBackgroundBrush(QBrush(Qt::black));
    layout->addWidget(view);
    m_ui->mainWgt->setLayout(layout);
}

void QtLaboratory::initConnects() {
    connect(m_ui->trianlgeBtn, SIGNAL(clicked()), this, SLOT(triangleBtnClicked()));
    connect(m_ui->rectBtn, SIGNAL(clicked()), this, SLOT(rectBtnClicked()));
    connect(m_ui->roundRectBtn, SIGNAL(clicked()), this, SLOT(roundRectBtnClicked()));
    connect(m_ui->circleBtn, SIGNAL(clicked()), this, SLOT(circleBtnClicked()));
    connect(m_ui->curvedPolygonBtn, SIGNAL(clicked()), this, SLOT(curvedPolyBtnClicked()));
    connect(m_ui->intersectBtn, SIGNAL(clicked()), this, SLOT(intersectBtnClicked()));
    connect(m_ui->unionBtn, SIGNAL(clicked()), this, SLOT(unionBtnClicked()));
    connect(m_ui->subtractBtn, SIGNAL(clicked()), this, SLOT(subtractBtnClicked()));
    connect(m_ui->pathToPolygonBtn, SIGNAL(clicked()), this, SLOT(pathToPolygonBtnClicked()));
    connect(m_ui->clearBtn, SIGNAL(clicked()), this, SLOT(clearBtnClicked()));
}
/* endregion */

/* region Slots Methods */
void QtLaboratory::triangleBtnClicked() {
    namespace bg = boost::geometry;
    namespace bp = boost::polygon;
    //    typedef bg::model::point<double, 2, bg::cs::cartesian> point_t;  // 二纬笛卡尔坐标系
    typedef bg::model::d2::point_xy<double> point_t;
    typedef bg::model::polygon<point_t>     polygon_t;  // polygon_t的外边框是顺时针，孔洞边框是
                                                        // 逆时针
    polygon_t poly1;
#if !defined(BOOST_NO_CXX11_UNIFIED_INITIALIZATION_SYNTAX) && !defined(BOOST_NO_CXX11_HDR_INITIALIZER_LIST)
    polygon_t poly2{ { { 0.0, 0.0 }, { 0.0, 5.0 }, { 5.0, 5.0 }, { 5.0, 0.0 }, { 0.0, 0.0 } },
                     { { 1.0, 1.0 }, { 4.0, 1.0 }, { 4.0, 4.0 }, { 1.0, 4.0 }, { 1.0, 1.0 } } };
#endif
    bg::append(poly1.outer(), point_t(0.0, 0.0));
    bg::append(poly1.outer(), point_t(0.0, 5.0));
    bg::append(poly1.outer(), point_t(5.0, 5.0));
    bg::append(poly1.outer(), point_t(5.0, 0.0));
    bg::append(poly1.outer(), point_t(0.0, 0.0));

    poly1.inners().resize(1);
    bg::append(poly1.inners()[0], point_t(1.0, 1.0));
    bg::append(poly1.inners()[0], point_t(4.0, 1.0));
    bg::append(poly1.inners()[0], point_t(4.0, 4.0));
    bg::append(poly1.inners()[0], point_t(1.0, 4.0));
    bg::append(poly1.inners()[0], point_t(1.0, 1.0));

    double a = bg::area(poly1);  // 面积
    std::cout << "poly1 area: " << a << std::endl;
    double b = bg::area(poly2);  // 面积
    std::cout << "poly2 area: " << b << std::endl;

    polygon_t result{};
    bg::union_(poly1, poly2, result);
    bg::intersection(poly1, poly2, result);
}

void QtLaboratory::rectBtnClicked() {
    auto* item = new RectangleItem();
    m_scene->addItem(item);
}

void QtLaboratory::roundRectBtnClicked() {
    //    m_scene->addItem(item);
}

void QtLaboratory::circleBtnClicked() {
    auto* item = new CircleItem();
    m_scene->addItem(item);
    //    m_scene->addItem(item);
}

void QtLaboratory::curvedPolyBtnClicked() {
    auto* item = new CurvePolygonItem();
    m_scene->addItem(item);
}

void QtLaboratory::intersectBtnClicked() {}

void QtLaboratory::unionBtnClicked() {
    namespace bg = boost::geometry;
    namespace bp = boost::polygon;

    auto& items = m_scene->getSelectedItems();
    if (items.size() >= 2) {
        auto* item1  = items.at(0);
        auto& outer1 = dynamic_cast<Shape*>(item1)->getOuter();
        std::cout << "rect: " << std::endl;
        for (auto p : outer1) {
            std::cout << "outline: "
                      << "(" << p.x << "," << p.y << ")" << std::endl;
        }
        auto& polys_inner = dynamic_cast<Shape*>(item1)->getInner();
        for (const auto& inner : polys_inner) {
            for (const auto& p : inner) {
                std::cout << "inner: "
                          << "(" << p.x << "," << p.y << ")" << std::endl;
            }
        }

        auto* item2  = items.at(1);
        auto& outer2 = dynamic_cast<Shape*>(item2)->getOuter();
        std::cout << "circle: " << std::endl;
        for (auto p : outer2) {
            std::cout << "outline: "
                      << "(" << p.x << "," << p.y << ")" << std::endl;
        }
    }
}

void QtLaboratory::subtractBtnClicked() {}

void QtLaboratory::pathToPolygonBtnClicked() {
//    QVector<QPointF>      points;
//    QPainterPath::Element element;
//    auto&                 items = m_scene->getSelectedItems();
//    if (!items.empty()) {
//        auto path = items[0]->shape();
//        for (int i = 0; i < path.elementCount(); ++i) {
//            element = path.elementAt(i);
//            if (element.type == QPainterPath::ArcToElement) {
//                qreal x1, y1, x2, y2;
//                qreal rx             = element.rect.width() / 2.0;
//                qreal ry             = element.rect.height() / 2.0;
//                qreal angle1         = -element.angle * M_PI / 180.0;
//                qreal angle2         = -(element.angle + element.span) * M_PI / 180.0;
//                x1                   = element.rect.center().x() + rx * qCos(angle1);
//                y1                   = element.rect.center().y() + ry * qSin(angle1);
//                x2                   = element.rect.center().x() + rx * qCos(angle2);
//                y2                   = element.rect.center().y() + ry * qSin(angle2);
//                int   numSegments    = qAbs(element.span) / 5;
//                qreal angleIncrement = element.span / qreal(numSegments);
//                qreal currentAngle   = angle1 + angleIncrement;
//                for (int j = 0; j < numSegments; ++j) {
//                    qreal x = element.rect.center().x() + rx * qCos(currentAngle);
//                    qreal y = element.rect.center().y() + ry * qSin(currentAngle);
//                    points.append(QPointF(x, y));
//                    currentAngle += angleIncrement;
//                }
//            }
//            else if (element.type == QPainterPath::LineToElement) {
//                points.append(element.p);
//            }
//        }
//    }
//    auto* polygonItem = new Polygon(outline);
//    qDebug() << "points" << points;
//    m_scene->addItem(polygonItem);
}

void QtLaboratory::clearBtnClicked() {
    for (auto* i : m_scene->items()) {
        m_scene->removeItem(i);
    }
}

/* endregion */
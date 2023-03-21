#include "qtlaboratory.h"
#include "geometryitem.h"
#include "labscene.h"
#include "labview.h"
#include "ui_qtlaboratory.h"

#include <QDebug>
#include <QTableView>

// #include <boost/geometry.hpp>
// #include <boost/geometry/geometries/adapted/boost_tuple.hpp>
// #include <boost/geometry/geometries/polygon.hpp>
#include <vector>

// BOOST_GEOMETRY_REGISTER_BOOST_TUPLE_CS(cs::cartesian)
//
// typedef boost::geometry::model::d2::point_xy<double> Point;
// typedef boost::geometry::model::polygon<Point>       Polygon;

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
void QtLaboratory::rectBtnClicked() {
    auto* item = new Rectangle();
    m_scene->addItem(item);
}

void QtLaboratory::roundRectBtnClicked() {
    //    m_scene->addItem(item);
}

void QtLaboratory::circleBtnClicked() {
    auto* item = new Circle();
    m_scene->addItem(item);
    //    m_scene->addItem(item);
}

void QtLaboratory::curvedPolyBtnClicked() {
    auto* item = new CurvePolygon();
    m_scene->addItem(item);
}
void QtLaboratory::intersectBtnClicked() {}

void QtLaboratory::unionBtnClicked() {}

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
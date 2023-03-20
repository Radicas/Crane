#include "qtlaboratory.h"
#include "geometryitem.h"
#include "labscene.h"
#include "ui_qtlaboratory.h"

#include <QDebug>
#include <QGraphicsView>
#include <QTableView>

#include <boost/geometry.hpp>
#include <boost/geometry/geometries/adapted/boost_tuple.hpp>
#include <boost/geometry/geometries/polygon.hpp>
#include <vector>

BOOST_GEOMETRY_REGISTER_BOOST_TUPLE_CS(cs::cartesian)

typedef boost::geometry::model::d2::point_xy<double> Point;
typedef boost::geometry::model::polygon<Point>       Polygon;

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
    auto view   = new QGraphicsView(m_scene.get(), this);
    view->setBackgroundBrush(QBrush(Qt::black));
    layout->addWidget(view);
    m_ui->mainWgt->setLayout(layout);
}

void QtLaboratory::initConnects() {
    connect(m_ui->rectBtn, SIGNAL(clicked()), this, SLOT(rectBtnClicked()));
    connect(m_ui->roundRectBtn, SIGNAL(clicked()), this, SLOT(roundRectBtnClicked()));
    connect(m_ui->circleBtn, SIGNAL(clicked()), this, SLOT(circleBtnClicked()));
    connect(m_ui->polygonBtn, SIGNAL(clicked()), this, SLOT(comPolyBtnClicked()));
    connect(m_ui->mergeBtn, SIGNAL(clicked()), this, SLOT(mergeBtnClicked()));
    connect(m_ui->overlapsBtn, SIGNAL(clicked()), this, SLOT(overlapsBtnClicked()));
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

void QtLaboratory::comPolyBtnClicked() {
    //    m_scene->addItem(item);
}

void QtLaboratory::mergeBtnClicked() {
}

void QtLaboratory::overlapsBtnClicked() {

}

void QtLaboratory::clearBtnClicked() {
    for (auto* i : m_scene->items()) {
        m_scene->removeItem(i);
    }
}
/* endregion */
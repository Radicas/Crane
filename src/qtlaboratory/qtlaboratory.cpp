#include "qtlaboratory.h"
#include "geometryitem.h"
#include "labscene.h"
#include "labview.h"
#include "shapehelper.h"
#include "ui_qtlaboratory.h"

#include <QDebug>
#include <QTableView>
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

    bg::union_(poly1, poly2, result);
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
    typedef bg::model::d2::point_xy<double> point_t;
    typedef bg::model::polygon<point_t>     polygon_t;

    polygon_t              poly1, poly2;  // boost polygon，用于布尔运算
    std::vector<polygon_t> result;

    auto& items = m_scene->getSelectedItems();  // 获取选中的对象

    if (items.size() >= 2) {
        auto* item1 = items.at(0);
        auto* item2 = items.at(1);

        poly1 = Crane::toBoostPolygon(dynamic_cast<Shape*>(item1)->getPolygonWithHoles());
        poly2 = Crane::toBoostPolygon(dynamic_cast<Shape*>(item2)->getPolygonWithHoles());

        bg::correct(poly1);
        bg::correct(poly2);
        bg::union_(poly1, poly2, result);

        for (const auto& res : result) {
            auto  r_poly = Crane::fromBoostPolygon(res);
            auto* item   = new PolygonItem(r_poly);
            m_scene->addItem(item);
        }
    }
}

void QtLaboratory::subtractBtnClicked() {}

void QtLaboratory::pathToPolygonBtnClicked() {}

void QtLaboratory::clearBtnClicked() {
    for (auto* i : m_scene->items()) {
        m_scene->removeItem(i);
    }
}

/* endregion */
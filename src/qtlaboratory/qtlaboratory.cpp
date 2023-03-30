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

void QtLaboratory::intersectBtnClicked() {

    typedef boost::geometry::model::polygon<boost::geometry::model::d2::point_xy<double>> polygon;

    polygon green, blue;

    boost::geometry::read_wkt(
        "POLYGON((2 1.3,2.4 1.7,2.8 1.8,3.4 1.2,3.7 1.6,3.4 2,4.1 3,5.3 2.6,5.4 1.2,4.9 0.8,2.9 0.7,2 1.3)"
        "(4.0 2.0, 4.2 1.4, 4.8 1.9, 4.4 2.2, 4.0 2.0))",
        green);

    boost::geometry::read_wkt(
        "POLYGON((4.0 -0.5 , 3.5 1.0 , 2.0 1.5 , 3.5 2.0 , 4.0 3.5 , 4.5 2.0 , 6.0 1.5 , 4.5 1.0 , 4.0 -0.5))", blue);

    std::list<polygon> output;
    boost::geometry::difference(green, blue, output);

    int i = 0;
    std::cout << "green - blue:" << std::endl;
    BOOST_FOREACH (polygon const& p, output) {
        auto  r_poly = Crane::fromBoostPolygon(p);
        auto* item   = new PolygonItem(r_poly);
        m_scene->addItem(item);
        std::cout << i++ << ": " << boost::geometry::area(p) << std::endl;
    }

    output.clear();
    boost::geometry::difference(blue, green, output);

    i = 0;
    std::cout << "blue - green:" << std::endl;
    BOOST_FOREACH (polygon const& p, output) {
        std::cout << i++ << ": " << boost::geometry::area(p) << std::endl;
    }
}

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
            std::cout << "poly area: " << bg::area(res) << std::endl;
            auto  r_poly = Crane::fromBoostPolygon(res);
            auto* item   = new PolygonItem(r_poly);
            m_scene->addItem(item);
        }
    }
}

void QtLaboratory::subtractBtnClicked() {
    typedef boost::polygon::polygon_with_holes_data<double>               polygon;
    typedef boost::polygon::polygon_traits<polygon>::point_type           point;
    typedef boost::polygon::polygon_with_holes_traits<polygon>::hole_type hole;

    point pts[5]      = { boost::polygon::construct<point>(0, 0), boost::polygon::construct<point>(0, 10),
                          boost::polygon::construct<point>(10, 10), boost::polygon::construct<point>(10, 0),
                          boost::polygon::construct<point>(0, 0) };
    point hole_pts[5] = { boost::polygon::construct<point>(1, 1), boost::polygon::construct<point>(9, 1),
                          boost::polygon::construct<point>(9, 9), boost::polygon::construct<point>(1, 9),
                          boost::polygon::construct<point>(1, 1) };

    point pts2[5]      = { boost::polygon::construct<point>(5, 5), boost::polygon::construct<point>(5, 15),
                           boost::polygon::construct<point>(15, 15), boost::polygon::construct<point>(15, 5),
                           boost::polygon::construct<point>(5, 5) };
    point hole_pts2[5] = { boost::polygon::construct<point>(6, 6), boost::polygon::construct<point>(14, 6),
                           boost::polygon::construct<point>(14, 14), boost::polygon::construct<point>(6, 14),
                           boost::polygon::construct<point>(6, 6) };

    hole hls1[1];
    hole hls2[1];

    boost::polygon::set_points(hls1[0], hole_pts, hole_pts + 5);
    boost::polygon::set_points(hls2[0], hole_pts2, hole_pts2 + 5);

    polygon              poly1, poly2;
    std::vector<polygon> res;

    boost::polygon::set_points(poly1, pts, pts + 5);
    boost::polygon::set_holes(poly1, hls1, hls1 + 1);
    boost::polygon::set_points(poly2, pts2, pts2 + 5);
    boost::polygon::set_holes(poly2, hls2, hls2 + 1);

    //    boost::geometry::union_(poly1, poly2, res);

    std::cout << "Area (using Boost.Polygon) poly1 : " << boost::polygon::area(poly1) << std::endl;
    std::cout << "Area (using Boost.Polygon) poly2 : " << boost::polygon::area(poly2) << std::endl;
    std::cout << "union_ area: " << boost::polygon::area(poly2) << std::endl;
}

void QtLaboratory::pathToPolygonBtnClicked() {

    typedef boost::geometry::model::polygon<boost::geometry::model::d2::point_xy<double>> polygon;

    polygon green, blue;

    boost::geometry::read_wkt(
        "POLYGON((2 1.3,2.4 1.7,2.8 1.8,3.4 1.2,3.7 1.6,3.4 2,4.1 3,5.3 2.6,5.4 1.2,4.9 0.8,2.9 0.7,2 1.3)"
        "(4.0 2.0, 4.2 1.4, 4.8 1.9, 4.4 2.2, 4.0 2.0))",
        green);

    boost::geometry::read_wkt(
        "POLYGON((4.0 -0.5 , 3.5 1.0 , 2.0 1.5 , 3.5 2.0 , 4.0 3.5 , 4.5 2.0 , 6.0 1.5 , 4.5 1.0 , 4.0 -0.5))", blue);

    std::vector<polygon> output;
    boost::geometry::union_(green, blue, output);

    int i = 0;
    std::cout << "green || blue:" << std::endl;
    BOOST_FOREACH (polygon const& p, output) {
        auto  r_poly = Crane::fromBoostPolygon(p);
        auto* item   = new PolygonItem(r_poly);
        m_scene->addItem(item);
        std::cout << i++ << ": " << boost::geometry::area(p) << std::endl;
    }
}

void QtLaboratory::clearBtnClicked() {
    for (auto* i : m_scene->items()) {
        m_scene->removeItem(i);
    }
}

/* endregion */
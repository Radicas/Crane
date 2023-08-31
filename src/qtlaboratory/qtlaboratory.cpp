#include "qtlaboratory.h"
#include "geometryitem.h"
#include "labscene.h"
#include "labview.h"
#include "shapehelper.h"
#include "ui_qtlaboratory.h"
#include "algorithm/geometry/polygon_offset.hpp"

#include <QDebug>
#include <QTableView>
#include <vector>

/* region Constructors / Destructor */
QtLaboratory::QtLaboratory(QWidget* parent) :
    QDialog(parent),
    m_ui(new Ui::QtLabDlg),
    m_scene(new LabScene()) {
    initScene();
    initConnects();
}

QtLaboratory::~QtLaboratory() = default;
/* endregion */

/* region Private Methods */
void QtLaboratory::initScene() {
    m_ui->setupUi(this);
    auto layout = new QVBoxLayout();
    auto view = new LabView(m_scene.get(), this);
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
    connect(m_ui->expandBtn, SIGNAL(clicked()), this, SLOT(expandBtnClicked()));
    connect(m_ui->clearBtn, SIGNAL(clicked()), this, SLOT(clearBtnClicked()));
}
/* endregion */

/* region Slots Methods */
void QtLaboratory::triangleBtnClicked() {
}

void QtLaboratory::rectBtnClicked() {
    // auto* item = new RectangleItem();
    // m_scene->addItem(item);
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
    // auto* item = new CurvePolygonItem();
    // m_scene->addItem(item);
}

void QtLaboratory::intersectBtnClicked() {
    // typedef boost::geometry::model::polygon<boost::geometry::model::d2::point_xy<double>> polygon;

    // polygon green, blue;

    // boost::geometry::read_wkt(
    //     "POLYGON((2 1.3,2.4 1.7,2.8 1.8,3.4 1.2,3.7 1.6,3.4 2,4.1 3,5.3 2.6,5.4 1.2,4.9 0.8,2.9 0.7,2 1.3)"
    //     "(4.0 2.0, 4.2 1.4, 4.8 1.9, 4.4 2.2, 4.0 2.0))",
    //     green);

    // boost::geometry::read_wkt(
    //     "POLYGON((4.0 -0.5 , 3.5 1.0 , 2.0 1.5 , 3.5 2.0 , 4.0 3.5 , 4.5 2.0 , 6.0 1.5 , 4.5 1.0 , 4.0 -0.5))",
    //     blue);

    // std::list<polygon> output;
    // boost::geometry::difference(green, blue, output);

    // int i = 0;
    // std::cout << "green - blue:" << std::endl;
    // BOOST_FOREACH (polygon const& p, output) {
    //     auto  r_poly = Crane::fromBoostPolygon(p);
    //     auto* item   = new PolygonItem(r_poly);
    //     m_scene->addItem(item);
    //     std::cout << i++ << ": " << boost::geometry::area(p) << std::endl;
    // }

    // output.clear();
    // boost::geometry::difference(blue, green, output);

    // i = 0;
    // std::cout << "blue - green:" << std::endl;
    // BOOST_FOREACH (polygon const& p, output) {
    //     std::cout << i++ << ": " << boost::geometry::area(p) << std::endl;
    // }
}

void QtLaboratory::unionBtnClicked() {
    // namespace bg = boost::geometry;
    // namespace bp = boost::polygon;
    // typedef bg::model::d2::point_xy<double> point_t;
    // typedef bg::model::polygon<point_t>     polygon_t;

    // polygon_t              poly1, poly2;  // boost polygon，用于布尔运算
    // std::vector<polygon_t> result;

    // auto& items = m_scene->getSelectedItems();  // 获取选中的对象

    // if (items.size() >= 2) {
    //     auto* item1 = items.at(0);
    //     auto* item2 = items.at(1);

    //     poly1 = Crane::toBoostPolygon(dynamic_cast<Shape*>(item1)->getPolygonWithHoles());
    //     poly2 = Crane::toBoostPolygon(dynamic_cast<Shape*>(item2)->getPolygonWithHoles());

    //     bg::correct(poly1);
    //     bg::correct(poly2);
    //     bg::union_(poly1, poly2, result);

    //     for (const auto& res : result) {
    //         std::cout << "poly area: " << bg::area(res) << std::endl;
    //         auto  r_poly = Crane::fromBoostPolygon(res);
    //         auto* item   = new PolygonItem(r_poly);
    //         m_scene->addItem(item);
    //     }
    // }
}

void QtLaboratory::subtractBtnClicked() {
    // typedef boost::polygon::polygon_with_holes_data<double>               polygon;
    // typedef boost::polygon::polygon_traits<polygon>::point_type           point;
    // typedef boost::polygon::polygon_with_holes_traits<polygon>::hole_type hole;

    // point pts[5]      = { boost::polygon::construct<point>(0, 0), boost::polygon::construct<point>(0, 10),
    //                       boost::polygon::construct<point>(10, 10), boost::polygon::construct<point>(10, 0),
    //                       boost::polygon::construct<point>(0, 0) };
    // point hole_pts[5] = { boost::polygon::construct<point>(1, 1), boost::polygon::construct<point>(9, 1),
    //                       boost::polygon::construct<point>(9, 9), boost::polygon::construct<point>(1, 9),
    //                       boost::polygon::construct<point>(1, 1) };

    // point pts2[5]      = { boost::polygon::construct<point>(5, 5), boost::polygon::construct<point>(5, 15),
    //                        boost::polygon::construct<point>(15, 15), boost::polygon::construct<point>(15, 5),
    //                        boost::polygon::construct<point>(5, 5) };
    // point hole_pts2[5] = { boost::polygon::construct<point>(6, 6), boost::polygon::construct<point>(14, 6),
    //                        boost::polygon::construct<point>(14, 14), boost::polygon::construct<point>(6, 14),
    //                        boost::polygon::construct<point>(6, 6) };

    // hole hls1[1];
    // hole hls2[1];

    // boost::polygon::set_points(hls1[0], hole_pts, hole_pts + 5);
    // boost::polygon::set_points(hls2[0], hole_pts2, hole_pts2 + 5);

    // polygon              poly1, poly2;
    // std::vector<polygon> res;

    // boost::polygon::set_points(poly1, pts, pts + 5);
    // boost::polygon::set_holes(poly1, hls1, hls1 + 1);
    // boost::polygon::set_points(poly2, pts2, pts2 + 5);
    // boost::polygon::set_holes(poly2, hls2, hls2 + 1);

    // //    boost::geometry::union_(poly1, poly2, res);

    // std::cout << "Area (using Boost.Polygon) poly1 : " << boost::polygon::area(poly1) << std::endl;
    // std::cout << "Area (using Boost.Polygon) poly2 : " << boost::polygon::area(poly2) << std::endl;
    // std::cout << "union_ area: " << boost::polygon::area(poly2) << std::endl;
}

void QtLaboratory::pathToPolygonBtnClicked() {
    // typedef boost::geometry::model::polygon<boost::geometry::model::d2::point_xy<double>> polygon;

    // polygon green, blue;

    // boost::geometry::read_wkt(
    //     "POLYGON((2 1.3,2.4 1.7,2.8 1.8,3.4 1.2,3.7 1.6,3.4 2,4.1 3,5.3 2.6,5.4 1.2,4.9 0.8,2.9 0.7,2 1.3)"
    //     "(4.0 2.0, 4.2 1.4, 4.8 1.9, 4.4 2.2, 4.0 2.0))",
    //     green);

    // boost::geometry::read_wkt(
    //     "POLYGON((4.0 -0.5 , 3.5 1.0 , 2.0 1.5 , 3.5 2.0 , 4.0 3.5 , 4.5 2.0 , 6.0 1.5 , 4.5 1.0 , 4.0 -0.5))",
    //     blue);

    // std::vector<polygon> output;
    // boost::geometry::union_(green, blue, output);

    // int i = 0;
    // std::cout << "green || blue:" << std::endl;
    // BOOST_FOREACH (polygon const& p, output) {
    //     auto  r_poly = Crane::fromBoostPolygon(p);
    //     auto* item   = new PolygonItem(r_poly);
    //     m_scene->addItem(item);
    //     std::cout << i++ << ": " << boost::geometry::area(p) << std::endl;
    // }
}

void QtLaboratory::clearBtnClicked() {
    // for (auto* i : m_scene->items()) {
    //     m_scene->removeItem(i);
    // }
}

void QtLaboratory::expandBtnClicked() {
    geometry::POLYGON outer;
    outer.emplace_back(17.016721, -14.025827);
    outer.emplace_back(16.942620, -14.117334);
    outer.emplace_back(16.436149, -14.699962);
    outer.emplace_back(15.909652, -15.264560);
    outer.emplace_back(15.363773, -15.810440);
    outer.emplace_back(14.799175, -16.336936);
    outer.emplace_back(14.216546, -16.843407);
    outer.emplace_back(13.616597, -17.329237);
    outer.emplace_back(13.000058, -17.793832);
    outer.emplace_back(12.367680, -18.236628);
    outer.emplace_back(11.720235, -18.657084);
    outer.emplace_back(11.058510, -19.054688);
    outer.emplace_back(10.383312, -19.428957);
    outer.emplace_back(9.695463, -19.779433);
    outer.emplace_back(8.995802, -20.105690);
    outer.emplace_back(8.285181, -20.407331);
    outer.emplace_back(7.564466, -20.683988);
    outer.emplace_back(6.834535, -20.935323);
    outer.emplace_back(6.096277, -21.161031);
    outer.emplace_back(5.350591, -21.360837);
    outer.emplace_back(4.598387, -21.534497);
    outer.emplace_back(3.840580, -21.681800);
    outer.emplace_back(3.078094, -21.802566);
    outer.emplace_back(2.311858, -21.896648);
    outer.emplace_back(1.542805, -21.963931);
    outer.emplace_back(0.771873, -22.004334);
    outer.emplace_back(-0.000000, -22.017807);
    outer.emplace_back(-0.771873, -22.004334);
    outer.emplace_back(-1.542805, -21.963931);
    outer.emplace_back(-2.311858, -21.896648);
    outer.emplace_back(-3.078094, -21.802566);
    outer.emplace_back(-3.840580, -21.681800);
    outer.emplace_back(-4.598387, -21.534497);
    outer.emplace_back(-5.350591, -21.360837);
    outer.emplace_back(-6.096277, -21.161031);
    outer.emplace_back(-6.834535, -20.935323);
    outer.emplace_back(-7.564466, -20.683988);
    outer.emplace_back(-8.285181, -20.407331);
    outer.emplace_back(-8.995802, -20.105690);
    outer.emplace_back(-9.695463, -19.779433);
    outer.emplace_back(-10.383312, -19.428957);
    outer.emplace_back(-11.058510, -19.054688);
    outer.emplace_back(-11.720235, -18.657084);
    outer.emplace_back(-12.367680, -18.236628);
    outer.emplace_back(-13.000058, -17.793832);
    outer.emplace_back(-13.616597, -17.329237);
    outer.emplace_back(-14.216546, -16.843407);
    outer.emplace_back(-14.799175, -16.336936);
    outer.emplace_back(-15.363773, -15.810440);
    outer.emplace_back(-15.909652, -15.264560);
    outer.emplace_back(-16.436149, -14.699962);
    outer.emplace_back(-16.942620, -14.117334);
    outer.emplace_back(-17.428449, -13.517384);
    outer.emplace_back(-17.893045, -12.900845);
    outer.emplace_back(-18.335840, -12.268468);
    outer.emplace_back(-18.756296, -11.621022);
    outer.emplace_back(-19.153901, -10.959297);
    outer.emplace_back(-19.528169, -10.284099);
    outer.emplace_back(-19.878646, -9.596251);
    outer.emplace_back(-20.204903, -8.896590);
    outer.emplace_back(-20.506543, -8.185969);
    outer.emplace_back(-20.783200, -7.465254);
    outer.emplace_back(-21.034536, -6.735322);
    outer.emplace_back(-21.260244, -5.997064);
    outer.emplace_back(-21.460050, -5.251379);
    outer.emplace_back(-21.633710, -4.499174);
    outer.emplace_back(-21.781012, -3.741368);
    outer.emplace_back(-21.901778, -2.978882);
    outer.emplace_back(-21.995860, -2.212645);
    outer.emplace_back(-22.063144, -1.443593);
    outer.emplace_back(-22.103547, -0.672660);
    outer.emplace_back(-22.117020, 0.099213);
    outer.emplace_back(-22.103547, 0.871085);
    outer.emplace_back(-22.063144, 1.642018);
    outer.emplace_back(-21.995860, 2.411071);
    outer.emplace_back(-21.901778, 3.177307);
    outer.emplace_back(-21.781012, 3.939793);
    outer.emplace_back(-21.633710, 4.697600);
    outer.emplace_back(-21.460050, 5.449804);
    outer.emplace_back(-21.260244, 6.195489);
    outer.emplace_back(-21.034536, 6.933748);
    outer.emplace_back(-20.783200, 7.663679);
    outer.emplace_back(-20.506543, 8.384394);
    outer.emplace_back(-20.204903, 9.095015);
    outer.emplace_back(-19.878646, 9.794676);
    outer.emplace_back(-19.528169, 10.482524);
    outer.emplace_back(-19.153901, 11.157722);
    outer.emplace_back(-18.756296, 11.819447);
    outer.emplace_back(-18.335840, 12.466893);
    outer.emplace_back(-17.893045, 13.099271);
    outer.emplace_back(-17.428449, 13.715810);
    outer.emplace_back(-16.942620, 14.315759);
    outer.emplace_back(-16.436149, 14.898387);
    outer.emplace_back(-15.909652, 15.462985);
    outer.emplace_back(-15.363773, 16.008865);
    outer.emplace_back(-14.799175, 16.535361);
    outer.emplace_back(-14.216546, 17.041833);
    outer.emplace_back(-13.616597, 17.527662);
    outer.emplace_back(-13.000058, 17.992257);
    outer.emplace_back(-12.367680, 18.435053);
    outer.emplace_back(-11.720235, 18.855509);
    outer.emplace_back(-11.058510, 19.253113);
    outer.emplace_back(-10.383312, 19.627382);
    outer.emplace_back(-9.695463, 19.977858);
    outer.emplace_back(-8.995802, 20.304115);
    outer.emplace_back(-8.285181, 20.605756);
    outer.emplace_back(-7.564466, 20.882413);
    outer.emplace_back(-6.834535, 21.133748);
    outer.emplace_back(-6.096277, 21.359456);
    outer.emplace_back(-5.350591, 21.559262);
    outer.emplace_back(-4.598387, 21.732922);
    outer.emplace_back(-3.840580, 21.880225);
    outer.emplace_back(-3.078094, 22.000991);
    outer.emplace_back(-2.311858, 22.095073);
    outer.emplace_back(-1.542805, 22.162356);
    outer.emplace_back(-0.771873, 22.202759);
    outer.emplace_back(0.000000, 22.216232);
    outer.emplace_back(0.771873, 22.202759);
    outer.emplace_back(1.542805, 22.162356);
    outer.emplace_back(2.311858, 22.095073);
    outer.emplace_back(3.078094, 22.000991);
    outer.emplace_back(3.840580, 21.880225);
    outer.emplace_back(4.598387, 21.732922);
    outer.emplace_back(5.350591, 21.559262);
    outer.emplace_back(6.096277, 21.359456);
    outer.emplace_back(6.834535, 21.133748);
    outer.emplace_back(7.564466, 20.882413);
    outer.emplace_back(8.285181, 20.605756);
    outer.emplace_back(8.995802, 20.304115);
    outer.emplace_back(9.695463, 19.977858);
    outer.emplace_back(10.383312, 19.627382);
    outer.emplace_back(11.058510, 19.253113);
    outer.emplace_back(11.720235, 18.855509);
    outer.emplace_back(12.367680, 18.435053);
    outer.emplace_back(13.000058, 17.992257);
    outer.emplace_back(13.616597, 17.527662);
    outer.emplace_back(14.216546, 17.041833);
    outer.emplace_back(14.799175, 16.535361);
    outer.emplace_back(15.363773, 16.008865);
    outer.emplace_back(15.909652, 15.462985);
    outer.emplace_back(16.303505, 15.040630);
    outer.emplace_back(27.547087, 15.040630);
    outer.emplace_back(27.547087, -14.025827);

    //        outer.emplace_back(-100, 100);
    //        outer.emplace_back(100, 100);
    //        outer.emplace_back(100, -100);
    //        outer.emplace_back(-100, -100);

    //    outer.emplace_back(-200, 200);
    //    outer.emplace_back(0, 100);
    //    outer.emplace_back(200, 200);
    //    outer.emplace_back(200, -100);
    //    outer.emplace_back(-200, -100);

    //    outer.emplace_back(-200, 0);
    //    outer.emplace_back(-100, 200);
    //    outer.emplace_back(0, 100);
    //    outer.emplace_back(200, 200);
    //    outer.emplace_back(200, 0);
    //    outer.emplace_back(100, -150);
    //    outer.emplace_back(-100, -150);

    //    std::cout << "************* add polygon *************" << std::endl;
    for (int i = 0; i < 5; ++i) {
        auto tmpOuter = outer;
        geometry::polygonOffset(tmpOuter, i, true);
        //        auto res = geometry::polygonOffset(tmpOuter, -(i + 1) * 10, m_scene.get());
        //        if (!res.empty()) {
        //            for (auto pt : res[0]) {
        //                std::cout << "pt: (" << pt.x << "," << pt.y << ")" << std::endl;
        //            }
        addPolygon(geometry::PolygonWithHoles(tmpOuter, {}));
        //        }
    }
}

void QtLaboratory::addPolygon(const geometry::PolygonWithHoles& pwh) {
    geometry::PolygonWithHoles poly;
    poly.setOuter(pwh.outer());
    auto* polygon = new PolygonItem(poly);
    m_scene->addItem(polygon);
}

/* endregion */
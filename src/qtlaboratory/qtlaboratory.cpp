#include "qtlaboratory.h"
#include "labscene.h"
#include "labview.h"
#include "ui_qtlaboratory.h"

#include <QGraphicsRectItem>
#include <vector>

/* region Constructors / Destructor */
QtLaboratory::QtLaboratory(QWidget* parent) :
    QDialog(parent),
    _ui(new Ui::QtLabDlg),
    _scene(new LabScene()) {
    initScene();
    initConnects();
}

QtLaboratory::~QtLaboratory() = default;
/* endregion */

/* region Private Methods */
void QtLaboratory::initScene() {
    _ui->setupUi(this);
    auto layout = new QVBoxLayout();
    auto view = new LabView(this);
    view->setScene(_scene.get());
    layout->addWidget(view);
    _ui->mainWgt->setLayout(layout);
}

void QtLaboratory::initConnects() {
    connect(_ui->trianlgeBtn, SIGNAL(clicked()), this, SLOT(triangleBtnClicked()));
    connect(_ui->rectBtn, SIGNAL(clicked()), this, SLOT(rectBtnClicked()));
    connect(_ui->polygonBtn, SIGNAL(clicked()), this, SLOT(polygonBtnClicked()));
}
/* endregion */

/* region Slots Methods */
void QtLaboratory::triangleBtnClicked() {
    QPen pen(Qt::cyan, 0);
    QBrush brush(Qt::NoBrush);
    auto* item = new QGraphicsPolygonItem();
    item->setPen(pen);
    item->setBrush(brush);
    QPolygonF polygon;
    polygon << QPointF(-180, 100) << QPointF(120, 120) << QPointF(0, -140);
    item->setPolygon(polygon);
    _scene->addItem(item);
}

void QtLaboratory::rectBtnClicked() {
    QPen pen(Qt::cyan, 0);
    QBrush brush(Qt::NoBrush);
    auto* item = new QGraphicsRectItem();
    item->setPen(pen);
    item->setBrush(brush);
    item->setRect(QRectF(-100, -100, 200, 200));
    _scene->addItem(item);
}

void QtLaboratory::polygonBtnClicked() {
    QPen pen(Qt::cyan, 0);
    QBrush brush(Qt::NoBrush);
    auto* item = new QGraphicsPolygonItem();
    item->setPen(pen);
    item->setBrush(brush);
    QPolygonF polygon;
    polygon << QPointF(-200, 60) << QPointF(-40, 20) << QPointF(40, 100) << QPointF(100, 140) << QPointF(160, 120)
            << QPointF(240, 100) << QPointF(280, 40) << QPointF(260, -40) << QPointF(200, -40) << QPointF(160, -20)
            << QPointF(120, 20) << QPointF(80, -20) << QPointF(60, -60) << QPointF(0, -80) << QPointF(-80, -60)
            << QPointF(-140, -40) << QPointF(-180, 0);
    item->setPolygon(polygon);
    _scene->addItem(item);
}

/* endregion */
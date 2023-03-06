#include "qtlaboratory.h"
#include "geometryitem.h"
#include "labscene.h"
#include "ui_qtlaboratory.h"

#include <QDebug>
#include <QGraphicsItem>
#include <QGraphicsView>
#include <QTableView>

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
    //    m_scene->addItem(item);
}

void QtLaboratory::comPolyBtnClicked() {
    //    m_scene->addItem(item);
}

void QtLaboratory::mergeBtnClicked() {
    //    auto         items = m_scene->items();
    //    QPainterPath path;
    //    for (auto* i : items) {
    //        path += i->mapToScene(i->shape());
    //        m_scene->removeItem(i);
    //    }
    //    m_scene->addPath(path, QPen(Qt::yellow), QBrush(Qt::blue));
}

void QtLaboratory::overlapsBtnClicked() {
    //    QPolygonF polygon;
    //    polygon << QPointF(-150, -150) << QPointF(150, -150) << QPointF(150, 150) << QPointF(-150, 150) <<
    //    QPointF(-150, -150); auto* rect = new DiagramItem(DiagramItem::DiagramType::Rectangle);
    //    rect->setPolygon(polygon);
    //    auto* circle = new DiagramItem(DiagramItem::DiagramType::Circle);
    //    m_scene->addItem(rect);
    //    m_scene->addItem(circle);
    //    auto  path     = rect->shape() - circle->shape();
    //    auto* pathItem = new QGraphicsPathItem(path);
    //    pathItem->setBrush(QBrush(Qt::gray));
    //    pathItem->setPen(Qt::NoPen);
    //    pathItem->setFlags(QGraphicsItem::GraphicsItemFlag::ItemIsSelectable |
    //    QGraphicsItem::GraphicsItemFlag::ItemIsMovable); m_scene->addItem(pathItem);
}

void QtLaboratory::clearBtnClicked() {
    for (auto* i : m_scene->items()) {
        m_scene->removeItem(i);
    }
}
/* endregion */
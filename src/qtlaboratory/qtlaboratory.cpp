#include "qtlaboratory.h"
#include "ui_qtlaboratory.h"
#include "diagramitem.h"
#include "profilemodel.cpp"

#include <QDebug>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QTableView>
#include <QTabWidget>
#include <iostream>

const QColor DEFAULT_CR = QColor(Qt::white);

/* region Constructors / Destructor */
QtLaboratory::QtLaboratory(QWidget *parent)
        : QDialog(parent), m_ui(new Ui::QtLabDlg), m_scene(new QGraphicsScene()) {
    initScene();
    initConnects();
}

QtLaboratory::~QtLaboratory() = default;
/* endregion */

/* region Private Methods */
void QtLaboratory::initScene() {
    m_ui->setupUi(this);
    auto layout = new QVBoxLayout();
    auto view = new QGraphicsView(m_scene.get(), this);
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
    connect(m_ui->layerProfileBtn, SIGNAL(clicked()), this, SLOT(layerProfileBtnClicked()));
}

/* region Slots Methods */
void QtLaboratory::rectBtnClicked() {
    auto *item = new DiagramItem(DiagramItem::DiagramType::Rectangle);
    m_scene->addItem(item);
}

void QtLaboratory::roundRectBtnClicked() {
    auto *item = new DiagramItem(DiagramItem::DiagramType::RoundRect);
    m_scene->addItem(item);
}

void QtLaboratory::circleBtnClicked() {
    auto *item = new DiagramItem(DiagramItem::DiagramType::Circle);
    m_scene->addItem(item);
}

void QtLaboratory::comPolyBtnClicked() {
    auto *item = new DiagramItem(DiagramItem::DiagramType::Polygon);
    m_scene->addItem(item);
}

void QtLaboratory::mergeBtnClicked() {
    auto items = m_scene->items();
    QPainterPath path;
    for (auto *i: items) {
        path += i->mapToScene(i->shape());
        m_scene->removeItem(i);
    }
    m_scene->addPath(path, QPen(Qt::yellow), QBrush(Qt::blue));
}

void QtLaboratory::overlapsBtnClicked() {
    QPolygonF polygon;
    polygon << QPointF(-150, -150) << QPointF(150, -150)
            << QPointF(150, 150) << QPointF(-150, 150)
            << QPointF(-150, -150);
    auto *rect = new DiagramItem(DiagramItem::DiagramType::Rectangle);
    rect->setPolygon(polygon);
    auto *circle = new DiagramItem(DiagramItem::DiagramType::Circle);
    m_scene->addItem(rect);
    m_scene->addItem(circle);
    m_scene->addPath(rect->shape() - circle->shape(), QPen(Qt::yellow), QBrush(Qt::blue));
}

void QtLaboratory::clearBtnClicked() {
    for (auto *i: m_scene->items()) {
        m_scene->removeItem(i);
    }
}

void QtLaboratory::layerProfileBtnClicked() {
    auto tab_widget = new QTabWidget();
    auto tab1 = new QWidget();
    auto tab2 = new QWidget();
    auto tab3 = new QWidget();
    auto layout1 = new QVBoxLayout();
    auto layout2 = new QVBoxLayout();
    auto layout3 = new QVBoxLayout();

    auto model = new MyTableModel();
    auto view1 = new QTableView();
    auto view2 = new QTableView();
    auto view3 = new QTableView();

    view1->setModel(model);
    view2->setModel(model);
    view3->setModel(model);

    layout1->addWidget(view1);
    layout2->addWidget(view2);
    layout3->addWidget(view3);

    tab1->setLayout(layout1);
    tab2->setLayout(layout2);
    tab3->setLayout(layout3);

    tab_widget->addTab(tab1, tr("CONDUCTOR"));
    tab_widget->addTab(tab2, tr("DIELECTRIC"));
    tab_widget->addTab(tab3, tr("PLANE"));
    tab_widget->show();
    this->reject();
}
/* endregion */
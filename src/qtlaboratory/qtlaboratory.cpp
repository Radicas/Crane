#include "qtlaboratory.h"
#include "ui_qtlaboratory.h"

#include <QDebug>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QMessageBox>
#include <iostream>

const QColor DEFAULT_CR = QColor(Qt::white);
/* region Constructors / Destructor */
QtLaboratory::QtLaboratory(QWidget *parent)
    : QDialog(parent), m_ui(new Ui::QtLabDlg), m_scene(new QGraphicsScene())
{
  initScene();
  initConnects();
}
QtLaboratory::~QtLaboratory() = default;
/* endregion */

/* region Slots Methods */
void QtLaboratory::rectBtnClicked()
{
  QPolygonF polygon;
  polygon << QPointF(-100, -100)
          << QPointF(100, -100)
          << QPointF(100, 100)
          << QPointF(-100, 100)
          << QPointF(-100, -100);
  auto *poly_item = new QGraphicsPolygonItem(polygon);
  poly_item->moveBy(-100, -100);
  poly_item->setPen(QPen(DEFAULT_CR));
  m_scene->addItem(poly_item);
}
void QtLaboratory::roundRectBtnClicked()
{
  QPolygonF polygon;
  QPainterPath path;
  path.moveTo(200, 50);
  path.arcTo(150, 0, 50, 50, 0, 90);
  path.arcTo(50, 0, 50, 50, 90, 90);
  path.arcTo(50, 50, 50, 50, 180, 90);
  path.arcTo(150, 50, 50, 50, 270, 90);
  path.lineTo(200, 25);
  polygon = path.toFillPolygon();
  auto *poly_item = new QGraphicsPolygonItem(polygon);
  poly_item->setPen(QPen(DEFAULT_CR));
  m_scene->addItem(poly_item);
}
void QtLaboratory::circleBtnClicked()
{
  QPolygonF polygon;
  QPainterPath path;
  path.moveTo(100, 0);
  path.arcTo(-100, -100, 200, 200, 0, 360);
  polygon = path.toFillPolygon();
  auto *poly_item = new QGraphicsPolygonItem(polygon);
  poly_item->setPen(QPen(DEFAULT_CR));
  m_scene->addItem(poly_item);
}
void QtLaboratory::simPolyBtnClicked()
{
  QPolygonF polygon;
  polygon << QPointF(-200, 0)
          << QPointF(-75, -75)
          << QPointF(-25, -25)
          << QPointF(25, -75)
          << QPointF(45, 30)
          << QPointF(-25, 75)
          << QPointF(-200, 0);
  auto *poly_item = new QGraphicsPolygonItem(polygon);
  poly_item->setPen(QPen(DEFAULT_CR));
  m_scene->addItem(poly_item);
}
void QtLaboratory::comPolyBtnClicked() {}
void QtLaboratory::mergeBtnClicked()
{
  auto items = m_scene->items();
  auto i1 = dynamic_cast<QGraphicsPolygonItem *>(items[0]);
  auto i2 = dynamic_cast<QGraphicsPolygonItem *>(items[1]);

  qDebug() << "i1: " << i1->polygon();
  qDebug() << "i2: " << i2->polygon();

  auto i3path = i1->mapToScene(i1->shape()) + i2->mapToScene(i2->shape());
  m_scene->addPath(i3path, DEFAULT_CR);
  m_scene->removeItem(i1);
  m_scene->removeItem(i2);
}
/* endregion */

/* region Private Methods */
void QtLaboratory::initScene()
{
  m_ui->setupUi(this);
  auto layout = new QVBoxLayout();
  auto view = new QGraphicsView(m_scene.get(), this);
  view->setBackgroundBrush(QBrush(Qt::black));
  layout->addWidget(view);
  m_ui->mainWgt->setLayout(layout);
}
void QtLaboratory::initConnects()
{
  connect(m_ui->rectBtn, SIGNAL(clicked()), this, SLOT(rectBtnClicked()));
  connect(m_ui->roundRectBtn, SIGNAL(clicked()), this, SLOT(roundRectBtnClicked()));
  connect(m_ui->circleBtn, SIGNAL(clicked()), this, SLOT(circleBtnClicked()));
  connect(m_ui->simPolyBtn, SIGNAL(clicked()), this, SLOT(simPolyBtnClicked()));
  connect(m_ui->comPolyBtn, SIGNAL(clicked()), this, SLOT(comPolyBtnClicked()));
  connect(m_ui->mergeBtn, SIGNAL(clicked()), this, SLOT(mergeBtnClicked()));
}
/* endregion */

#include "qtlaboratory.h"
#include "ui_qtlaboratory.h"
#include "diagramitem.h"

#include <QGraphicsView>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QMessageBox>
#include <QDebug>
#include <iostream>

QtLaboratory::QtLaboratory(QWidget *parent)
    : QDialog(parent), m_ui(new Ui::QtLabDlg), m_scene(new QGraphicsScene())
{
    initScene();
    initConnects();
}

QtLaboratory::~QtLaboratory() = default;

void QtLaboratory::initConnects()
{
}

void QtLaboratory::initScene()
{
    m_ui->setupUi(this);
    auto layout = new QVBoxLayout();
    auto view = new QGraphicsView(m_scene.get(), this);
    layout->addWidget(view);
    m_ui->mainWgt->setLayout(layout);
    connect(m_ui->geometryBtn, SIGNAL(clicked()), this, SLOT(geoBtnClicked()));
    connect(m_ui->mergeBtn, SIGNAL(clicked()), this, SLOT(mergeBtnClicked()));
}

void QtLaboratory::geoBtnClicked()
{
    auto *i1 = new DiagramItem(DiagramItem::DiagramType::StartEnd, nullptr);
    auto *i2 = new DiagramItem(DiagramItem::DiagramType::Conditional, nullptr);
    auto *i3 = new DiagramItem(DiagramItem::DiagramType::Io, nullptr);
    auto *i4 = new DiagramItem(DiagramItem::DiagramType::Step, nullptr);
    auto *i5 = new DiagramItem(DiagramItem::DiagramType::Circle, nullptr);
    auto *i6 = new DiagramItem(DiagramItem::DiagramType::Step, nullptr);
    // m_scene->addItem(i1);
    // m_scene->addItem(i2);
    // m_scene->addItem(i3);
    // m_scene->addItem(i4);
    // m_scene->addItem(i5);
    // m_scene->addItem(i6);

    // i5->moveBy(-100,-100);
    // m_scene->addItem(i5);
    // m_scene->addItem(i5);

    i5->setPen(Qt::NoPen);
    i5->setBrush(QBrush(Qt::blue));
    m_scene->addItem(i5);
}

void QtLaboratory::mergeBtnClicked()
{
    auto items = m_scene->items();
    auto i1 = dynamic_cast<QGraphicsPolygonItem *>(items[0]);
    auto i2 = dynamic_cast<QGraphicsPolygonItem *>(items[1]);

    qDebug() << "i1: " << i1->polygon();
    qDebug() << "i2: " << i2->polygon();

    auto i3path = i1->mapToScene(i1->shape()) + i2->mapToScene(i2->shape());
    m_scene->addPath(i3path, QPen(Qt::NoPen), QBrush(Qt::blue));
    m_scene->removeItem(i1);
    m_scene->removeItem(i2);
}
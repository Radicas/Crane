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
    connect(m_ui->geometryBtn, QPushButton::clicked, this, QtLaboratory::geoBtnClicked);
    connect(m_ui->mergeBtn, QPushButton::clicked, this, QtLaboratory::mergeBtnClicked);
}

void QtLaboratory::geoBtnClicked()
{
    auto *i1 = new DiagramItem(DiagramItem::DiagramType::StartEnd, nullptr);
    auto *i2 = new DiagramItem(DiagramItem::DiagramType::Conditional, nullptr);
    auto *i3 = new DiagramItem(DiagramItem::DiagramType::Io, nullptr);
    auto *i4 = new DiagramItem(DiagramItem::DiagramType::Step, nullptr);
    auto* i5 = new DiagramItem(DiagramItem::DiagramType::Circle, nullptr);
    // m_scene->addItem(i1);
    // m_scene->addItem(i2);
    // m_scene->addItem(i3);
    m_scene->addItem(i4);
    m_scene->addItem(i5);
}

void QtLaboratory::mergeBtnClicked() {
    for (auto* i : m_scene->items()) {
        qDebug() << i->shape();
    }
}
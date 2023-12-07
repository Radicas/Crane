#include "laboratorydialog.h"

#include "ui_qtlaboratory.h"
#include "labscene.h"
#include "labview.h"

#include <QGraphicsPathItem>
#include <memory>
/* region Constructors / Destructor */
LaboratoryDialog::LaboratoryDialog(QWidget* parent) :
    QDialog(parent),
    _ui(nullptr),
    _scene(nullptr) {
    initObjects();
    initScene();
    initConnections();
}
LaboratoryDialog::~LaboratoryDialog() = default;
/* endregion */

/* region Private Methods */
void LaboratoryDialog::initObjects() {
    _ui = std::make_unique<Ui::QtLabDlg>();
    _scene = std::make_unique<LabScene>();
}

void LaboratoryDialog::initScene() {
    _ui->setupUi(this);
    auto* layout = new QVBoxLayout();
    auto* view = new LabView(this);
    view->setScene(_scene.get());
    layout->addWidget(view);
    _ui->mainWgt->setLayout(layout);
    setModal(true);
    setWindowTitle(tr("Laboratory"));
    showMaximized(); // 最大化(区别于全屏)
}

void LaboratoryDialog::initConnections() {
    auto* item = new QGraphicsPathItem();
    QPainterPath path;
    path.moveTo(QPointF(0, 0));
    path.lineTo(QPointF(100, 0));
    path.lineTo(QPointF(100, 100));
    path.lineTo(QPointF(200, 0));
    path.lineTo(QPointF(200, 200));
    path.lineTo(QPointF(300, 0));
    path.lineTo(QPointF(300, 300));
    path.closeSubpath();
    QPen pen(Qt::red, 0);
    QBrush brush(Qt::blue);
    item->setPen(pen);
    item->setBrush(brush);
    item->setPath(path);
    _scene->addItem(item);
}
/* endregion */
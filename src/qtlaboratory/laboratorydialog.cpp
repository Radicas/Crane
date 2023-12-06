#include "laboratorydialog.h"

#include "ui_qtlaboratory.h"
#include "labscene.h"
#include "labview.h"

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
    auto layout = new QVBoxLayout();
    auto view = new LabView(this);
    view->setScene(_scene.get());
    layout->addWidget(view);
    _ui->mainWgt->setLayout(layout);
}

void LaboratoryDialog::initConnections() {
}
/* endregion */
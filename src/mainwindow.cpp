#include "mainwindow.h"

#include "cranegpt/cranegptdialog.h"
#include "pycheck/pyenvdialog.h"
#include "qtlaboratory/qtlaboratory.h"
#include "ui_mainwindow.h"

#include <iostream>

/* region Constructors / Destructor */
MainWindow::MainWindow(QWidget* parent) :
    QMainWindow(parent),
    ui(new Ui::Crane) {
    ui->setupUi(this);
    initComponents();
}

MainWindow::~MainWindow() = default;
/* endregion */

/* region Private Methods */
void MainWindow::initComponents() {
    connect(ui->actionAbout, SIGNAL(triggered()), this, SLOT(aboutTriggered()));
    connect(ui->actionExit, SIGNAL(triggered()), this, SLOT(exitTriggered()));
    connect(ui->pyEnvBtn, SIGNAL(clicked()), this, SLOT(pyEnvBtnClicked()));
    connect(ui->qtLabBtn, SIGNAL(clicked()), this, SLOT(qtLabBtnClicked()));
    connect(ui->craneGPTBtn, SIGNAL(clicked()), this, SLOT(craneGPTBtnClicked()));
}
/* endregion */

/* region Slots Methods */
void MainWindow::pyEnvBtnClicked() {
    std::unique_ptr<PyEnvDialog> pyenv_dlg(new PyEnvDialog());
    pyenv_dlg->exec();
}
void MainWindow::aboutTriggered() {
}
void MainWindow::exitTriggered() {
    this->close();
}

void MainWindow::qtLabBtnClicked() {
    std::unique_ptr<QtLaboratory> ql_dlg(new QtLaboratory(this));
    ql_dlg->setMinimumSize(1000, 618);
    ql_dlg->exec();
}

void MainWindow::craneGPTBtnClicked() {
    std::unique_ptr<CraneGPTDialog> crane_gpt_dlg(new CraneGPTDialog());
    crane_gpt_dlg->exec();
}
/* endregion */
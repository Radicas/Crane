#include "mainwindow.h"

#include "pycheck/pyenvdialog.h"
#include "ui_mainwindow.h"
#include "utils/visual_utils.h"
#include "qtlaboratory/laboratorydialog.h"

#include <iostream>
#include <QApplication>
#include <QScreen>
#include <QDesktopWidget>

/* region Constructors / Destructor */
MainWindow::MainWindow(QWidget* parent) :
    QMainWindow(parent),
    _ui(new Ui::Crane) {
    setUpDialog();
    initComponents();
}

MainWindow::~MainWindow() = default;
/* endregion */

/* region Private Methods */
void MainWindow::setUpDialog() {
    _ui->setupUi(this);
    setBaseSize(309, 500);
    Utils::showCenteredWindow(this);
}

void MainWindow::initComponents() {
    connect(_ui->actionAbout, SIGNAL(triggered()), this, SLOT(aboutTriggered()));
    connect(_ui->actionExit, SIGNAL(triggered()), this, SLOT(exitTriggered()));
    connect(_ui->pyEnvBtn, SIGNAL(clicked()), this, SLOT(pyEnvBtnClicked()));

    _ui->pyEnvBtn->setVisible(false); // 禁用pyenv显示

    connect(_ui->qtLabBtn, SIGNAL(clicked()), this, SLOT(qtLabBtnClicked()));
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
    std::unique_ptr<LaboratoryDialog> ql_dlg(new LaboratoryDialog(this));
    ql_dlg->setBaseSize(1600, 900);
    Utils::showCenteredWindow(this);
    ql_dlg->exec();
}
/* endregion */
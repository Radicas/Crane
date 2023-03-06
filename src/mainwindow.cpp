#include "mainwindow.h"

#include "pycheck/pyenvdialog.h"
#include "qtlaboratory/qtlaboratory.h"
#include "ui_mainwindow.h"

#include <QMessageBox>
#include <iostream>

/* region Constructors / Destructor */
MainWindow::MainWindow(QWidget* parent): QMainWindow(parent), ui(new Ui::Scarab) {
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
}
/* endregion */

/* region Slots Methods */
void MainWindow::pyEnvBtnClicked() {
    std::unique_ptr< PyEnvDialog > pyenvdlg(new PyEnvDialog());
    pyenvdlg->exec();
}
void MainWindow::aboutTriggered() {
    QMessageBox::about(this, tr("About"), tr("ttt!"));
}
void MainWindow::exitTriggered() {
    this->close();
}
void MainWindow::qtLabBtnClicked() {
    std::unique_ptr< QtLaboratory > qlDlg(new QtLaboratory(this));
    qlDlg->exec();
}
/* endregion */
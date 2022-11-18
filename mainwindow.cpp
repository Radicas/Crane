#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "pyenvdialog.h"

#include <QMessageBox>
#include <iostream>
/* region Constructors / Destructor */
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::Scarab), pyenvdlg(new PyEnvDialog)
{
    ui->setupUi(this);
    initComponents();
}

MainWindow::~MainWindow() = default;
/* endregion */

/* region Private Methods */
void MainWindow::initComponents()
{
    connect(ui->actionAbout, SIGNAL(triggered()), this, SLOT(aboutTriggered()));
    connect(ui->actionExit, SIGNAL(triggered()), this, SLOT(exitTriggered()));
    connect(ui->pyEnvBtn, SIGNAL(clicked()), this, SLOT(pyEnvBtnClicked()));
}
/* endregion */

/* region Slots Methods */
void MainWindow::pyEnvBtnClicked()
{
    std::cout << "/* message */" << std::endl;
    pyenvdlg->exec();
}
void MainWindow::aboutTriggered() { QMessageBox::about(this, tr("About"), tr("圣甲虫是一个定制化的软件工具包")); }
void MainWindow::exitTriggered() { this->close(); }
/* endregion */
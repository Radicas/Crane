#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "pyenvdialog.h"

#include <QMessageBox>

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
    connect(ui->pyEnvBtn, QPushButton::clicked, this, [this]()
            { pyenvdlg->exec(); });
    connect(ui->actionAbout, QAction::triggered, this,
            [this]()
            { QMessageBox::about(this, tr("About"), tr("圣甲虫是一个定制化的软件工具包")); });
    connect(ui->actionExit, QAction::triggered, this, [this]()
            { this->close(); });
}
/* endregion */
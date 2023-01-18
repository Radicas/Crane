#include "mainwindow.h"
#include "src/pycheck/pyenvdialog.h"
#include "src/qtlaboratory/qtlaboratory.h"
#include "ui_mainwindow.h"

#include <QMessageBox>
#include <iostream>
/* region Constructors / Destructor */
MainWindow::MainWindow( QWidget* parent ) : QMainWindow( parent ), ui( new Ui::Scarab ) {
    ui->setupUi( this );
    initComponents();
}

MainWindow::~MainWindow() = default;
/* endregion */

/* region Private Methods */
void MainWindow::initComponents() {
    connect( ui->actionAbout, SIGNAL( triggered() ), this, SLOT( aboutTriggered() ) );
    connect( ui->actionExit, SIGNAL( triggered() ), this, SLOT( exitTriggered() ) );
    connect( ui->pyEnvBtn, SIGNAL( clicked() ), this, SLOT( pyEnvBtnClicked() ) );
    connect( ui->wtoEat, SIGNAL( clicked() ), this, SLOT( wToEatBtnClicked() ) );
    connect( ui->qtLabBtn, SIGNAL( clicked() ), this, SLOT( qtLabBtnClicked() ) );
}
/* endregion */

/* region Slots Methods */
void MainWindow::pyEnvBtnClicked() {
    std::unique_ptr< PyEnvDialog > pyenvdlg( new PyEnvDialog() );
    pyenvdlg->exec();
}
void MainWindow::wToEatBtnClicked() {}
void MainWindow::aboutTriggered() {
    QMessageBox::about( this, tr( "About" ), tr( "圣甲虫是个好用的软件!" ) );
}
void MainWindow::exitTriggered() {
    this->close();
}
void MainWindow::qtLabBtnClicked() {
    std::unique_ptr< QtLaboratory > qlDlg( new QtLaboratory( this ) );
    qlDlg->exec();
}
/* endregion */
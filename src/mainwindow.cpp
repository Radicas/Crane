#include "mainwindow.h"
#include "generic/geometry_algo.hpp"
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
void MainWindow::wToEatBtnClicked() {
    using namespace geometry;
    double r1;
    double r2;
    double angle;
    POINT  p0_0( 0, 0 );
    POINT  p0_1( 0, 1 );
    POINT  p1_0( 1, 0 );
    POINT  p1_1( 1, 1 );
    POINT  p1_2( 1, 2 );
    POINT  p2_2( 2, 2 );
    POINT  p3_2( 3, 2 );
    POINT  p3_4( 3, 4 );
    POINT  p5_3( 5, 3 );

    /* test for tri2circle */
    // auto   center1 = trip2circle( POINT( 1, 1 ), POINT( 1, 2 ), POINT( 3, 4 ), r1 );
    // auto   center2 = trip2circle( POINT( -1, 1 ), POINT( -2, 4 ), POINT( -5, 3 ), r2 );
    // std::cout << "center1: "
    //           << "(" << center1.x << "," << center1.y << ")" << std::endl;
    // std::cout << "radius1: " << r1 << std::endl;

    // std::cout << "center2: "
    //           << "(" << center2.x << "," << center2.y << ")" << std::endl;
    // std::cout << "radius2: " << r2 << std::endl;

    /* test for included angle */
    // double an = includedAngle( p1_1, p2_2, angle );
    // std::cout << "included angle: " << angle << std::endl;

    /* test for sweep angle */

    double dotmulti = dotmultiply( p0_1, p1_1, p0_0 );
    std::cout << "dotmultiply: " << dotmulti << std::endl;

    double mo = g_module( p0_1, p1_1 );
    std::cout << "module: " << mo << std::endl;

    double sw1 = sweepAngle( p0_1, p1_1, p0_0 );
    std::cout << "sw1: " << sw1 << std::endl;

    double sw2 = sweepAngle( p0_1, p1_0, p0_0 );
    std::cout << "sw2: " << sw2 << std::endl;
}
void MainWindow::aboutTriggered() {
    QMessageBox::about( this, tr( "About" ), tr( "ttt!" ) );
}
void MainWindow::exitTriggered() {
    this->close();
}
void MainWindow::qtLabBtnClicked() {
    std::unique_ptr< QtLaboratory > qlDlg( new QtLaboratory( this ) );
    qlDlg->exec();
}
/* endregion */
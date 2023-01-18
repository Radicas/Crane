#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "src/pycheck/pyenvdialog.h"

#include <QMainWindow>
#include <memory.h>

QT_BEGIN_NAMESPACE
namespace Ui {
class Scarab;
}
QT_END_NAMESPACE
class PyEnvDialog;
class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow( QWidget* parent = nullptr );
    ~MainWindow();

private slots:
    void initComponents();
    void pyEnvBtnClicked();
    void wToEatBtnClicked();
    void aboutTriggered();
    void exitTriggered();
    void qtLabBtnClicked();

private:
    std::unique_ptr< Ui::Scarab > ui;
};
#endif  // MAINWINDOW_H

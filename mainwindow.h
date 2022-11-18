#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "pyenvdialog.h"

#include <QMainWindow>
#include <memory.h>

QT_BEGIN_NAMESPACE
namespace Ui
{
    class Scarab;
}
QT_END_NAMESPACE
class PyEnvDialog;
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    void initComponents();

private:
    std::unique_ptr<Ui::Scarab> ui;
    std::unique_ptr<PyEnvDialog> pyenvdlg;
};
#endif // MAINWINDOW_H

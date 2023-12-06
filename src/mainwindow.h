/**
 * @file mainwindow.h
 * @author Radica
 * @brief 主界面
 * @version 0.1
 * @date 2023-02-19
 *
 * @copyright Copyright (c) 2023
 *
 */
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <memory.h>

QT_BEGIN_NAMESPACE
namespace Ui {
class Crane;
}
QT_END_NAMESPACE
class PyEnvDialog;
class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    /* region Constructors / Destructor */
    explicit MainWindow(QWidget* parent = nullptr);

    ~MainWindow() override;
    /* endregion */

private slots:

    /* region Slots Methods */
    void initComponents();

    static void pyEnvBtnClicked();

    void aboutTriggered();

    void exitTriggered();

    void qtLabBtnClicked();
    /* endregion */

private:
    /* region Private Methods */
    void setUpDialog();
    /* endregion */

    /* data */
    std::unique_ptr<Ui::Crane> _ui;
};
#endif // MAINWINDOW_H

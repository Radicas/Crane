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
	class Scarab;
}
QT_END_NAMESPACE
class PyEnvDialog;
class MainWindow: public QMainWindow {
	Q_OBJECT

public:
	/* region Constructors / Destructor */
	MainWindow(QWidget* parent = nullptr);
	~MainWindow();
	/* endregion */

private slots:
	void initComponents();
	void pyEnvBtnClicked();
	void aboutTriggered();
	void exitTriggered();
	void qtLabBtnClicked();

private:
	/* data */
	std::unique_ptr< Ui::Scarab > ui;
};
#endif  // MAINWINDOW_H

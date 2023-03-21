/**
 * @file pyenvdialog.h
 * @author Radica
 * @brief python环境检查
 * @version 0.1
 * @date 2023-02-19
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#ifndef REDEDA_PYENVDIALOG_H
#define REDEDA_PYENVDIALOG_H

#include <QDialog>

QT_BEGIN_NAMESPACE
namespace Ui {
class PyEnvDlg;
}
QT_END_NAMESPACE

class PyEnvDialog : public QDialog {
    Q_OBJECT
public:
    PyEnvDialog( QWidget* parent = nullptr );

    ~PyEnvDialog();

private:
    void setUp();

private slots:
    void checked();

    void clearText();

private:
    /* data */
    std::unique_ptr< Ui::PyEnvDlg > ui;
};

#endif  // REDEDA_PYENVDIALOG_H
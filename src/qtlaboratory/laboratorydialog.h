/**
 * @file qtlaboratory.h
 * @author Radica
 * @brief
 * @version 0.1
 * @date 2023-02-19
 *
 * @copyright Copyright (c) 2023
 *
 */
#ifndef REDEDA_SRC_QTLABORATORY_LABORATORYDIALOG_H
#define REDEDA_SRC_QTLABORATORY_LABORATORYDIALOG_H

#include <QDialog>

QT_BEGIN_NAMESPACE
class QGraphicsScene;
QT_END_NAMESPACE

namespace Ui {
class QtLabDlg;
}

class LabScene;

class LaboratoryDialog : public QDialog {
public:
    /* region Constructors */
    LaboratoryDialog(QWidget* parent = nullptr);
    LaboratoryDialog(const LaboratoryDialog& other) = delete;
    LaboratoryDialog& operator=(const LaboratoryDialog& rhs) = delete;
    ~LaboratoryDialog();
    /* endregion */

private:
    /* region Private Methods */
    /**
     * @brief 
     * 
     */
    void initObjects();

    /**
     * @brief 
     * 
     */
    void initScene();

    /**
     * @brief 
     * 
     */
    void initConnections();
    /* endregion */

    std::unique_ptr<Ui::QtLabDlg> _ui; ///< ui
    std::unique_ptr<LabScene> _scene;  ///< 场景
};

#endif // REDEDA_SRC_QTLABORATORY_LABORATORYDIALOG_H

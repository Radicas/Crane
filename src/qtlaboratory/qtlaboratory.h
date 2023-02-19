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
#ifndef REDEDA_QTLABORATORY_H
#define REDEDA_QTLABORATORY_H

#include <QDialog>

QT_BEGIN_NAMESPACE
class QGraphicsScene;
QT_END_NAMESPACE

namespace Ui {
class QtLabDlg;
}

class QtLaboratory : public QDialog {
    Q_OBJECT
public:
    explicit QtLaboratory( QWidget* parent = nullptr );
    ~QtLaboratory() override;

private slots:
    void rectBtnClicked();
    void roundRectBtnClicked();
    void circleBtnClicked();
    void comPolyBtnClicked();
    void mergeBtnClicked();
    void overlapsBtnClicked();
    void clearBtnClicked();
    void layerProfileBtnClicked();

private:
    void initScene();
    void initConnects();

private:
    std::unique_ptr<Ui::QtLabDlg>   m_ui;
    std::unique_ptr<QGraphicsScene> m_scene;
};

#endif  // REDEDA_QTLABORATORY_H
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

namespace geometry {
class PolygonWithHoles;
}

namespace Ui {
class QtLabDlg;
}

class LabScene;
class QtLaboratory : public QDialog {
    Q_OBJECT

public:
    explicit QtLaboratory(QWidget* parent = nullptr);

    QtLaboratory(const QtLaboratory& rhs) = delete;

    ~QtLaboratory() override;

private slots:

    void triangleBtnClicked();

    void rectBtnClicked();

    void polygonBtnClicked();

private:
    void initScene();

    void initConnects();

private:
    std::unique_ptr<Ui::QtLabDlg> _ui; ///< ui
    std::unique_ptr<LabScene> _scene;  ///< 场景
};

#endif // REDEDA_QTLABORATORY_H
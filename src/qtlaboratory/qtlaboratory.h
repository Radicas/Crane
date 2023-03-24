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

class LabScene;
class QtLaboratory : public QDialog {
    Q_OBJECT
public:
    explicit QtLaboratory( QWidget* parent = nullptr );

    ~QtLaboratory() override;

private slots:
    /**
     *
     */
    void triangleBtnClicked();

    /**
     * @brief 矩形
     */
    void rectBtnClicked();

    /**
     * @brief 圆角矩形
     */
    void roundRectBtnClicked();

    /**
     * @brief 圆形
     */
    void circleBtnClicked();

    /**
     * @brief 贝塞尔曲线封闭图形
     */
    void curvedPolyBtnClicked();

    /**
     * @brief 交集
     */
    void intersectBtnClicked();

    /**
     * @brief 补集
     */
    void unionBtnClicked();

    /**
     * @brief 差集
     */
    void subtractBtnClicked();

    /**
     * @brief 路径转多边形绘制
     */
    void pathToPolygonBtnClicked();
    
    /**
     * @brief 清空屏幕
     */
    void clearBtnClicked();

private:
    void initScene();

    void initConnects();

private:
    std::unique_ptr<Ui::QtLabDlg> m_ui;    ///< ui
    std::unique_ptr<LabScene>     m_scene; ///< 场景
};

#endif  // REDEDA_QTLABORATORY_H
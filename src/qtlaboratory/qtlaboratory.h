#ifndef REDEDA_QTLABORATORY_H
#define REDEDA_QTLABORATORY_H

#include <QDialog>

namespace Ui
{
    class QtLabDlg;
}
class QGraphicsScene;
QT_END_NAMESPACE
class QtLaboratory : public QDialog
{
    Q_OBJECT
public:
    explicit QtLaboratory(QWidget *parent = nullptr);
    ~QtLaboratory() override;

private slots:
    void geoBtnClicked();
    void mergeBtnClicked();

private:
    void initScene();
    void initConnects();

private:
    std::unique_ptr<Ui::QtLabDlg> m_ui;
    std::unique_ptr<QGraphicsScene> m_scene;
};

#endif // REDEDA_QTLABORATORY_H
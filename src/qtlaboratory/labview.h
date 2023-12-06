#ifndef LABVIEW_H
#define LABVIEW_H

#include <QGraphicsView>

class LabView : public QGraphicsView {
    Q_OBJECT

public:
    LabView(QWidget* parent = nullptr);

protected:
    void mousePressEvent(QMouseEvent* event) override;

private:
    /* region Private Methods */
    void initView();
    /* endregion */
};

#endif // LABVIEW_H
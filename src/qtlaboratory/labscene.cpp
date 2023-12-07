#include "labscene.h"

#include <QGraphicsLineItem>
#include <QGraphicsSceneWheelEvent>
/* region Constructors / Destructor */
LabScene::LabScene(QObject* parent) :
    QGraphicsScene(parent) {
    initScene();
}
/* endregion */

/* region Private Methods */
void LabScene::initScene() {
    // setSceneRect(0, 0, 800, 600);
    setBackgroundBrush(Qt::black); // 设置背景颜色为浅灰色
    createGrid(1000, 1000, 50);
}

void LabScene::createGrid(int numRows, int numCols, int gridSpacing) {
    // 计算实际的网格尺寸
    int gridWidth = numCols * gridSpacing;
    int gridHeight = numRows * gridSpacing;

    QPen pen(Qt::white, 0);

    // 绘制垂直网格线
    for (int i = 0; i < numCols; i++) {
        int x = i * gridSpacing;
        QGraphicsLineItem* line = new QGraphicsLineItem(x, 0, x, gridHeight);
        line->setPen(pen);
        addItem(line);
    }

    // 绘制水平网格线
    for (int i = 0; i < numRows; i++) {
        int y = i * gridSpacing;
        QGraphicsLineItem* line = new QGraphicsLineItem(0, y, gridWidth, y);
        line->setPen(pen);
        addItem(line);
    }
}

/* endregion */
#ifndef LABSCENE_H
#define LABSCENE_H

#include <QGraphicsScene>

class LabScene : public QGraphicsScene {
    Q_OBJECT

public:
    /* region Constructors / Destructor */
    LabScene(QObject* parent = nullptr);
    /* endregion */

private:
    /* region Private Methods */
    void initScene();

    void createGrid(int numRows, int numCols, int gridSpacing);
    /* endregion */
};

#endif // LABSCENE_H
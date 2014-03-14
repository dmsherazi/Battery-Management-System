#include <QtGui/QApplication>

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QPointF>
#include <QVector>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QVector <QPointF> points;

    // Fill in points with n number of points
    for(int i = 0; i< 100; i++)
       points.append(QPointF(i*5, i*5));

    // Create a view, put a scene in it and add tiny circles
    // in the scene
    QGraphicsView * view = new QGraphicsView();
    QGraphicsScene * scene = new QGraphicsScene();
    view->setScene(scene);

    for(int i = 0; i< points.size(); i++)
        scene->addEllipse(points[i].x(), points[i].y(), 1, 1);

    // Show the view
    view->show();

    // or add the view to the layout inside another widget

    return a.exec();
}

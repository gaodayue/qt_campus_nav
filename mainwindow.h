#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include "graph.h"

#define NUM_NODES 24

namespace Ui {
class MainWindow;
}

class Node {
public:
    friend class MainWindow;
    Node(double longitude, double latitude, int x, int y):
        longitude(longitude), latitude(latitude), x(x), y(y) {}
private:
    double longitude;
    double latitude;
    int x;
    int y;
};

extern Node nodes[NUM_NODES];

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:

    void on_startButton_clicked();
    void on_endButton_clicked();


private:
    int map2x(double longitude);
    int map2y(double latitude);

    int nearestNode(double longitude, double latitude);
    void buildGraph();

    Ui::MainWindow *ui;
    QGraphicsScene *scene;
    QPen *pen;
    Graph *graph;
};

#endif // MAINWINDOW_H

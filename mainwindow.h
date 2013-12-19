#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QTouchEvent>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:

    void on_startButton_clicked();

    void on_endButton_clicked();
    std::list<std::pair<double, double> >   getWay(double lat, double lon);


private:
    Ui::MainWindow *ui;
    QGraphicsView * gv;
    QGraphicsScene * scene;
    QPen * pen;
    int startX;
    int startY;
};

#endif // MAINWINDOW_H

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "mapping.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // show images
    scene = new QGraphicsScene();
    scene->addPixmap(QPixmap(":/images/campus_flat.png"));
    //ui->graphicsView->resize(480, 272);
    //ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    //ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

    ui->graphicsView->setScene(scene);

    //ui->menuBar->addMenu("Menu 1");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_startButton_clicked()
{
    Mapping *m = new Mapping;
    // testing coordinate
    double lat = 39.9513795787521;
    double lon = 116.340297150787;
    int x = m->xPos(lon);
    int y = m->yPos(lat);
    startX = x;
    startY = y;

    // drawing point
    double rad = 2; // the point radius
    scene->addEllipse(x-rad, y-rad, rad*2.0, rad*2.0,
                QPen(), QBrush(Qt::green));
}

void MainWindow::on_endButton_clicked()
{
    Mapping *m = new Mapping;
    // testing coordinate
    double lat = 39.9510795787521;
    double lon = 116.340297150787;
    int x = m->xPos(lon);
    int y = m->yPos(lat);

    // drawing point
    double rad = 2; // the point radius
    scene->addEllipse(x-rad, y-rad, rad*2.0, rad*2.0,
                QPen(), QBrush(Qt::green));

    // start drawing line from start point to end point
    // direct line! need fix!
    scene->addLine(startX, startY, x, y);
}

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "mapping.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    pen = new QPen();
    pen->setBrush(Qt::green);
    pen->setWidth(3);

    // show images
    scene = new QGraphicsScene();
    //scene->addPixmap(QPixmap(":/images/campus_flat.png"));
    scene->addPixmap(QPixmap("/gps/images/campus_flat.png"));

    //gv = new QGraphicsView();
    //gv->resize(480, 213);
    //gv->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    //gv->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    //gv->setScene(scene);
    //ui->graphicsView->resize(480, 272);
    //ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    //ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

    ui->graphicsView->setScene(scene);
    //ui->menuBar->addMenu("Menu 1");
    //setCentralWidget(gv);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_startButton_clicked()
{
    Mapping *m = new Mapping;
    // testing coordinate
    double lat = 39.951445375634954;
    double lon = 116.33830158728031;
    int x = m->xPos(lon);
    int y = m->yPos(lat);
    startX = x;
    startY = y;

    // drawing point
    double rad = 2; // the point radius
    scene->addEllipse(x-rad, y-rad, rad*2.0, rad*2.0,
                QPen(), QBrush(Qt::green));
}

std::list<std::pair<double, double> >   MainWindow::getWay(double lat, double lon)
{
    std::list<std::pair<double, double> > way;
    std::pair<double, double> point = std::make_pair(39.951445375634954, 116.33830158728031);
    way.push_back(point);
    std::pair<double, double> point3 = std::make_pair(39.9512233108989, 116.33932082670597);
    way.push_back(point3);



    std::pair<double, double> point2 = std::make_pair(39.95102386325794, 116.34042053240208);
    way.push_back(point2);

    return way;
}


void MainWindow::on_endButton_clicked()
{

   Mapping *m = new Mapping;
   std::list<std::pair<double, double> > way = this->getWay(39.95102386325794, 116.34042053240208);

   //Drawing lines
   int x;
   int y;
   for (std::list<std::pair<double, double> >::iterator it=way.begin(); it != way.end(); ++it) {
       if (it != way.begin()) {
           scene->addLine(x, y, m->xPos((*it).second), m->yPos((*it).first), *pen);
       }
       x = m->xPos((*it).second);
       y = m->yPos((*it).first);
       // drawing point
       double rad = 2;
       scene->addEllipse(x-rad, y-rad, rad*2.0, rad*2.0,
                          QPen(), QBrush(Qt::green));
   }

}

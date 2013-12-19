#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <math.h>

#define SCREEN_WIDTH 960
#define SCREEN_HEIGHT 544
#define LON_RATIO 0.000008985640973
#define LAT_RATIO -0.000006821052714
#define X_RATIO 111288.666328474
#define Y_RATIO -146604.936492225
#define LAT_0 39.9527550177562
#define LON_0 116.337367080619

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
    scene->addPixmap(QPixmap(":/images/campus_flat.png"));
    //scene->addPixmap(QPixmap("/gps/images/campus_flat.png"));

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
    //Mapping *m = new Mapping;
    // testing coordinate
    double lat = 39.951445375634954;
    double lon = 116.33830158728031;
    int x = xPos(lon);
    int y = yPos(lat);

    startX = x;
    startY = y;

    // drawing point
    double rad = 2; // the point radius
    scene->addEllipse(x-rad, y-rad, rad*2.0, rad*2.0, QPen(), QBrush(Qt::green));

    // drawing flag
    QGraphicsPixmapItem *item = new QGraphicsPixmapItem(QPixmap(":/images/flag.png"));
    //QGraphicsPixmapItem *item = new QGraphicsPixmapItem(QPixmap("/gps/images/flag.png"));
    item->setPos(x-rad ,y-35); // image height is 35
    scene->addItem(item);
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

   std::list<std::pair<double, double> > way = this->getWay(39.95102386325794, 116.34042053240208);

   //Drawing lines
   int x;
   int y;
   for (std::list<std::pair<double, double> >::iterator it=way.begin(); it != way.end(); ++it) {
       if (it != way.begin()) {
           scene->addLine(x, y, xPos((*it).second), yPos((*it).first), *pen);
       }
       x = xPos((*it).second);
       y = yPos((*it).first);
       // drawing point
       double rad = 2;
       scene->addEllipse(x-rad, y-rad, rad*2.0, rad*2.0,
                          QPen(), QBrush(Qt::green));
   }

   // drawing flag
   QGraphicsPixmapItem *item = new QGraphicsPixmapItem(QPixmap(":/images/endflag.png"));
   //QGraphicsPixmapItem *item = new QGraphicsPixmapItem(QPixmap("/gps/images/endflag.png"));
   item->setPos(x ,y-35); // image height is 35
   scene->addItem(item);
}

int MainWindow::xPos(double longitude){
    double x = (longitude-LON_0)*(X_RATIO);
    return (int)round(x);
}

int MainWindow::yPos(double latitude){
    double y = (latitude-LAT_0)*(Y_RATIO);
    return (int)round(y);
}

double MainWindow::latitude(int y){
    //double latitude = (y * LAT_RATIO) + (LAT_0);
    return (double) (y * LAT_RATIO) + (LAT_0);
}

double MainWindow::longitude(int x){
    //double longitude = (x * LON_RATIO) + (LON_0);
    return (double) (x * LON_RATIO) + (LON_0);
}

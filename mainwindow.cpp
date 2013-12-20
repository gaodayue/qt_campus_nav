#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTime>
#include <QDebug>
#include <QGraphicsPixmapItem>
#include <gpsutils.h>

//------------------------------------
// for converting
#include <math.h>

#define SCREEN_WIDTH 960
#define SCREEN_HEIGHT 544
#define LON_RATIO 0.000008985640973
#define LAT_RATIO -0.000006821052714
#define X_RATIO 111288.666328474
#define Y_RATIO -146604.936492225
#define LAT_0 39.9527550177562
#define LON_0 116.337367080619
//------------------------------------

#define POINT_RADIUS 2.0

Node nodes[NUM_NODES] = {
    Node(0, 0, 0, 0),   // dummy node, just to make index count from 1
    Node(116.3340944051,	39.9503661462,	322,	173),
    Node(116.3349956274,	39.9505100762,	427,	146),
    Node(116.3331770897,	39.9499795896,	216,	229),
    Node(116.3340783119,	39.9501358603,	327,	197),
    Node(116.3332951069,	39.9495765740,	229,	292),
    Node(116.3342499733,	39.9497287332,	346,	264),
    Node(116.3351619244,	39.9498932293,	443,	239),
    Node(116.3365727663,	39.9501194107,	601,	199),
    Node(116.3380587101,	39.9503661533,	767,	159),
    Node(116.3367873430,	39.9494861008,	621,	293),
    Node(116.3377153873,	39.9496464851,	719,	268),
    Node(116.3382518291,	39.9497451829,	787,	249),
    Node(116.3345450163,	39.9487417486,	376,	405),
    Node(116.3350117207,	39.9488116605,	428,	392),
    Node(116.3353818655,	39.9488939096,	466,	382),
    Node(116.3368248940,	39.9490913073,	631,	343),
    Node(116.3379031420,	39.9489967210,	743,	374),
    Node(116.3384556770,	39.9490954197,	810,	355),
    Node(116.3369536400,	39.9486471618,	652,	419),
    Node(116.3378763199,	39.9487910981,	747,	392),
    Node(116.3355374336,	39.9480220633,	456,	515),
    Node(116.3369590044,	39.9482482509,	661,	461),
    Node(116.3378065825,	39.9484250880,	752,	432)
};

void MainWindow::buildGraph()
{
    graph = new Graph(NUM_NODES);
    // horizontal edges
    graph->connect(1, 2, 83);
    graph->connect(3, 4, 87);

    graph->connect(5, 6, 93);
    graph->connect(6, 7, 75);
    graph->connect(7, 8, 123);
    graph->connect(8, 9, 132);

    graph->connect(10, 11, 75);
    graph->connect(11, 12, 52);

    graph->connect(13, 14, 43);
    graph->connect(14, 15, 30);
    graph->connect(15, 16, 127);

    graph->connect(17, 18, 50);
    graph->connect(19, 20, 75);

    graph->connect(21, 22, 160);
    graph->connect(22, 23, 68);

    // vertical edges
    graph->connect(3, 5, 50);
    graph->connect(1, 4, 17);
    graph->connect(4, 6, 50);
    graph->connect(6, 13, 105);
    graph->connect(14, 21, 94);
    graph->connect(2, 7, 71);
    graph->connect(7, 15, 115);
    graph->connect(8, 10, 73);
    graph->connect(10, 16, 40);
    graph->connect(16, 19, 60);
    graph->connect(19, 22, 34);
    graph->connect(11, 17, 81);
    graph->connect(17, 20, 18);
    graph->connect(20, 23, 33);
    graph->connect(9, 12, 71);
    graph->connect(12, 18, 83);
}

// mapping from longitude to x index of pixel
int MainWindow::map2x(double longitude)
{
    double x = (longitude-LON_0)*(X_RATIO);
    return (int)round(x);
}

// mapping from latitude to y index of pixel
int MainWindow::map2y(double latitude)
{
    double y = (latitude-LAT_0)*(Y_RATIO);
    return (int)round(y);
}

int MainWindow::nearestNode(double longitude, double latitude)
{

    int lastNearest = -1;
    double lastDistance = -1;
    double currentDistance = 0;
    for(int i = 1; i < NUM_NODES; ++i) {
        currentDistance = pow(longitude - nodes[i].longitude, 2.0)
                + pow(latitude - nodes[i].latitude, 2.0);
        if (currentDistance < lastDistance || lastDistance == -1)
            {
               lastDistance = currentDistance;
               lastNearest = i;
            }
    }
    return lastNearest;
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->closeButton, SIGNAL(clicked()), this, SLOT(close()));

    setWindowFlags(Qt::FramelessWindowHint);

    pen = new QPen();
    pen->setBrush(Qt::green);
    pen->setWidth(3);

    // show images
    scene = new QGraphicsScene();
//    scene->addPixmap(QPixmap(":/images/campus_flat.png"));
    scene->addPixmap(QPixmap("/gps/images/campus_flat.png"));

    ui->graphicsView->resize(480, 272);
    ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    ui->graphicsView->setRenderHint(QPainter::Antialiasing);
    ui->graphicsView->setScene(scene);
    ui->graphicsView->setCacheMode(QGraphicsView::CacheBackground);
    ui->graphicsView->setDragMode(QGraphicsView::ScrollHandDrag);

    this->buildGraph();

    restarted = true;
}

MainWindow::~MainWindow()
{
    delete pen;
    delete graph;
    delete ui;
}

void MainWindow::on_startButton_clicked()
{
    restarted = false;
    double longitude = 116.3402435;
    double latitude = 39.95160164;

    GPSUtils::get_location(&longitude, &latitude);
    GPSUtils::record_location(longitude, latitude);

    int startNode = nearestNode(longitude, latitude);

    Node node = nodes[startNode];

    // drawing point

    scene->addEllipse(node.x - POINT_RADIUS,
                      node.y - POINT_RADIUS,
                      2 * POINT_RADIUS,
                      2 * POINT_RADIUS);
//    QGraphicsPixmapItem *flag = new QGraphicsPixmapItem(QPixmap(":/images/flag.png"));
    QGraphicsPixmapItem *flag = new QGraphicsPixmapItem(QPixmap("/gps/images/flag.png"));
    flag->setPos(node.x - POINT_RADIUS, node.y - 35);
    scene->addItem(flag);
    ui->graphicsView->centerOn(QPointF(node.x, node.y));
}

void MainWindow::on_endButton_clicked()
{
    int startNode, endNode;
    double start_long, start_lati, end_long, end_lati;

    // get start node from file
    GPSUtils::read_location(&start_long, &start_lati);
    startNode = this->nearestNode(start_long, start_lati);

    // get end node from GPS
    GPSUtils::get_location(&end_long, &end_lati);
    end_long = 116.3359558582;
    end_lati = 39.9490748575;
    endNode = this->nearestNode(end_long, end_lati);

    QLinkedList<int> path = graph->shortestPath(startNode, endNode);

    // log out path
    foreach(int i, path)
    {
        qDebug() << i << ",";
    }

    // Drawing path
    int prev = -1;
    foreach(int i, path)
    {
        if (i != startNode)
        {
            Node from = nodes[prev];
            Node to = nodes[i];
            // draw line
            scene->addLine(from.x, from.y, to.x, to.y, *pen);
            // draw point
            scene->addEllipse(to.x - POINT_RADIUS,
                              to.y - POINT_RADIUS,
                              2 * POINT_RADIUS,
                              2 * POINT_RADIUS);
            ui->graphicsView->centerOn(QPointF(to.x, to.y));
            delay(500); // micro seconds
        }
        else if (restarted)
        {   // if the device restart, should draw startNode too
            Node node = nodes[startNode];
//            QGraphicsPixmapItem *startFlag = new QGraphicsPixmapItem(QPixmap(":/images/flag.png"));
            QGraphicsPixmapItem *startFlag = new QGraphicsPixmapItem(QPixmap("/gps/images/flag.png"));
            startFlag->setPos(node.x - POINT_RADIUS, node.y - 35);
            scene->addItem(startFlag);
            ui->graphicsView->centerOn(QPointF(node.x, node.y));
        }

        prev = i;
    }

    // Draw the end flag
    Node destination = nodes[endNode];
//    QGraphicsPixmapItem *flag = new QGraphicsPixmapItem(QPixmap(":/images/endflag.png"));
    QGraphicsPixmapItem *flag = new QGraphicsPixmapItem(QPixmap("/gps/images/endflag.png"));
    flag->setPos(destination.x, destination.y - 35);
    scene->addItem(flag);
}

void MainWindow::delay(int mSec)
{
    QTime dieTime= QTime::currentTime().addMSecs(500);
    while( QTime::currentTime() < dieTime )
    QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}

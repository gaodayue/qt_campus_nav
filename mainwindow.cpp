#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "mapping.h"
#include <QDebug>
#include <QGraphicsPixmapItem>

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
    Node(116.3402435,	39.95160164,	322,	173),
    Node(116.341134,	39.95176613,	427,	146),
    Node(116.3393208,	39.95122331,	216,	229),
    Node(116.3402972,	39.95137958,	327,	197),
    Node(116.3394442,	39.95085115,	229,	292),
    Node(116.3404205,	39.95102386,	346,	264),
    Node(116.3413164,	39.95118836,	443,	239),
    Node(116.3427326,	39.95144332,	601,	199),
    Node(116.34424,     39.95171473,	767,	159),
    Node(116.3429204,	39.95078946,	621,	293),
    Node(116.3438913,	39.95096629,	719,	268),
    Node(116.3444117,	39.95106499,	787,	249),
    Node(116.3407317,	39.95002456,	376,	405),
    Node(116.3412306,	39.9501027,     428,	392),
    Node(116.3415739,	39.95016438,	466,	382),
    Node(116.3430115,	39.95042757,	631,	343),
    Node(116.3440469,	39.95029186,	743,	374),
    Node(116.3446155,	39.95039467,	810,	355),
    Node(116.3431886,	39.9499382,     652,	419),
    Node(116.3440844,	39.95009447,	747,	392),
    Node(116.3414505,	39.94928844,	456,	515),
    Node(116.3433066,	39.94959687,	661,	461),
    Node(116.343902,	39.94968323,	752,	432)
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

int MainWindow::nearistNode(double longitude, double latitude)
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

    pen = new QPen();
    pen->setBrush(Qt::green);
    pen->setWidth(3);

    // show images
    scene = new QGraphicsScene();
    scene->addPixmap(QPixmap(":/images/campus_flat.png"));
    //scene->addPixmap(QPixmap("/gps/images/campus_flat.png"));

    ui->graphicsView->resize(480, 272);
    ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    ui->graphicsView->setRenderHint(QPainter::Antialiasing);
    ui->graphicsView->setScene(scene);
    ui->graphicsView->setCacheMode(QGraphicsView::CacheBackground);
    ui->graphicsView->setDragMode(QGraphicsView::ScrollHandDrag);

    this->buildGraph();
}

MainWindow::~MainWindow()
{
    delete pen;
    delete graph;
    delete ui;
}

void MainWindow::on_startButton_clicked()
{
    // TODO: should get location from GPS and
    // calculate nearist startNode
    startNode = nearistNode(116.3402435, 39.95160164);

    Node node = nodes[startNode];

    // drawing point

    scene->addEllipse(node.x - POINT_RADIUS,
                      node.y - POINT_RADIUS,
                      2 * POINT_RADIUS,
                      2 * POINT_RADIUS);
    QGraphicsPixmapItem *flag = new QGraphicsPixmapItem(QPixmap(":/images/flag.png"));
//    QGraphicsPixmapItem *flag = new QGraphicsPixmapItem(QPixmap("/gps/images/flag.png"));
    flag->setPos(node.x - POINT_RADIUS, node.y - 35);
    scene->addItem(flag);
}

void MainWindow::on_endButton_clicked()
{
    // TODO: should get localtion from GPS and
    // calculate the nearist endNode
    const int end_node = nearistNode(116.343902,39.94968323);

    QLinkedList<int> path = graph->shortestPath(startNode, end_node);

    // log out path
    foreach(int i, path)
    {
        qDebug() << i << ",";
    }

    // Drawing path
    int prev = -1;
    foreach(int i, path)
    {
        // FIXME if the device restart, should draw startNode too
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
        }
        prev = i;
    }

    // Draw the end flag
    Node destination = nodes[end_node];
    QGraphicsPixmapItem *flag = new QGraphicsPixmapItem(QPixmap(":/images/endflag.png"));
//    QGraphicsPixmapItem *flag = new QGraphicsPixmapItem(QPixmap("/gps/images/endflag.png"));
    flag->setPos(destination.x, destination.y - 35);
    scene->addItem(flag);
}

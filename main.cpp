#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //QFont appFont("Bitstream Vera Serif");
    //a.setFont(appFont);
    MainWindow w;
    w.show();
    
    return a.exec();
}

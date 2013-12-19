#ifndef MYGRAPHICSVIEW_H
#define MYGRAPHICSVIEW_H

#include <QGraphicsView>

class myGraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    //explicit myGraphicsView(QObject *parent = 0);
    myGraphicsView(QWidget *parent = 0);

    bool viewportEvent(QEvent *event);
    
signals:
    
public slots:
    
};

#endif // MYGRAPHICSVIEW_H

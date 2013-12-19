#include "mygraphicsview.h"
#include <QScrollBar>
#include <QTouchEvent>

myGraphicsView::myGraphicsView(QWidget *parent) :
    QGraphicsView(parent)
{
    viewport()->setAttribute(Qt::WA_AcceptTouchEvents);
    setDragMode(ScrollHandDrag);
}

bool myGraphicsView::viewportEvent(QEvent *event){
    switch (event->type()){
    case QEvent::TouchBegin:
    case QEvent::TouchUpdate:
    case QEvent::TouchEnd:
    {
        printf("TOUCHED!\n");
        QTouchEvent *touchEvent = static_cast<QTouchEvent *>(event);
        QList<QTouchEvent::TouchPoint> touchPoints = touchEvent->touchPoints();
        if (touchPoints.count() > 0) {
            // begin getting position from one of the touch point
            // can use touchPoints.first() or touchPoints.last() or touchPoints.at()
            const QTouchEvent::TouchPoint &touchPoint0 = touchPoints.first();
            // const QTouchEvent::TouchPoint &touchPoint1 = touchPoints.last();
            // if (touchEvent->touchPointStates() & Qt::TouchPointReleased) { }

            int x = touchPoint0.pos().x();
            int y = touchPoint0.pos().y();
            printf("X:%d, Y:%d\n", x, y);
        }
    }
    default:
        break;
    }
    return QGraphicsView::viewportEvent(event);
}

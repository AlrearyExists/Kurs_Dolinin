#include "myqwtplot.h"
#include <QMouseEvent>

myQwtPlot::myQwtPlot(QWidget *w)
{
    setParent(w);
    timer = new QTimer;
    QObject::connect(timer,SIGNAL(timeout()), this, SLOT(timer_out()));
}


void myQwtPlot::mousePressEvent(QMouseEvent *event)
{
    st_x = transform_x(event->x());
    st_y = transform_y(event->y());
    speed_factor = 0.0;
    timer->start(10);
}


void myQwtPlot::mouseReleaseEvent(QMouseEvent *event)
{
    end_x = transform_x(event->x());
    end_y = transform_y(event->y());
    timer->stop();
    emit pull(st_x, st_y, end_x, end_y, speed_factor);
}

double myQwtPlot::transform_x(int x)
{
    return (double)(x - pos().x())*range_x/width();
}

double myQwtPlot::transform_y(int y)
{
    return (double)(height() -y -pos().y())*range_y/(double)height();
}

void myQwtPlot::timer_out()
{
    speed_factor += 1.0;
}

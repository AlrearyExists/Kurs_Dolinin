#ifndef MYQWTPLOT_H
#define MYQWTPLOT_H
#include <qwt_plot.h>
#include <QTimer>
#include <QObject>

class myQwtPlot : public QwtPlot
{
     Q_OBJECT
public:
    myQwtPlot(QWidget *w);

    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

    double transform_x(int x);
    double transform_y(int y);

    double range_x;
    double range_y;

    double st_x;
    double st_y;

    double end_x;
    double end_y;
    double speed_factor;
    QTimer *timer;

signals: void pull(double x1, double y1, double x2, double y2, double speed_factor);
public
    slots:
    void timer_out();
};

#endif // MYQWTPLOT_H

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <qwt_plot.h>
#include <qwt_plot_curve.h>
#include <QVector>
#include <QPointF>
#include "qbilliard.h"
#include <qwt_symbol.h>
#include "myqwtplot.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    QVector<QPointF> circle(float x, float y, float r);

    QVector<QPointF> vector;
    QVector<QPointF> vector2;
    QVector<QPointF> vector3;

    QwtPlotCurve *curve;
    QwtPlotCurve *curve2;
    QwtPlotCurve *curve3;

    QwtSymbol *symbol;
    QwtSymbol *symbol3;

    QBilliard *billiard;

    QwtPlotCurve *curve_luslz[6];

    myQwtPlot *plot;
    void init_once();
    void update_image();
    bool stop_cond();
    bool stop;
    int N_p;
    int balls;
    int p[2];

public slots:
    void pull(double x1, double y1, double x2, double y2, double speed_factor);

private slots:
    void on_pushButton_clicked();


    void on_actionStart_triggered();

    void on_actionStop_triggered();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H

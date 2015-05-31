#include "mainwindow.h"
#include <QApplication>
#include "qwt_plot.h"
#include "myqwtplot.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    myQwtPlot *plot = new myQwtPlot(&w);
    w.plot = plot;
    w.init_once();

    w.show();
    
    return a.exec();
}

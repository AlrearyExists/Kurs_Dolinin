#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMouseEvent>
#include "myqwtplot.h"
#include <qwt_plot.h>

void MainWindow::init_once()
{
    billiard = new QBilliard;

    QObject::connect(plot, SIGNAL(pull(double, double, double, double, double)), this, SLOT(pull(double, double, double, double, double)));

    plot->range_x = billiard->X_size;
    plot->range_y = billiard->Y_size;

    plot->setAxisAutoScale(0, 0);
    plot->setAxisAutoScale(2, 0);

    plot->setAxisScale(0, 0, billiard->Y_size);
    plot->setAxisScale(2, 0, billiard->X_size);

    plot->enableAxis(0, 0);
    plot->enableAxis(2, 0);

    plot->setGeometry(15, 40, 360,610);

    curve = new QwtPlotCurve;
    curve2 = new QwtPlotCurve;
    curve3 = new QwtPlotCurve;

    curve->attach(plot);
    curve2->attach(plot);
    curve3->attach(plot);

    curve->setPen(QPen(QColor(255,0,0,255)));
    curve2->setPen(QPen(QColor(0,0,255,255)));

    curve->setStyle(QwtPlotCurve::Dots);
    curve2->setStyle(QwtPlotCurve::Dots);
    curve3->setStyle(QwtPlotCurve::Dots);
    stop = 0;


    for (int i = 0; i < 6; i++) {
        curve_luslz[i] = new QwtPlotCurve;
        curve_luslz[i]->attach(plot);
        curve_luslz[i]->setSamples(circle(billiard->luzls[i][0], billiard->luzls[i][1], billiard->luzl_R));
    }
    plot->replot();
    balls = billiard->left_balls;
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->label_6->setVisible(0);
    ui->label_2->setVisible(0);
    ui->label_3->setVisible(0);
    ui->label_4->setVisible(0);
    ui->label_5->setVisible(0);
    ui->label->setVisible(0);
    ui->textEdit->setVisible(0);
    ui->pushButton->setVisible(0);
    N_p = 0;
    p[0] = 0;
    p[1] = 0;
}

MainWindow::~MainWindow()
{
    delete ui;
}

QVector<QPointF> MainWindow::circle(float x, float y, float r){
    QVector<QPointF> vector1;
    double sin;
    double cos;
    for (float i = 0; i <= 3.14/2; i += 0.05){
        sin = qSin(i);
        cos = qCos(i);
        vector1.append(QPointF(x + r*cos, y + r*sin));
        vector1.append(QPointF(x + r*cos, y - r*sin));
        vector1.append(QPointF(x - r*cos, y + r*sin));
        vector1.append(QPointF(x - r*cos, y - r*sin));
    }
    return vector1;
}

void MainWindow::on_pushButton_clicked()
{
    if (!ui->label_6->isVisible()) {
        billiard->balls[0][0] = billiard->X_size/2;
        billiard->balls[0][1] = billiard->Y_size/4;
        billiard->balls[0][4] = 1;
        update_image();
    }
}

void MainWindow::pull(double x1, double y1, double x2, double y2, double speed_factor)
{
    QString str;
    if (!ui->label_6->isVisible()){
        if (N_p == 0) {
            ui->label_2->setText("<FONT COLOR=#FF0000>Player 1</FONT>");
            ui->label_3->setText("<FONT COLOR=#000000>Player 2</FONT>");
        }

        if (N_p == 1) {
            ui->label_2->setText("<FONT COLOR=#000000>Player 1</FONT>");
            ui->label_3->setText("<FONT COLOR=#FF0000>Player 2</FONT>");
        }

        double sp = billiard->pull(x1, y1, x2, y2, speed_factor);
        str.setNum(sp);
        ui->textEdit->setText(str);
        ui->label_6->setVisible(1);
        while (!stop_cond() && !stop) {
            QApplication::processEvents();
            billiard->move();
            update_image();
        }

        if ((balls - billiard->left_balls) > 0) {
            p[N_p] += (balls - billiard->left_balls);
        } else {
            // N_p = ~N_p;
            if (N_p == 1)
                N_p = 0;
            else {
                if (N_p == 0)
                    N_p = 1;
            }
        }

        balls = billiard->left_balls;
        ui->label_6->setVisible(0);
        str.setNum(p[0]);
        ui->label_4->setText(str);
        str.setNum(p[1]);
        ui->label_5->setText(str);
        if (N_p == 0) {
            ui->label_2->setText("<FONT COLOR=#FF0000>Player 1</FONT>");
            ui->label_3->setText("<FONT COLOR=#000000>Player 2</FONT>");
        }

        if (N_p == 1) {
            ui->label_2->setText("<FONT COLOR=#000000>Player 1</FONT>");
            ui->label_3->setText("<FONT COLOR=#FF0000>Player 2</FONT>");
        }
    }
}

void MainWindow::update_image()
{
    vector.clear();
    vector2.clear();
    for (int i = 1; i < 16; i++)
        vector.operator +=(circle(billiard->balls[i][0], billiard->balls[i][1], billiard->ball_R));

    vector2.operator +=(circle(billiard->balls[0][0], billiard->balls[0][1], billiard->ball_R));

    curve2->setSamples(vector2);
    curve->setSamples(vector);
    plot->replot();
}

bool MainWindow::stop_cond()
{
    bool st = 1;
    for (int i = 0; i < 16; i++) {
        if ((billiard->balls[i][2] != 0.0) || (billiard->balls[i][3] != 0.0)) {
            return 0;
        }
    }
    //  billiard->recount_collisions();
    return st;
}

void MainWindow::on_actionStart_triggered()
{
    billiard = new QBilliard;
    billiard->init();
    update_image();
    stop = 0;
    ui->label_2->setVisible(1);
    ui->label_3->setVisible(1);
    ui->label_4->setVisible(1);
    ui->label_5->setVisible(1);

    ui->label->setVisible(1);
    ui->textEdit->setVisible(1);
    ui->pushButton->setVisible(1);
    ui->label_2->setText("<FONT COLOR=#FF0000>Player 1</FONT>");
}

void MainWindow::on_actionStop_triggered()
{
    stop = 1;
}

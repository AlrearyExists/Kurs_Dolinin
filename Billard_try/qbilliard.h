#ifndef QBILLIARD_H
#define QBILLIARD_H
#include <QList>
#include <QPoint>

class QBilliard
{
public:
    QBilliard();
    void init();
    void move();
    double pull(double x1, double y1, double x2, double y2, double speed_factor);
    bool check_for_across(double x1, double y1, double x2, double y2);

    double N_balls;
    double balls[16][5];
    double luzls[6][2];
    double X_size;
    double Y_size;

    double ball_R;
    double luzl_R;
    double m;

    double delta;
    double k;
    double k_inter;
    double dt_st;
    double dt;
    double dtau;
    double col_t;

    double t_move;
    double t_global;
    double t_without_stop;

    void velocity_controll();
    void recount_collisions();
    void collide();
    void luzls_control(int i);
    int left_balls;

     void interact(int i, int j);
//    int collisiers[8][2];
//    int collisiers_wall[16][2];
    QList <QPoint> collisiers;
    QList <QPoint> collisiers_wall;

    int col_w;
    int col;
};

#endif // QBILLIARD_H

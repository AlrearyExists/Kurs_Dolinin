#include "qbilliard.h"
#include <math.h>
#include <qmath.h>

double pi = 3.14;

QBilliard::QBilliard()
{
    t_without_stop = 0;
    col_w = 0;
    col = 0;
    delta = 0.01;
    N_balls = 16;
    left_balls = N_balls - 1;
    X_size = 1.410;
    Y_size = 2.53;
    t_global = 0;

    ball_R = 0.034;
    luzl_R = 0.05;
    m = 0.288;
    dt_st = 0.01;
    dt  = dt_st;

    //Define luzls position
    luzls[0][0] = 0;
    luzls[0][1] = 0;

    luzls[1][0] = X_size;
    luzls[1][1] = 0;

    luzls[2][0] = 0;
    luzls[2][1] = Y_size/2;

    luzls[3][0] = X_size;
    luzls[3][1] = Y_size/2;

    luzls[4][0] = 0;
    luzls[4][1] = Y_size;

    luzls[5][0] = X_size;
    luzls[5][1] = Y_size;

    for (int i = 0; i < 16; i++) {
        for (int j = 0; j < 4; j++) {
            balls[i][j] = 0.0;
        }
        balls[i][4] = 1.0;
    }

    k = 0.25;
    k_inter = 0.25;
}

void QBilliard::init()
{
    balls[0][0] = X_size/2;
    balls[0][1] = Y_size/4;

    balls[1][0] = X_size/2;
    balls[1][1] = Y_size*(0.5 + 1.0/8);

    balls[2][0] = balls[1][0] + delta + ball_R;
    balls[2][1] = balls[1][1] + delta + 2*ball_R;

    balls[3][0] = balls[1][0] - delta - ball_R;
    balls[3][1] = balls[2][1];

    balls[4][0] = X_size/2;
    balls[4][1] = balls[3][1] + delta + 2*ball_R;

    balls[5][0] = X_size/2 + 2*ball_R + delta;
    balls[5][1] = balls[4][1];

    balls[6][0] = X_size/2 - 2*ball_R - delta;
    balls[6][1] = balls[4][1];

    balls[7][0] = X_size/2 + delta + ball_R;
    balls[7][1] = balls[6][1] + delta + 2*ball_R;

    balls[8][0] = X_size/2 - delta - ball_R;
    balls[8][1] = balls[7][1];

    balls[9][0] = balls[7][0] + delta + 2*ball_R;
    balls[9][1] = balls[7][1];

    balls[10][0] = balls[8][0] - delta - 2*ball_R;
    balls[10][1] = balls[7][1];

    balls[11][0] = X_size/2;
    balls[11][1] = balls[10][1] + 2*ball_R + delta;

    balls[12][0] = balls[11][0] + 2*ball_R + delta;
    balls[12][1] = balls[11][1];

    balls[13][0] = balls[11][0] - 2*ball_R - delta;
    balls[13][1] = balls[11][1];

    balls[14][0] = balls[12][0] + 2*ball_R + delta;
    balls[14][1] = balls[11][1];

    balls[15][0] = balls[13][0] - 2*ball_R - delta;
    balls[15][1] = balls[11][1];
}

void QBilliard::move()
{
    velocity_controll();
    if ((t_global + dt ) < t_without_stop) {
        t_global += dt;

        for (int i = 0; i < 16; i++) {
            balls[i][0] += balls[i][2]*dt;
            balls[i][1] += balls[i][3]*dt;
        }

    } else {

        double dt2 = t_without_stop - t_global;
        t_global += dt2;

        for (int i = 0; i < 16; i++) {
            balls[i][0] += balls[i][2]*dt2;
            balls[i][1] += balls[i][3]*dt2;
        }

        collide();
        recount_collisions();
    }
}

double QBilliard::pull(double x1, double y1, double x2, double y2, double speed_factor)
{
    speed_factor = 10*sqrt( pow((x1 - x2), 2) + pow((y1 - y2), 2))/speed_factor;
    double cs;
    double sn;

    double tn = (y2 - y1)/(x2 - x1);


    cs = sqrt(1/(tn*tn + 1));
    sn = sqrt(1 - cs*cs);

    if ((x2 < x1) && (y2 > y1))
        cs *= (-1);

    if ((x2 < x1) && (y2 < y1)) {
        cs *= (-1);
        sn *= (-1);
    }

    if ((x2 > x1) && (y2 < y1))
        sn *= (-1);

    if (check_for_across(x1, y1, x2, y2))
    {
        balls[0][2] = speed_factor*cs;
        balls[0][3] = speed_factor*sn;
    }
    recount_collisions();
    return speed_factor;
}

bool QBilliard::check_for_across(double x1, double y1, double x2, double y2)
{
//    double x0 = balls[0][0];
//    double y0 = balls[0][1];
//    double a = (y2 - y1)/(x2 - x1);
//    double b = -a*x1 + y1;
//    double D = qPow((a*(b - y0) - x0), 2) - (1 + a*a)*(x0*x0 + qPow((b - y0), 2) - ball_R*ball_R);
//    if (D >= 0)
//        return 1;
//    else
//        return 0;
    return 1;

}

void QBilliard::velocity_controll()
{
    for (int i = 0; i < 16; i++) {
        balls[i][2] *= (1 - k*dt);
        balls[i][3] *= (1 - k*dt);
        if ((qAbs(balls[i][2]) < 0.01) && (qAbs(balls[i][3]) < 0.01)) {
            balls[i][2] = 0;
            balls[i][3] = 0;
        }
    }
}

void QBilliard::interact(int i, int j)
{   
    double Sx = balls[j][0] - balls[i][0];
    double Sy = balls[j][1] - balls[i][1];
    double S = qPow(Sx, 2) + qPow(Sy, 2);

    double temp[4];

    //new velocity (particle i , x-projection)
    temp[0] = (1.0/S)*(balls[j][2]*qPow(Sx,2) + (balls[j][3] - balls[i][3])*Sx*Sy + balls[i][2]*qPow(Sy,2));

    //new velocity (particle i , y-projection)
    temp[1] = (1.0/S)*(balls[j][3]*qPow(Sy,2) + (balls[j][2] - balls[i][2])*Sx*Sy + balls[i][3]*qPow(Sx,2));

    //new velocity (particle j , x-projection)
    temp[2] = (1.0/S)*(balls[i][2]*qPow(Sx,2) + (balls[i][3] - balls[j][3])*Sx*Sy + balls[j][2]*qPow(Sy,2));

    //new velocity (particle j , y-projection)
    temp[3] = (1.0/S)*(balls[i][3]*qPow(Sy,2) + (balls[i][2] - balls[j][2])*Sx*Sy + balls[j][3]*qPow(Sx,2));

    balls[i][2] = temp[0];
    balls[i][3] = temp[1];
    balls[j][2] = temp[2];
    balls[j][3] = temp[3];
}

void QBilliard::recount_collisions()
{
    collisiers.clear();

    //recounting minimum time
    QList<QPoint> mols_collised_wall;
    QList<QPoint> mols_collised;

    this->t_without_stop = 10000;

    for (int i = 0; i < 16; i++) {
        mols_collised.clear();
        long double tau = 10000;
        long double tau_temp = 10000;

        //wals
        //x
        if (balls[i][4] != 0.0) {
            if (balls[i][2] != 0) {
                if (balls[i][2] > 0){
                    tau_temp = (X_size - ball_R - balls[i][0])/balls[i][2];
                    if (tau_temp < tau) {
                        tau = tau_temp;
                        mols_collised_wall.clear();
                        mols_collised_wall.append(QPoint(i,-2));
                    }
                }
                else {
                    tau_temp = (ball_R - balls[i][0])/balls[i][2];
                    if (tau_temp < tau) {
                        tau = tau_temp;
                        mols_collised_wall.clear();
                        mols_collised_wall.append(QPoint(i,-2));
                    }
                }
            }
            //y
            if (balls[i][3] != 0) {
                if (balls[i][3] > 0){
                    tau_temp = (Y_size - ball_R - balls[i][1])/balls[i][3];
                    if (tau_temp < tau) {
                        tau = tau_temp;
                        mols_collised_wall.clear();
                        mols_collised_wall.append(QPoint(i,-1));
                    }
                }
                else {
                    tau_temp = (ball_R - balls[i][1])/balls[i][3];
                    if (tau_temp < tau) {
                        tau = tau_temp;
                        mols_collised_wall.clear();
                        mols_collised_wall.append(QPoint(i,-1));
                    }
                }
            }
        }
        //One with other
        long double a, b, c, d, dx, dy, dvx, dvy, x1, x2;
        for (int j = i + 1; j < 16; j++){
            if (balls[j][4] != 0) {
                dx = balls[j][0] - balls[i][0];
                dy = balls[j][1] - balls[i][1];
                dvx = balls[i][2] - balls[j][2];
                dvy = balls[i][3] - balls[j][3];
                a = qPow(dvx, 2) + qPow(dvy,2);
                b = -2*(dx*dvx+dy*dvy);
                c = qPow(dx, 2) + qPow(dy, 2) - 4*ball_R*ball_R;
                d = qPow(b,2) - 4*a*c;

                if ((d >= 0) && ( b<0 )){
                    d = qPow(d, 0.5);
                    x1 = (-b - d)/(2*a);
                    x2 = (-b + d)/(2*a);
                    if ((x1 > 0) && (x2 > 0)){
                        if (x1 < x2)
                            tau_temp = x1;
                        else tau_temp = x2;
                    }

                    else {
                        if (x1>0) tau_temp = x1;
                        if (x2>0) tau_temp = x2;
                    }
                    if (tau_temp==tau) {
                        mols_collised.append(QPoint(i,j));
                    }
                    if (tau_temp < tau){
                        mols_collised.clear();
                        mols_collised_wall.clear();
                        mols_collised.append(QPoint(i,j));
                        tau = tau_temp;
                    }
                }
            }
        }

        tau = 1.0/k*qLn(1.0/(1 - k*tau));
        if (tau < dt_st)
            dt = tau/2;
        else
            dt = dt_st;

        mols_collised.operator +=(mols_collised_wall);
        if (tau == (t_without_stop - t_global)){
            collisiers.operator +=(mols_collised);
        }

        if (tau < (t_without_stop - t_global) && (tau > 0)){
            t_without_stop = t_global + tau;
            collisiers.clear();
            collisiers.operator +=(mols_collised);
        }
    }
}

void QBilliard::collide(){
    if (this->collisiers.size() > 0){
        QPoint particles;
        for (int i = 0; i < collisiers.size(); i++){
            particles = collisiers.operator [](i);
            switch (particles.y()){
            case -2: {
                luzls_control(particles.x());

                balls[particles.x()][2] *= (k_inter - 1);
                break;
            }
            case -1: {
              luzls_control(particles.x());
                balls[particles.x()][3] *= (k_inter - 1);
                break;
            }
            default: {
                interact(particles.x(), particles.y());
                balls[particles.x()][2] *= (1 - k_inter);
                balls[particles.x()][3] *= (1 - k_inter);

                balls[particles.y()][2] *= (1 - k_inter);
                balls[particles.y()][3] *= (1 - k_inter);
                break;
            }
            }
        }
    }
}

void QBilliard::luzls_control(int i)
{
    for (int j = 0; j < 6; j++) {
        if ((pow((luzls[j][0] - balls[i][0]), 2) + pow((luzls[j][1] - balls[i][1]), 2)) <= 12*ball_R*ball_R) {
            balls[i][4] = 0.0;
            balls[i][2] = 0.0;
            balls[i][3] = 0.0;
            balls[i][0] = -10.0;
            balls[i][1] = -10.0;
            if (i != 0) 
					left_balls--;
    }
}
}

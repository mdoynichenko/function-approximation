#ifndef DRAW_H
#define DRAW_H


#include <QPoint>
#include <QPainter>
#include "add.h"
#include <functional>

using namespace std;

class Drawer{
public:
    double x0;
    double y0;
    double max;
    double min;
    double a;
    double b;
    int n;
    double scaleX;
    double scaleY;

    Drawer():
        x0(0),
        y0(0),
        scaleX(1),
        scaleY(1)
    {}
    Drawer(double a, double b, double scaleX, double scaleY):
        x0(a),
        y0(b),
        scaleX(scaleX),
        scaleY(scaleY)
    {}

    ~Drawer();

    double norm_size;

    void drawAxes(QPainter* p, double width, double height);
    QPoint FixCoords(double x, double y);
    void drawFunction1(QPainter *p, double a, double b, GData* data, double(GData::*)(double));
    void drawFunction(QPainter *p, double a, double b, const function<double(double)> &myfunc, int pen);

    void DrawGraphic(QPainter *p, GData* data, double width);
    double getX() {return scaleX;}
    double getY() {return scaleY;}
    void rescaleX(double sc){scaleX = sc;}
    void rescaleY(double sc){scaleY = sc;}
    void moveTo(double x, double y){x0 = x; y0 = y;}
};

#endif // DRAW_H

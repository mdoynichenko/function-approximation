#include "add.h"
#include "draw.h"
#include <iostream>
#include <QString>
#include <sstream>
#include <functional>

using namespace std;


QPoint Drawer::FixCoords(double x, double y)
{
    return QPoint(static_cast<int>((x-a)*scaleX) , static_cast<int>((max-y)*scaleY));
}

Drawer::~Drawer()
{}


void Drawer::DrawGraphic(QPainter *p, GData* data, double width)
{
    QPolygon polyg1, polyg2;
    QPen pen(Qt::black, 2, Qt::SolidLine);

    polyg1 << QPoint(width, y0) << QPoint(width - 20, y0 + 5) << QPoint(width - 20, y0 - 5);//правый
    polyg2 << QPoint(x0, 0) << QPoint(x0 - 5, 20) << QPoint(x0 + 5, 20);//верхний

    p->setPen(pen);

    p->drawLine(x0, 0, x0, norm_size); //y axis (0, 0) = (10, 200)
    p->drawLine(0, y0, norm_size, y0); // x axis

    p->setPen(QPen(Qt::yellow, 10, Qt::SolidLine));
    p->drawPoint(FixCoords(0,0));

    p->setPen(QPen(Qt::black, 2, Qt::SolidLine));
    p->setBrush(QBrush(Qt::black));
    p->drawPolygon(polyg1);
    p->drawPolygon(polyg2);


    std::ostringstream s;
    s << "max: " << data->max << " min : " << data->min << " n = " << data->n << " k = " << data->type << "  f(x) = ";

    switch(data->k)
    {
    case 0: {
        s << "1";
        break;
    }
    case 1: {
        s << "x";
        break;
    }
    case 2:{
        s << "x^2";
        break;
    }
    case 3: {
        s << "x^3";
        break;
    }
    case 4: {
        s << "x^4";
        break;
    }
    case 5:{
        s << "e^x";
        break;
    }
    case 6:{
        s << "1/(25x^2 + 1)";
        break;
    }
    }

    std::string str = s.str();
    std::cout << data->max << std::endl;
    QString as(str.c_str());
    p->drawText(50, 50, as);
}

void Drawer::drawFunction(QPainter *p, double a, double b, const function<double(double)> &myfunc, int pen)
{
    int step = 64;
    double tmp_x = a;
    double prev_x = a;
    double tmp_f = myfunc(tmp_x);
    double prev_f = tmp_f;
    switch(pen)
    {
    case 0: {
        p->setPen(QPen(Qt::blue, 5, Qt::SolidLine));
        break;
    }
    case 1: {
        p->setPen(QPen(Qt::green, 3, Qt::SolidLine));
        break;
    }
    case 2: {
        p->setPen(QPen(Qt::red, 1, Qt::SolidLine));
        break;
    }
    }
    p->drawPoint(FixCoords(tmp_x, tmp_f));
    for(int i = 0; i < step; i++)
    {

         prev_x = tmp_x;
         prev_f = tmp_f;
         tmp_x = a + (b-a)*i/(step - 1);
         tmp_f = myfunc(tmp_x);
         p->drawLine(FixCoords(prev_x, prev_f), FixCoords(tmp_x, tmp_f));
         //p->drawPoint(FixCoords(tmp_x, tmp_f));
    }
}

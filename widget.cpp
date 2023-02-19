#include "widget.h"
#include "ui_widget.h"
#include "add.h"
#include <iostream>
#include <QKeyEvent>

Widget::Widget(QWidget *parent, double a, double b, int n, int k)
    : QWidget(parent)
{
    gdata = new GData(a, b, n, k);
    draw = new Drawer();
    draw->norm_size = norm_size;
}

Widget::~Widget()
{

    delete ui;
}

void Widget::keyPressEvent(QKeyEvent *event)
{
    switch(event->key()){
    case 48:
    {
        std::cout << "change of function" << std::endl;
        gdata->k++;
        if(gdata->k > 6) gdata->k = 0;
        gdata->updateF();
        update();
        break;
    }
    case 49:
    {
        std::cout << "change of approximation functions" << std::endl;
        if(gdata->n > 50)
        {
            std::cout << "can not change (n > 50), can display only function itself" << std::endl;
        }
        else
        {
            gdata->type++;
            if(gdata->type > 4) gdata->type = 1;
            gdata->updateF();
            update();
        }
        break;
    }
    case 50:
    {
        std::cout << "changed: x*2" << std::endl;
        double old_a = gdata->a;
        double old_b = gdata->b;
        gdata->b = (old_a+old_b)/2 + old_b - old_a;
        gdata->a = (old_a+old_b)/2 - (old_b - old_a);
        gdata->updateF();
        update();
        break;
    }
    case 51:
    {
        std::cout << "changed: x/2" << std::endl;
        double old_a = gdata->a;
        double old_b = gdata->b;
        gdata->b = (old_a+old_b)/2 + (old_b - old_a)/4;
        gdata->a = (old_a+old_b)/2 - (old_b - old_a)/4;
        gdata->updateF();
        update();
        break;
    }
    case 52:
    {
        std::cout << "increasing of nuber of dots" << std::endl;
        gdata->n*=2;
        if(gdata->n > 50) gdata->type = 1;
        gdata->updateF();
        update();
        break;
    }
    case 53:
    {
        std::cout << "decreasing of number of dots" << std::endl;
        gdata->n = static_cast<int>(gdata->n/2);
        if(gdata->n < 4) gdata->n = 4;
        gdata->updateF();
        update();
        break;
    }
    case 54:
    {
        std::cout << "decreasing of p" << std::endl;
        gdata->plus++;
        gdata->updateF();
        update();
        break;
    }
    case 55:
    {
        std::cout << "decreasing of p" << std::endl;
        gdata->plus--;
        gdata->updateF();
        update();
        break;
    }
    default:
        std::cout << "unknown command" << std::endl;
        break;
    }
}

void Widget::paintEvent(QPaintEvent *)
{

    gdata->countTMM();
    double w = gdata->b - gdata->a;
    double h = gdata->max - gdata->min;

    std::cout << "width: " <<  w << "   height: " << h << std::endl;
    setFixedHeight(norm_size);
    setFixedWidth(norm_size);

    double x0;
    double y0;
    scale_factor_X = norm_size / w;
    scale_factor_Y = norm_size / h;

    x0 = -gdata->a*scale_factor_X;
    y0 = (gdata->max)*scale_factor_Y;

    std::cout << "scaleX: " << scale_factor_X << " scaleY: " << scale_factor_Y << std::endl;
    std::cout << "x0: " << x0 << " y0: " << y0 << std::endl;

    draw->x0 = x0;
    draw->y0 = y0;
    draw->scaleX = scale_factor_X;
    draw->scaleY = scale_factor_Y;
    draw->max = gdata->max;
    draw->min = gdata->min;
    draw->a = gdata->a;
    draw->b = gdata->b;
    draw->n = gdata->n;


    QPainter *p = new QPainter(this);
    draw->DrawGraphic(p, gdata, width());
   switch(gdata->type)
    {
        case 1:
        {
            draw->drawFunction(p, gdata->a,gdata->b, [&](double x)->double { return gdata->f(x); }, 0);
            if(gdata->n <= 50) draw->drawFunction(p, gdata->a,gdata->b, [&](double x)->double { return gdata->AppFunc1(x); }, 1);
            break;
        }
        case 2:
        {
            draw->drawFunction(p, gdata->a,gdata->b, [&](double x)->double { return gdata->f(x); }, 0);
            draw->drawFunction(p, gdata->a,gdata->b, [&](double x)->double { return gdata->AppFunc2(x); }, 2);
            break;
        }
        case 3:
        {
            draw->drawFunction(p, gdata->a,gdata->b, [&](double x)->double { return gdata->f(x); }, 0);
            draw->drawFunction(p, gdata->a,gdata->b, [&](double x)->double { return gdata->AppFunc1(x); }, 1);
            draw->drawFunction(p, gdata->a,gdata->b, [&](double x)->double { return gdata->AppFunc2(x); }, 2);
            break;
        }
        default:
        {
            draw->drawFunction(p, gdata->a,gdata->b, [&](double x)->double { return gdata->AppFunc1(x) - gdata->f(x); }, 1);
            draw->drawFunction(p, gdata->a,gdata->b, [&](double x)->double { return gdata->AppFunc2(x) - gdata->f(x); }, 2);
            break;
        }
    }
    p->end();
}

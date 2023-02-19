#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "add.h"
#include "draw.h"


QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:


    ~Widget();
    Drawer* draw;
    GData* gdata;


    Widget(QWidget *parent = nullptr,
                  double a = 0,
                  double b = 0,
                  int    n = 100,
                  int    k = 0);

    const double norm_size = 500;
    double scale_factor_X;
    double scale_factor_Y;

private:
    Ui::Widget *ui;
protected:
    void keyPressEvent(QKeyEvent *event);
    void drawFunction(double func(double), double a, double b);
    void paintEvent(QPaintEvent *);
};
#endif // WIDGET_H

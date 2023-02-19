#include "widget.h"
#include <fstream>
#include <iostream>
#include <vector>
#include <cmath>
#include <QApplication>

using namespace std;


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    double a;
    double b;
    int n;
    int k;
    if(argc != 5)
    {
        std::cout << "incorrect number of arguments" << std::endl;
       return 0;
    }
    a = atof(argv[1]);
    b = atof(argv[2]);
    n = atoi(argv[3]);
    k = atoi(argv[4]);
    cout << "recieved args: " << a << " " << b << " " << n << " " << k << endl;
    Widget w(nullptr, a, b, n, k);
    w.show();
    return app.exec();
}


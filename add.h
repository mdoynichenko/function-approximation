#ifndef ADD_H
#define ADD_H

#include <fstream>
#include <vector>
#include <utility>
#include <map>
#include <functional>

class GData{
public:
    GData();
    GData(double a, double b, int n, int k);
    ~GData();

    std::vector<double> x;
    std::vector<double> y;
    std::vector<double> data1;
    std::vector<double> data2;
    double a;
    double b;
    int n;
    int k;
    int type = 1;
    int plus = 0;
    double f(double x);
    double f_clear(double x);
    double min;
    double max;
    double x_fixed;
    double maxf;

    const int g_steps = 64;
    void countMinMax(const std::function<double(double)> &tmpF, bool force_update);
    void countTMM();
    void countAbsMaxF();
    void updateF();
    void BuildAppFunc1();
    void BuildAppFunc2();
    double AppFunc1(double x);
    double AppFunc2(double x);
    void SolveTriangleGauss(double *matr, double *vec_b);
};


#endif // ADD_H


#include <iostream>
#include <fstream>
#include <thread>
#include <vector>
#include <cmath>
#include <map>
#include <utility>
#include "add.h"
#include <functional>


using namespace std;

double GData::f(double x)
{
    if(fabs(x - x_fixed) < 1e-10)
    {
        switch(k)
        {
        case 0:{
            return 1 + plus*0.1*maxf;
            break;
        }
        case 1:
        {
            return x+ plus*0.1*maxf;
            break;
        }
        case 2:
        {
            return x*x+ plus*0.1*maxf;
            break;
        }
        case 3:{
            return x*x*x+ plus*0.1*maxf;
            break;
        }
        case 4:{
            return x*x*x*x+ plus*0.1*maxf;
            break;
        }
        case 5:{
            return exp(x)+ plus*0.1*maxf;
            break;
        }
        default: {
            return 1/(25*x*x + 1) + plus*0.1*maxf;
            break;
        }
        }
    }
    switch(k)
    {
    case 0: {
        return 1;
        break;
    }
    case 1: {
        return x;
        break;
    }
    case 2: {
        return x*x;
        break;
    }
    case 3: {
        return x*x*x;
        break;
    }
    case 4: {
        return x*x*x*x;
        break;
    }
    case 5:
    {
        return exp(x);
        break;
    }
    default:
    {
        return 1/(25*x*x + 1);
        break;
    }
    }
}

double GData::f_clear(double x)
{
    switch(k)
    {
    case 0: {
        return 1;
        break;
    }
    case 1: {
        return x;
        break;
    }
    case 2: {
        return x*x;
        break;
    }
    case 3: {
        return x*x*x;
        break;
    }
    case 4: {
        return x*x*x*x;
        break;
    }
    case 5:
    {
        return exp(x);
        break;
    }
    default:
    {
        return 1/(25*x*x + 1);
        break;
    }
    }
}

void GData::BuildAppFunc1() // движущиеся многочлены второый степени
{
    vector<double> tmp;
    data1.clear();

    // коэфф-ты последнего многочлена
    tmp.push_back(y[n-3]);
    tmp.push_back((y[n-2] - y[n-3])/(x[n-2] - x[n-3]));
    tmp.push_back((y[n-1] - y[n-2])/(x[n-1] - x[n-2]));
    tmp[2] = (tmp[2] - tmp[1])/(x[n-1] - x[n-3]);

    // коэф-ты остальных многочленов

    for(size_t i = 1; i <= static_cast<size_t>(n-2); i++){
        tmp.push_back(y[n-3 - i]);
        tmp.push_back((y[n-3 - i+1] - y[n-3 - i])/(x[n-3 - i+1] - x[n-3 - i]));
        tmp.push_back((tmp[3*(i-1) + 1] - tmp[3*i + 1])/(x[n-3 - i+2] - x[n-3 - i]));
    }

    // перекидываем из tmp в a с исправлением порядка
   for(size_t j = 0; j <= static_cast<size_t>(n-3); j++){
       data1.push_back(tmp[3*(n-3-j)]);
       data1.push_back(tmp[3*(n-3-j) + 1]);
       data1.push_back(tmp[3*(n-3-j) + 2]);
   }
   tmp.clear();
}

double GData::AppFunc1(double t)
{
    double res = 0;
    size_t j = 0;
    for(size_t i = 0; i < static_cast<size_t>(n-1); i++)
    {
        if(t >= a + i*(b-a)/(n-1) && t <= a + (i+1)*(b-a)/(n-1))
        {
            j = i;
            break;
        }
    }
    if(j > static_cast<size_t>(n-3)) j = n-3;
    res = data1[3*j] + (t - x[j])*data1[3*j + 1] + (t - x[j])*(t - x[j+1])*data1[3*j + 2];
    return res;
}

double GData::AppFunc2(double t)
{
    double res = 0;
    double tmp_f;
    double c1,c2,c3,c4;
    size_t j = 0;
    for(size_t i = 0; i < static_cast<size_t>(n); i++)
    {
        if(t >= a + i*(b-a)/(n-1) && t <= a + (i+1)*(b-a)/(n-1))
        {
            j = i;
            break;
        }
    }

    tmp_f = (data2[n+j+1] - data2[n+j])/(data2[2*n + j+1] - data2[2*n + j]);
    c1 = data2[n+j];
    c2 = data2[j];
    c3 = (3*tmp_f - 2*data2[j] - data2[j+1])/(data2[2*n + j+1] - data2[2*n + j]);
    c4 = (data2[j] + data2[j+1] - 2*tmp_f)/((data2[2*n + j+1] - data2[2*n + j])*(data2[2*n + j+1] - data2[2*n + j]));

    res = c1 + c2*(t - data2[2*n + j]) + c3*(t - data2[2*n + j])*(t - data2[2*n + j]) + c4*(t - data2[2*n + j])*(t - data2[2*n + j])*(t - data2[2*n + j]);

    return res;
}

double max(double a, double b)
{
    if(a > b) return a;
    return b;
}
double min(double a, double b)
{
    if(a < b) return a;
    return b;
}

double dFunc(double p, size_t n, vector<double> x, vector<double> f)
{
    size_t k = 0;
    double k_dist = fabs(p - x[0]);
    for(size_t i = 0; i < n; i++)
    {
        if(fabs(p - x[i]) < k_dist)
        {
            k = i;
            k_dist = fabs(p - x[i]);
        }
    }

    if(k == 0)
    {
        return (f[0] - f[1])/(x[0] - x[1]);
    }
    else if(k == n-1)
    {
        return (f[n-1] - f[n-2])/(x[n-1] - x[n-2]);
    }
    else
    {
        return ((f[k] - f[k-1])/(x[k] - x[k-1]) + (f[k] - f[k+1])/(x[k] - x[k+1]))/2;
    }
}


double d2Func(double p, size_t n, vector<double> x, vector<double> f)
{
    double l_df;
    double r_df;
 //   double c_df;
    size_t k = 0;
    double k_dist = fabs(p - x[0]);
    for(size_t i = 0; i < n; i++)
    {
        if(fabs(p - x[i]) < k_dist)
        {
            k = i;
            k_dist = fabs(p - x[i]);
        }
    }

    if(k == 0)
    {
        l_df = (f[0] - f[1])/(x[0] - x[1]);
        r_df = (f[1] - f[2])/(x[1] - x[2]);
        return (l_df - r_df)/(x[0] - x[1]);
    }
    else if(k == n-1)
    {
        r_df = (f[n-1] - f[n-2])/(x[n-1] - x[n-2]);
        l_df = (f[n-2] - f[n-3])/(x[n-2] - x[n-3]);
        return (l_df - r_df)/(x[n-2] - x[n-1]);
    }
    else
    {
         l_df = (f[k] - f[k-1])/(x[k] - x[k-1]);
         r_df = (f[k] - f[k+1])/(x[k] - x[k+1]);
         return (l_df - r_df)/(x[k] - x[k+1]);
    }
}

void GData::SolveTriangleGauss(double *matr, double *vec_b)
{
    double *tmp_data;
    tmp_data = new double[n];
    std::cout << "hello triang 1" << std::endl;
    for(size_t i = 0; i < static_cast<size_t>(n); i++)
    {
//        std::cout << "i = " << i << "matr[0] = " << matr[0] <<  std::endl;
        if(fabs(matr[i*n + i]) < 1e-10)
        {
            cout << "Error, one of diagonal cells is equal to zero!!!" << endl;
        }
        if(i == 0)
        {
            std::cout << "YO" << std::endl;
            std::cout << "YO!" << std::endl;
            matr[i*n+1]/= matr[i*n];
            vec_b[i]/=matr[i*n];
            matr[i*n] = 1;

        }
        else if(i == static_cast<size_t>(n-1))
        {
//            matr[k*n+k-1]/= matr[k*n +k];
            vec_b[i]/=matr[i*n+i];
            matr[i*n +i] = 1;
        }
        else
        {
//            matr[k*n+k-1]/= matr[k*n +k];
            matr[i*n+i+1]/= matr[i*n + i];
            vec_b[i]/=matr[i*n + i];
            matr[i*n + i] = 1;
        }

        if(i < static_cast<size_t>(n-1))
        {
            matr[(i+1)*n + i+1] -= matr[(i+1)*n + i]*matr[i*n + i +1];
            vec_b[i+1] -= matr[(i+1)*n + i]*vec_b[i];
  /*          if(k > 0) // вроде не нужно, это ведь ноль
            {
                matr[(k+1)*n + k-1] -= matr[(k+1)*n + k]*matr[k*n + k-1];
            } */

            matr[(i+1)*n + i] = 0;

        }
    }

/*    // обратный ход метода Гаусса (без учета трехдиагональности)
    for(size_t i = 0; i < n; i++)
    {
        x[n-i] = b[n-i];
        for(size_t j = 0; j < i; j++)
        {
            x[n-i] -= x[n-j]*matr[(n-i)*n + n-j];
        }
    }
 */
    // обратный ход метода Гаусса (с учетом трехдиагональности)
    for(size_t i = 0; i < static_cast<size_t>(n); i++)
    {
        tmp_data[n-i] = vec_b[n-i];
        if(i>0)
        {
            tmp_data[n-i] -= matr[(n-i)*n +n-i+1]*x[n-i+1];
        }
    }
    data2.clear();
    for(size_t i = 0; i < static_cast<size_t>(n); i++)
    {
        data2.push_back(tmp_data[i]);
    }
    delete []tmp_data;
}


void GData::BuildAppFunc2()
{
    double *matr;
    matr = new double[n*n];
    double *vec_b;
    vec_b = new double[n];

    data2.clear();

    // условие из метода кубических сплайнов
    for(size_t i = 1; i < static_cast<size_t>(n-1); i++)
    {
        for(size_t j = 0; j < i-1; j++)
        {
            matr[i*n +j] = 0;
        }
        for(size_t j = i+2; j < static_cast<size_t>(n); j++)
        {
            matr[i*n +j] = 0;
        }
        matr[i*n + i-1] = x[i+1] - x[i];
        matr[i*n + i] = 2*(x[i+1] - x[i-1]);
        matr[i*n + i+1] = x[i] - x[i-1];
        vec_b[i] =  3*(x[i+1] - x[i])*(y[i] - y[i-1])/(x[i] - x[i-1]) + 3*(x[i] - x[i-1])*(y[i+1] - y[i])/(x[i+1] - x[i]);
    }

    std::cout << "hello 1" << std::endl;

    // условие для определения недостающих граничных условий по известным вторым производным
    for(size_t h = 0; h < static_cast<size_t>(n-2); h++)
    {
        matr[n-1-h] = 0;
        matr[n*(n-1) +h] = 0;
    }

    matr[0] = 2;
    matr[1] = 1;
    vec_b[0] = 3*(y[1] - y[0])/(x[1] - x[0]) - d2Func(x[0], n, x, y)*(x[1] -x[0])/2;

    matr[n*(n-1) + n-2] = 1;
    matr[n*(n-1) + n-1] = 2;
    vec_b[n-1] = 3*(y[n-1] - y[n-2])/(x[n-1] - x[n-2]) + d2Func(x[n-1], n, x, y)*(x[n-1] -x[n-2])/2;
    // условия определены
    std::cout << "hello 2" << std::endl;

    // решаем трехдиагональную систему методом Гаусса
    SolveTriangleGauss(matr, vec_b);

    std::cout << "hello 3" << std::endl;

    for(size_t j = 0; j < static_cast<size_t>(n); j++)
    {
        data2.push_back(y[j]);
    }
    for(size_t j = 0; j < static_cast<size_t>(n); j++)
    {
        data2.push_back(x[j]);
    }
    delete []matr;
    delete []vec_b;
}
int sMin(int a, int b)
{
    if(a > b) return b;
    return a;
}

int sMax(int a, int b)
{
    if(a < b) return b;
    return a;
}

GData::GData():
    a(0), b(1), n(10), k(0)
{
    updateF();
}

GData::GData(double a, double b, int n, int k):
    a(a), b(b), n(n), k(k)
{
    updateF();
}

void GData::countMinMax(const function<double(double)> &tmpF, bool force_update)
{
    double tmpMin = tmpF(a);
    double tmpMax = tmpF(a);
    double tmp_f = tmpF(a);
    for(int i = 0; i < g_steps; i++)
    {
        tmp_f = tmpF(a + (b-a)*i/(g_steps-1));
        if(tmp_f > tmpMax) tmpMax = tmp_f;
        if(tmp_f < tmpMin) tmpMin = tmp_f;
    }

    if(force_update == true)
    {
        this->max = tmpMax;
        this->min = tmpMin;
    }
    else
    {
        if(this->max < tmpMax)
        {
            this->max = tmpMax;
        }
        if(this->min > tmpMin)
        {
            this->min = tmpMin;
        }
    }
}

void GData::countAbsMaxF()
{
    double tmp_max = f_clear(a);
    double tmp_f = tmp_max;
    for(int i = 0; i < g_steps; i++)
    {
        tmp_f = f_clear(a + (b-a)*i/(g_steps-1));
        if(tmp_f > tmp_max) tmp_max = tmp_f;
    }
    maxf = tmp_max;
}


void GData::countTMM()
{
    if(n <= 50)
    {
        switch(type)
        {
        case 1:{
            countMinMax([&](double x) -> double {return f(x); }, true);
            countMinMax([&](double x) -> double {return AppFunc1(x); }, false);
            break;
        }
        case 2:{
            countMinMax([&](double x) -> double {return f(x); }, true);
            countMinMax([&](double x) -> double {return AppFunc2(x); }, false);
            break;
        }
        case 3:{
            countMinMax([&](double x) -> double {return f(x); }, true);
            countMinMax([&](double x) -> double {return AppFunc1(x); }, false);
            countMinMax([&](double x) -> double {return AppFunc2(x); }, false);
            break;
        }
        case 4:{
            countMinMax([&](double x) -> double {return AppFunc1(x) - f(x); }, true);
            countMinMax([&](double x) -> double {return AppFunc2(x) - f(x); }, false);
            break;
        }
        }
    }
    else
    {
        countMinMax([&](double x) -> double {return f(x); }, true);
    }
}


void GData::updateF()
{
    x_fixed = a + ((b-a)*(static_cast<size_t>(n/2)))/(n-1);
    countAbsMaxF();
    this->x.clear();
    this->y.clear();
    for(size_t i = 0; i < static_cast<size_t>(n); i++)
    {
        this->x.push_back(a + ((b-a)*(i))/(n-1));
        this->y.push_back(f(a + ((b-a)*i)/(n-1)));
    }
    if( n <= 50)
    {
        BuildAppFunc1();
        BuildAppFunc2();
    }
}

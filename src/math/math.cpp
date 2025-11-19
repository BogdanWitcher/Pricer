#include <cmath>

#include "../../include/math/numerical_methods.hpp"

using namespace math;

double math::standart_integral_gaussian_quadrature(double a, double b, int n, double (*func)(double))
{  
    double delta_x;
    double sum;

    double x_0;
    double x_plus;
    double x_minus;

    double t_0;
    double t_plus;
    double t_minus;

    double w_1;
    double w_2;
    double w_3;

    delta_x = 1 / ((double) n - 1);
    sum     = 0.0;

    t_0     = 0.5;
    t_plus  = 0.5 + 0.5 * std::sqrt(3.0 / 5);
    t_minus = 0.5 - 0.5 * std::sqrt(3.0 / 5);

    w_1 = 5.0 / 18;
    w_2 = 8.0 / 18;
    w_3 = 5.0 / 18;


    for(int i = 0; i < n - 1; ++i)
    {
        x_0      = (b - a) * (t_0 * delta_x + i * delta_x)     + a;
        x_plus   = (b - a) * (t_plus * delta_x + i * delta_x)  + a;
        x_minus  = (b - a) * (t_minus * delta_x + i * delta_x) + a;

        sum += delta_x * (w_1 * func(x_minus) + w_2 * func(x_0) + w_3 * func(x_plus));
    }

    sum *= (b-a);

    return sum;
}

double math::integral(double a, double b, int N, int n, double (*func)(double), double (*integralMethod)(double, double, int, double (*)(double)))
{
    double res;
    double step;

    step = (b - a) / (N - 1);
    res = 0.0;

    for(int i = 0; i < N - 1; ++i)
    {
        res += integralMethod(a + i * step, a + (i + 1) * step, n, func);
    }

    return res;
}

double math::normalPDF(double x)
{
    return (1.0 / std::sqrt(2 * M_PI)) * std::exp(-x * x / 2.0);
}


double math::normalCDF(double x)
{
    double PhiZero = 0.5;

    return PhiZero + math::integral(0, x, 10000, 100, math::normalPDF, math::standart_integral_gaussian_quadrature);
}


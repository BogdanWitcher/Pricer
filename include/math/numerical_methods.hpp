// include/math/numerical_methods.hpp
#pragma once

namespace math
{
    #ifndef M_PI
        #define M_PI 3.14159265358979323846
    #endif // M_PI

    double standart_integral_gaussian_quadrature(double a, double b, int n, double (*func)(double));
    double integral(double a, double b, int N, int n, double (*func)(double), double (*integralMethod)(double, double, int, double (*)(double)));
    double normalPDF(double x);
    double normalCDF(double x);

}; // namespace math
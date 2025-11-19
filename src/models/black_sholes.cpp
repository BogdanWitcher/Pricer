#include <cmath>

#include "../../include/models/black_sholes.hpp"
#include "../../include/math/numerical_methods.hpp"

using namespace black_sholes_model;

double BlackScholesModel::volatility(double strike, double timeToExpiration) const
{
    return __sigma;
}

double BlackScholesModel::riskRate(double time) const
{
    return __model->riskFreeRate;
}

double BlackScholesModel::BlackSholesFormulaCall(double spot, double strike, double timeToExpiration) const
{
    double d_1 = (std::log(spot / strike) + (__model->riskFreeRate + __sigma * __sigma / 2) * (timeToExpiration)) / (__sigma * std::sqrt(timeToExpiration));
    double d_2 = d_1 - __sigma * std::sqrt(timeToExpiration);
    double price = spot * math::normalCDF(d_1) - strike * std::exp(-__model->riskFreeRate * timeToExpiration) * math::normalCDF(d_2);

    return price;
}

double BlackScholesModel::impliedVolatilityNewton(double spot, double strike, double timeToExpiration, double callPrice, double tol, int maxIter) const
{
    if (timeToExpiration <= 0.0 || callPrice <= 0.0)
    {
        return 0.0;
    }

    double sigma = std::sqrt(2.0 * std::fabs(std::log(spot / strike)) / timeToExpiration);
    if(sigma < 1e-4)
    { 
        sigma = 0.20;        
    }

    for(int i = 0; i < maxIter; ++i)
    {
        double d1 = (std::log(spot / strike) + (__model->riskFreeRate + sigma * sigma / 2) * timeToExpiration) / (sigma * std::sqrt(timeToExpiration));
        double d2 = d1 - sigma * std::sqrt(timeToExpiration);
        double c  = spot * math::normalCDF(d1)- strike * std::exp(-__model->riskFreeRate * timeToExpiration) * math::normalCDF(d2);
        double vega = spot * math::normalPDF(d1) * std::sqrt(timeToExpiration);

        double diff = c - callPrice;
        if (std::fabs(diff) < tol) 
        {
            return sigma;
        }

        double ds = diff / vega;
        sigma -= ds;
        if(sigma <= 0.0) 
        {
            sigma = 1e-4;
        }    
    }
    
    return sigma;
}

void BlackScholesModel::colibrateModel()
{
    if(st == State::COLIBRATE)
    {
        return;
    }

    __sigma = impliedVolatilityNewton(__model->spotPrice, __model->strike, __model->timeToExpiration, __model->marketPrice);
    st = State::COLIBRATE;
}

std::string BlackScholesModel::getStatus()
{
    if(st == NONCOLIBRATE)
    {
        return "Non Colibrate";
    }

    return "Colibrate";
}

double OptionCall::payOff() const
{
    return std::max(0.0, __lastPrice - __strike);
}

double OptionCall::calculatePrice(double spot, double strike, double timeToExpiration, BlackScholesModel &bs) const
{
    return bs.BlackSholesFormulaCall(spot, strike, timeToExpiration);
}
#include <cmath>

#include "../../include/models/black_sholes.hpp"
#include "../../include/math/numerical_methods.hpp"

using namespace black_sholes_model;

double BlackScholesModel::volatility(double strike, double timeToExpiration) const
{
    return __sigma;
}

double BlackScholesModel::riskRate(double timeToExpiration) const
{
    return __model->riskFreeRate;
}

// double BlackScholesModel::impliedVolatilityNewton(double spot, double strike, double timeToExpiration, double callPrice, double tol, int maxIter) const
// {
//     if (timeToExpiration <= 0.0 || callPrice <= 0.0)
//     {
//         return 0.0;
//     }

//     double sigma = std::sqrt(2.0 * std::fabs(std::log(spot / strike)) / timeToExpiration);
//     if(sigma < 1e-4)
//     { 
//         sigma = 0.20;        
//     }

//     for(int i = 0; i < maxIter; ++i)
//     {
//         double d1 = (std::log(spot / strike) + (__model->riskFreeRate + sigma * sigma / 2) * timeToExpiration) / (sigma * std::sqrt(timeToExpiration));
//         double d2 = d1 - sigma * std::sqrt(timeToExpiration);
//         double c  = spot * math::normalCDF(d1)- strike * std::exp(-__model->riskFreeRate * timeToExpiration) * math::normalCDF(d2);
//         double vega = spot * math::normalPDF(d1) * std::sqrt(timeToExpiration);

//         double diff = c - callPrice;
//         if (std::fabs(diff) < tol) 
//         {
//             return sigma;
//         }

//         double ds = diff / vega;
//         sigma -= ds;
//         if(sigma <= 0.0) 
//         {
//             sigma = 1e-4;
//         }    
//     }
    
//     return sigma;
// }

double BlackScholesModel::impliedVolatilityNewton(double spot, double strike, double timeToExpiration, double callPrice, double tol, int maxIter) const
{
    if (timeToExpiration <= 0.0 || callPrice <= 0.0)
    {
        return 0.0;
    }

    double sigma = 0.3;

    for(int i = 0; i < maxIter; ++i)
    {
        double sqrtT = std::sqrt(timeToExpiration);
        double d1 = (std::log(spot / strike) + (__model->riskFreeRate + sigma * sigma / 2.0) * timeToExpiration) / (sigma * sqrtT);
        double d2 = d1 - sigma * sqrtT;
        
        double c = spot * math::normalCDF(d1) - strike * std::exp(-__model->riskFreeRate * timeToExpiration) * math::normalCDF(d2);
        double vega = spot * math::normalPDF(d1) * sqrtT;

        if (vega < 1e-10) {
            break;
        }

        double diff = c - callPrice;
        if (std::fabs(diff) < tol) 
        {
            return sigma;
        }

        sigma = sigma - diff / vega;
        
        if (std::fabs(sigma) < 1e-12) {
            sigma = 0.1;
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
    return std::max(0.0, __spotPrice - __strike);
}

double OptionCall::calculatePrice(BlackScholesModel &bs) const
{
    double d_1 = (std::log(__spotPrice / __strike) + (bs.riskRate(__timeToExpiration) + bs.volatility(__strike, __timeToExpiration) * 
                    bs.volatility(__strike, __timeToExpiration) / 2) * (__timeToExpiration)) / (bs.volatility(__strike, __timeToExpiration) 
                    * std::sqrt(__timeToExpiration));

    double d_2 = d_1 - bs.volatility(__strike, __timeToExpiration) * std::sqrt(__timeToExpiration);
    double price = __spotPrice * math::normalCDF(d_1) - __strike * std::exp(-bs.riskRate(__timeToExpiration) * __timeToExpiration) * math::normalCDF(d_2);

    return price;
}

double OptionCall::calculateDelta(BlackScholesModel &bs)  const
{
    double d_1 = (std::log(__spotPrice / __strike) + (bs.riskRate(__timeToExpiration) + bs.volatility(__strike, __timeToExpiration) * 
                    bs.volatility(__strike, __timeToExpiration) / 2) * (__timeToExpiration)) / (bs.volatility(__strike, __timeToExpiration) 
                    * std::sqrt(__timeToExpiration));

    return math::normalCDF(d_1);
}

double OptionCall::calculateGamma(BlackScholesModel &bs)  const
{
    double d_1 = (std::log(__spotPrice / __strike) + (bs.riskRate(__timeToExpiration) + bs.volatility(__strike, __timeToExpiration) * 
                    bs.volatility(__strike, __timeToExpiration) / 2) * (__timeToExpiration)) / (bs.volatility(__strike, __timeToExpiration) 
                    * std::sqrt(__timeToExpiration));

    return math::normalPDF(d_1) / (__spotPrice * bs.volatility(__strike, __timeToExpiration) * std::sqrt(__timeToExpiration));
}

double OptionCall::calculateVega(BlackScholesModel &bs) const
{
    double d_1 = (std::log(__spotPrice / __strike) + (bs.riskRate(__timeToExpiration) + bs.volatility(__strike, __timeToExpiration) * 
                    bs.volatility(__strike, __timeToExpiration) / 2) * (__timeToExpiration)) / (bs.volatility(__strike, __timeToExpiration) 
                    * std::sqrt(__timeToExpiration));

    return math::normalPDF(d_1) * __spotPrice * std::sqrt(__timeToExpiration);
}

double OptionPut::payOff() const
{
    return std::max(__strike - __spotPrice, 0.0);
}

double OptionPut::calculatePrice(BlackScholesModel &bs) const
{
    double d_1 = (std::log(__spotPrice / __strike) + (bs.riskRate(__timeToExpiration) + bs.volatility(__strike, __timeToExpiration) * 
                    bs.volatility(__strike, __timeToExpiration) / 2) * (__timeToExpiration)) / (bs.volatility(__strike, __timeToExpiration) 
                    * std::sqrt(__timeToExpiration));
    
    double d_2 = d_1 - bs.volatility(__strike, __timeToExpiration) * std::sqrt(__timeToExpiration);
    double price = -__spotPrice * math::normalCDF(-d_1) + __strike * std::exp(-bs.riskRate(__timeToExpiration) * __timeToExpiration) * math::normalCDF(-d_2);

    return price;
}

double OptionPut::calculateDelta(BlackScholesModel &bs)  const
{
    double d_1 = (std::log(__spotPrice / __strike) + (bs.riskRate(__timeToExpiration) + bs.volatility(__strike, __timeToExpiration) * 
                    bs.volatility(__strike, __timeToExpiration) / 2) * (__timeToExpiration)) / (bs.volatility(__strike, __timeToExpiration) 
                    * std::sqrt(__timeToExpiration));

    return math::normalCDF(d_1) - 1.0;
}

double OptionPut::calculateGamma(BlackScholesModel &bs)  const
{
    double d_1 = (std::log(__spotPrice / __strike) + (bs.riskRate(__timeToExpiration) + bs.volatility(__strike, __timeToExpiration) * 
                    bs.volatility(__strike, __timeToExpiration) / 2) * (__timeToExpiration)) / (bs.volatility(__strike, __timeToExpiration) 
                    * std::sqrt(__timeToExpiration));

    return math::normalPDF(d_1) / (__spotPrice * bs.volatility(__strike, __timeToExpiration) * std::sqrt(__timeToExpiration));
}

double OptionPut::calculateVega(BlackScholesModel &bs) const
{
    double d_1 = (std::log(__spotPrice / __strike) + (bs.riskRate(__timeToExpiration) + bs.volatility(__strike, __timeToExpiration) * 
                    bs.volatility(__strike, __timeToExpiration) / 2) * (__timeToExpiration)) / (bs.volatility(__strike, __timeToExpiration) 
                    * std::sqrt(__timeToExpiration));

    return math::normalPDF(d_1) * __spotPrice * std::sqrt(__timeToExpiration);
}

double BarrierOption::solvePDE(BlackScholesModel &bs) const 
{
    // Параметры модели
    double r = bs.riskRate(__timeToExpiration);
    double sigma = bs.volatility(__strike, __timeToExpiration);
    
    // Простая реализация метода конечных разностей
    int timeSteps = 100;
    int priceSteps = 100;
    
    double dt = __timeToExpiration / timeSteps;
    double maxPrice = 2.0 * std::max(__spotPrice, __strike); // Максимальная цена для сетки
    
    // Создаем сетку цен
    std::vector<double> prices(priceSteps + 1);
    for(int i = 0; i <= priceSteps; ++i) 
    {
        prices[i] = i * maxPrice / priceSteps;
    }
    
    // Начальные условия (в момент экспирации)
    std::vector<double> V(priceSteps + 1);
    for(int i = 0; i <= priceSteps; ++i) 
    {
        V[i] = std::max(prices[i] - __strike, 0.0); // Payoff для call
    }
    
    // Применяем граничные условия для барьера
    for(int i = 0; i <= priceSteps; ++i) 
    {
        if(__barrierType == "down-and-out" && prices[i] <= __barrier) 
        {
            V[i] = 0.0;
        }
        else if(__barrierType == "up-and-out" && prices[i] >= __barrier) 
        {
            V[i] = 0.0;
        }
        // Для knock-in опционов начальное условие другое
        else if(__barrierType == "down-and-in" && prices[i] > __barrier) 
        {
            V[i] = 0.0;
        }
        else if(__barrierType == "up-and-in" && prices[i] < __barrier) 
        {
            V[i] = 0.0;
        }
    }
    
    // Обратный ход по времени (явная схема)
    for(int n = timeSteps - 1; n >= 0; --n) 
    {
        std::vector<double> V_new = V;
        
        for (int i = 1; i < priceSteps; ++i) 
        {
            double S = prices[i];
            double dS = prices[i+1] - prices[i];
            double dS2 = dS * dS;
            
            // Коэффициенты УРЧП
            double alpha = 0.5 * sigma * sigma * S * S / dS2;
            double beta = r * S / (2 * dS);
            double gamma = r;
            
            // Явная схема
            V_new[i] = V[i] + dt * (
                alpha * (V[i+1] - 2 * V[i] + V[i-1]) +
                beta * (V[i+1] - V[i-1]) -
                gamma * V[i]
            );
            
            // Применяем граничные условия на каждом шаге
            if (__barrierType.find("out") != std::string::npos) 
            {
                if ((__barrierType == "down-and-out" && S <= __barrier) ||
                    (__barrierType == "up-and-out" && S >= __barrier)) 
                {
                    V_new[i] = 0.0;
                }
            }
        }
        
        V = V_new;
    }
    
    // Находим цену для текущего spot
    int index = static_cast<int>(__spotPrice * priceSteps / maxPrice);
    if(index < 0) 
    {
        index = 0;
    }

    if(index > priceSteps) 
    {
        index = priceSteps;
    }
    
    return V[index];
}

double BarrierOption::calculatePrice(BlackScholesModel &bs) const 
{
    return solvePDE(bs);
}

double BarrierOption::calculateDelta(BlackScholesModel &bs) const
{
    return 0.0;
}

double BarrierOption::calculateGamma(BlackScholesModel &bs) const
{
    return 0.0;
}

double BarrierOption::calculateVega(BlackScholesModel &bs) const
{
    return 0.0;
}
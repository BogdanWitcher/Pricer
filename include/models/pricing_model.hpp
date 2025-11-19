// include/models/PricingModel.hpp
#pragma once

#include <vector>
#include <string>

#include "../market/market_data.hpp"

class PricingModel
{
public:
    virtual ~PricingModel() = default;

    // Неконстантные данные
    virtual double volatility(double strike, double timeToExpiration) const = 0;
    virtual double riskRate(double time) const = 0;

    // колибровка
    virtual void colibrateModel() = 0;
    
    // Получение имени модели
    virtual std::string getName() const = 0;
    
};
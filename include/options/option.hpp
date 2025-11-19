// include/options/Option.hpp
#pragma once

#include <memory>

#include "../models/pricing_model.hpp"

class Option
{
protected:

    double __strike;
    double __timeToExpiration;
    std::shared_ptr<PricingModel> __model;

public:

    Option(double strike, double timeTpExpiration, std::shared_ptr<PricingModel> model):
    __strike(strike),
    __timeToExpiration(timeTpExpiration),
    __model(model)
    {

    }

    virtual ~Option() = default;

    // Геттеры
    double getStrike() const {return __strike;}
    double getTimeToExpiration() const {return __timeToExpiration;}

    // получение типа опциона
    virtual std::string getType() const = 0;

};
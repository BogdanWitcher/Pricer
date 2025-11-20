// include/models/BlackScholesModel.hpp
#pragma once

#include <string>
#include <memory>

#include "../../include/models/pricing_model.hpp"
#include "../../include/market/market_data.hpp"

namespace black_sholes_model
{
    class BlackScholesModel : public PricingModel
    {
    private:
        
        enum State
        {
            COLIBRATE,
            NONCOLIBRATE
        };

        State st = State::NONCOLIBRATE;
        double __sigma;

        double impliedVolatilityNewton(double spot, double strike, double timeToExpiration, double callPrice, double tol = 1e-6, int maxIter = 50) const;

    protected:

        std::shared_ptr<MarketData> __model;

    public:

        BlackScholesModel(std::shared_ptr<MarketData> model):
        __model(model)
        {

        }

        ~BlackScholesModel() override
        {

        }

        // Волатильность и безрисковая ставка
        double volatility(double strike, double timeToExpiration) const override;
        double riskRate(double time) const override;
        double BlackSholesFormulaCall(double spot, double strike, double timeToExpiration) const;

        void colibrateModel() override;


        std::string getStatus();
        std::string getName() const override {return "Black-Scholes";}
    };

    class Option 
    {
    public:

        virtual double calculatePrice(double spot, double strike, double timeToExpiration, BlackScholesModel &bs) const = 0;
        // virtual double calculateDelta(double spot, double strike, double timeToExpiration, BlackScholesModel &bs)  const = 0;
        // virtual double calculateGamma(double spot, double strike, double timeToExpiration, BlackScholesModel &bs)  const = 0;
        // virtual double calculateVega(double spot, double strike, double timeToExpiration, BlackScholesModel &bs) const = 0;

        // Получение типа опциона
        virtual std::string getType() const = 0;
    };

    class VanillaOption : public Option
    {
    protected:
        
        double __strike;
        double __timeToExpiration;

    public:

        VanillaOption(double strike, double timeToExpiration):
        __strike(strike),
        __timeToExpiration(timeToExpiration)
        {

        }

        // Функция выплаты
        virtual double payOff() const = 0;
    };

    class OptionCall : public VanillaOption
    {
    private:

        double __spotPrice;

    public:
        
        OptionCall(double spotPrice, double strike, double timeToExpiration): 
        VanillaOption(strike, timeToExpiration),
        __spotPrice(spotPrice)
        {
    
        };

        // Функция выплаты
        double payOff() const override;

        // Методы рассчета цен и греков
        double calculatePrice(double spot, double strike, double timeToExpiration, BlackScholesModel &bs) const override;
        // double calculateDelta(double spot, double strike, double timeToExpiration, BlackScholesModel &bs) const override;
        // double calculateGamma(double spot, double strike, double timeToExpiration, BlackScholesModel &bs) const override;
        // double calculateVega(double spot, double strike, double timeToExpiration, BlackScholesModel &bs) const override;
        
        // Получение типа опциона
        std::string getType() const override {return "call";}

    };


} //black_sholes_model
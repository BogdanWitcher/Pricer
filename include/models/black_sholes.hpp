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
        double riskRate(double timeToExpiration) const override;

        void colibrateModel() override;


        std::string getStatus();
        std::string getName() const override {return "Black-Scholes";}
    };

    class Option 
    {
    public:

        virtual double calculatePrice(BlackScholesModel &bs) const = 0;
        virtual double calculateDelta(BlackScholesModel &bs)  const = 0;
        virtual double calculateGamma(BlackScholesModel &bs)  const = 0;
        virtual double calculateVega(BlackScholesModel &bs) const = 0;

        // Получение типа опциона
        virtual std::string getType() const = 0;
    };

    class VanillaOption : public Option
    {
    protected:
        
        double __strike;
        double __timeToExpiration;
        double __spotPrice;

    public:

        VanillaOption(double spotPrice, double strike, double timeToExpiration):
        __spotPrice(spotPrice),
        __strike(strike),
        __timeToExpiration(timeToExpiration)
        {

        }

        // Функция выплаты
        virtual double payOff() const = 0;
    };

    class OptionCall : public VanillaOption
    {
    public:
        
        OptionCall(double spotPrice, double strike, double timeToExpiration): 
        VanillaOption(spotPrice, strike, timeToExpiration)
        {
    
        };

        // Функция выплаты
        double payOff() const override;

        // Методы рассчета цен и греков
        double calculatePrice(BlackScholesModel &bs) const override;
        double calculateDelta(BlackScholesModel &bs) const override;
        double calculateGamma(BlackScholesModel &bs) const override;
        double calculateVega(BlackScholesModel &bs) const override;
        
        // Получение типа опциона
        std::string getType() const override {return "call";}

    };

    class OptionPut : public VanillaOption
    {
    public:
        
        OptionPut(double spotPrice, double strike, double timeToExpiration): 
        VanillaOption(spotPrice, strike, timeToExpiration)
        {
    
        };

        // Функция выплаты
        double payOff() const override;

        // Методы рассчета цен и греков
        double calculatePrice(BlackScholesModel &bs) const override;
        double calculateDelta(BlackScholesModel &bs) const override;
        double calculateGamma(BlackScholesModel &bs) const override;
        double calculateVega(BlackScholesModel &bs) const override;
        
        // Получение типа опциона
        std::string getType() const override {return "put";}

    };

    class BarrierOption : public Option
    {
    private:

        double __spotPrice;
        double __strike;
        double __timeToExpiration;
        double __barrier;
        std::string __barrierType; // "up-and-out", "down-and-out", "up-and-in", "down-and-in"

        double solvePDE(BlackScholesModel &bs) const;

    public:
        BarrierOption(double spotPrice, double strike, double timeToExpiration, double barrier, const std::string& type): 
        __spotPrice(spotPrice),
        __strike(strike),
        __timeToExpiration(timeToExpiration),
        __barrier(barrier), 
        __barrierType(type) 
        {

        }

        double calculatePrice(BlackScholesModel &bs) const override;
        // double calculateDelta(BlackScholesModel &bs) const override;
        // double calculateGamma(BlackScholesModel &bs) const override;
        // double calculateVega(BlackScholesModel &bs) const override;
        
        std::string getType() const override {return "barrier_" + __barrierType;}
    };


} //black_sholes_model
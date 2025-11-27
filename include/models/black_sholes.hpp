// include/models/BlackScholesModel.hpp
#pragma once

#include <string>
#include <memory>

#include "../../include/models/pricing_model.hpp"
#include "../../include/market/market_data.hpp"

/**
 * @brief Пространство имен для модели Блэка-Шоулза
 * 
 * Содержит реализацию классической модели оценки опционов,
 * включая различные типы опционов и методы расчета греков.
 */
namespace black_sholes_model
{
    /**
     * @brief Класс модели Блэка-Шоулза для оценки опционов
     * 
     * Реализует методы расчета цен опционов, волатильности,
     * а также калибровку модели по рыночным данным.
     */
    class BlackScholesModel : public PricingModel
    {
    private:
        
        /// @brief Состояние модели (калибрована/не калибрована)
        enum State
        {
            COLIBRATE,
            NONCOLIBRATE
        };

        State st = State::NONCOLIBRATE;  ///< Текущее состояние модели
        double __sigma;                  ///< Рассчитанная волатильность

        /**
         * @brief Расчет подразумеваемой волатильности методом Ньютона
         * @param spot Цена спот
         * @param strike Цена страйк
         * @param timeToExpiration Время до экспирации
         * @param callPrice Цена опциона колл
         * @param tol Точность расчета
         * @param maxIter Максимальное количество итераций
         * @return Подразумеваемая волатильность
         */
        double impliedVolatilityNewton(double spot, double strike, double timeToExpiration, double callPrice, double tol = 1e-6, int maxIter = 50) const;

    protected:

        std::shared_ptr<MarketData> __model;  ///< Рыночные данные для модели

    public:

        /**
         * @brief Конструктор модели
         * @param model Указатель на рыночные данные
         */
        BlackScholesModel(std::shared_ptr<MarketData> model):
        __model(model)
        {

        }

        ~BlackScholesModel() override
        {

        }

        /**
         * @brief Расчет волатильности
         * @param strike Цена страйк
         * @param timeToExpiration Время до экспирации
         * @return Значение волатильности
         */
        double volatility(double strike, double timeToExpiration) const override;
        
        /**
         * @brief Расчет безрисковой ставки
         * @param timeToExpiration Время до экспирации
         * @return Значение безрисковой ставки
         */
        double riskRate(double timeToExpiration) const override;

        /**
         * @brief Калибровка модели по рыночным данным
         */
        void colibrateModel() override;

        /**
         * @brief Получение статуса модели
         * @return Строка с статусом модели
         */
        std::string getStatus();
        
        /**
         * @brief Получение имени модели
         * @return "Black-Scholes"
         */
        std::string getName() const override {return "Black-Scholes";}
    };

    /**
     * @brief Абстрактный базовый класс для опционов
     * 
     * Определяет интерфейс для расчета цен и греков различных типов опционов.
     */
    class Option 
    {
    public:

        virtual ~Option() = default;

        /**
         * @brief Расчет теоретической цены опциона
         * @param bs Модель Блэка-Шоулза
         * @return Теоретическая цена опциона
         */
        virtual double calculatePrice(BlackScholesModel &bs) const = 0;
        
        /**
         * @brief Расчет дельты опциона
         * @param bs Модель Блэка-Шоулза
         * @return Значение дельты
         */
        virtual double calculateDelta(BlackScholesModel &bs)  const = 0;
        
        /**
         * @brief Расчет гаммы опциона
         * @param bs Модель Блэка-Шоулза
         * @return Значение гаммы
         */
        virtual double calculateGamma(BlackScholesModel &bs)  const = 0;
        
        /**
         * @brief Расчет веги опциона
         * @param bs Модель Блэка-Шоулза
         * @return Значение веги
         */
        virtual double calculateVega(BlackScholesModel &bs) const = 0;

        /**
         * @brief Получение типа опциона
         * @return Строка с типом опциона
         */
        virtual std::string getType() const = 0;
    };

    /**
     * @brief Базовый класс для ванильных опционов
     * 
     * Содержит общие параметры для опционов колл и пут.
     */
    class VanillaOption : public Option
    {
    protected:
        
        double __strike;              ///< Цена исполнения
        double __timeToExpiration;    ///< Время до экспирации
        double __spotPrice;           ///< Текущая цена базового актива

    public:

        /**
         * @brief Конструктор ванильного опциона
         * @param spotPrice Цена спот
         * @param strike Цена страйк
         * @param timeToExpiration Время до экспирации
         */
        VanillaOption(double spotPrice, double strike, double timeToExpiration):
        __spotPrice(spotPrice),
        __strike(strike),
        __timeToExpiration(timeToExpiration)
        {

        }

        /**
         * @brief Функция выплаты опциона
         * @return Выплата при экспирации
         */
        virtual double payOff() const = 0;
    };

    /**
     * @brief Класс для опционов колл (call options)
     */
    class OptionCall : public VanillaOption
    {
    public:
        
        /**
         * @brief Конструктор опциона колл
         * @param spotPrice Цена спот
         * @param strike Цена страйк
         * @param timeToExpiration Время до экспирации
         */
        OptionCall(double spotPrice, double strike, double timeToExpiration): 
        VanillaOption(spotPrice, strike, timeToExpiration)
        {
    
        };

        /**
         * @brief Функция выплаты для опциона колл
         * @return max(Spot - Strike, 0)
         */
        double payOff() const override;

        /**
         * @brief Расчет цены опциона колл
         * @param bs Модель Блэка-Шоулза
         * @return Теоретическая цена
         */
        double calculatePrice(BlackScholesModel &bs) const override;
        
        /**
         * @brief Расчет дельты опциона колл
         * @param bs Модель Блэка-Шоулза
         * @return Значение дельты
         */
        double calculateDelta(BlackScholesModel &bs) const override;
        
        /**
         * @brief Расчет гаммы опциона колл
         * @param bs Модель Блэка-Шоулза
         * @return Значение гаммы
         */
        double calculateGamma(BlackScholesModel &bs) const override;
        
        /**
         * @brief Расчет веги опциона колл
         * @param bs Модель Блэка-Шоулза
         * @return Значение веги
         */
        double calculateVega(BlackScholesModel &bs) const override;
        
        /**
         * @brief Получение типа опциона
         * @return "call"
         */
        std::string getType() const override {return "call";}

    };

    /**
     * @brief Класс для опционов пут (put options)
     */
    class OptionPut : public VanillaOption
    {
    public:
        
        /**
         * @brief Конструктор опциона пут
         * @param spotPrice Цена спот
         * @param strike Цена страйк
         * @param timeToExpiration Время до экспирации
         */
        OptionPut(double spotPrice, double strike, double timeToExpiration): 
        VanillaOption(spotPrice, strike, timeToExpiration)
        {
    
        };

        /**
         * @brief Функция выплаты для опциона пут
         * @return max(Strike - Spot, 0)
         */
        double payOff() const override;

        /**
         * @brief Расчет цены опциона пут
         * @param bs Модель Блэка-Шоулза
         * @return Теоретическая цена
         */
        double calculatePrice(BlackScholesModel &bs) const override;
        
        /**
         * @brief Расчет дельты опциона пут
         * @param bs Модель Блэка-Шоулза
         * @return Значение дельты
         */
        double calculateDelta(BlackScholesModel &bs) const override;
        
        /**
         * @brief Расчет гаммы опциона пут
         * @param bs Модель Блэка-Шоулза
         * @return Значение гаммы
         */
        double calculateGamma(BlackScholesModel &bs) const override;
        
        /**
         * @brief Расчет веги опциона пут
         * @param bs Модель Блэка-Шоулза
         * @return Значение веги
         */
        double calculateVega(BlackScholesModel &bs) const override;
        
        /**
         * @brief Получение типа опциона
         * @return "put"
         */
        std::string getType() const override {return "put";}

    };

    /**
     * @brief Класс для барьерных опционов
     * 
     * Реализует расчет цен для барьерных опционов различных типов:
     * up-and-out, down-and-out, up-and-in, down-and-in
     */
    class BarrierOption : public Option
    {
    private:

        double __spotPrice;           ///< Текущая цена базового актива
        double __strike;              ///< Цена исполнения
        double __timeToExpiration;    ///< Время до экспирации
        double __barrier;             ///< Барьерный уровень
        std::string __barrierType;    ///< Тип барьера

        /**
         * @brief Решение уравнения в частных производных для барьерного опциона
         * @param bs Модель Блэка-Шоулза
         * @return Цена опциона
         */
        double solvePDE(BlackScholesModel &bs) const;

    public:
    
        /**
         * @brief Конструктор барьерного опциона
         * @param spotPrice Цена спот
         * @param strike Цена страйк
         * @param timeToExpiration Время до экспирации
         * @param barrier Барьерный уровень
         * @param type Тип барьера
         */
        BarrierOption(double spotPrice, double strike, double timeToExpiration, double barrier, const std::string& type): 
        __spotPrice(spotPrice),
        __strike(strike),
        __timeToExpiration(timeToExpiration),
        __barrier(barrier), 
        __barrierType(type) 
        {

        }

        /**
         * @brief Расчет цены барьерного опциона
         * @param bs Модель Блэка-Шоулза
         * @return Теоретическая цена
         */
        double calculatePrice(BlackScholesModel &bs) const override;
        
        // Методы для греков пока не реализованы
        double calculateDelta(BlackScholesModel &bs) const override;
        double calculateGamma(BlackScholesModel &bs) const override;
        double calculateVega(BlackScholesModel &bs) const override;
        
        /**
         * @brief Получение типа опциона
         * @return Строка с типом барьерного опциона
         */
        std::string getType() const override {return "barrier_" + __barrierType;}
    };


} //black_sholes_model
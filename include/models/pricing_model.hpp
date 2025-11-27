// include/models/PricingModel.hpp
#pragma once

#include <vector>
#include <string>

#include "../market/market_data.hpp"

/**
 * @brief Абстрактный базовый класс для моделей оценки опционов
 * 
 * Определяет общий интерфейс для всех моделей ценообразования,
 * обеспечивая единообразие использования различных моделей.
 */
class PricingModel
{
public:
    virtual ~PricingModel() = default;

    /**
     * @brief Расчет волатильности
     * @param strike Цена страйк
     * @param timeToExpiration Время до экспирации
     * @return Значение волатильности
     */
    virtual double volatility(double strike, double timeToExpiration) const = 0;
    
    /**
     * @brief Расчет безрисковой ставки
     * @param time Время до экспирации
     * @return Значение безрисковой ставки
     */
    virtual double riskRate(double time) const = 0;

    /**
     * @brief Калибровка модели по рыночным данным
     * 
     * Настройка параметров модели для соответствия наблюдаемым рыночным ценам
     */
    virtual void colibrateModel() = 0;
    
    /**
     * @brief Получение имени модели
     * @return Строка с названием модели
     */
    virtual std::string getName() const = 0;
    
};
// include/market/market_data.hpp
#pragma once

#include <string>
#include <vector>

/**
 * @brief Структура для хранения рыночных данных по опционам
 * 
 * Содержит все необходимые параметры для расчета цен опционов
 * по моделям.
 */
struct MarketData 
{
    std::string ticker; ///< Тикер инструмента
    double spotPrice;   ///< Текущая цена базового актива
    double strike;      ///< Цена исполнения опциона
    double timeToExpiration; ///< Время до экспирации (в годах)
    double marketPrice; ///< Рыночная цена опциона
    double riskFreeRate;    ///< Безрисковая процентная ставка
    std::string optionType; ///< Тип опциона ("call" или "put")

    /**
     * @brief Конструктор по умолчанию
     * 
     * Инициализирует все числовые поля нулевыми значениями
     */
    MarketData(): 
    spotPrice(0.0), 
    strike(0.0), 
    timeToExpiration(0.0), 
    marketPrice(0.0), 
    riskFreeRate(0.0) 
    {
        
    }
    
};

/**
 * @brief Абстрактный класс для загрузки рыночных данных
 * 
 * Определяет интерфейс для загрузки данных из различных источников:
 * CSV файлов, API, ручного ввода и т.д.
 */
class MarketDataLoader 
{
public:

    /**
     * @brief Загрузка данных из CSV файла
     * @param filename Имя CSV файла
     * @return Структура MarketData с загруженными данными
     */
    virtual MarketData loadFromCSV(const std::string& filename) = 0;

    /**
     * @brief Загрузка данных через API
     * @param symbol Символ инструмента
     * @return Структура MarketData с данными из API
     */
    virtual MarketData loadFromAPI(const std::string& symbol) = 0;

    /**
     * @brief Загрузка данных через ручной ввод
     * @return Структура MarketData с введенными данными
     */
    virtual MarketData loadManualInput() = 0;
    
    /**
     * @brief Валидация рыночных данных
     * @param data Данные для проверки
     * @return true если данные корректны, false в противном случае
     */
    static bool validateMarketData(const MarketData& data);
};
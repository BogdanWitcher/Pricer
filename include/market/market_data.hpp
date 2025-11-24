// include/market/market_data.hpp
#pragma once

#include <string>
#include <vector>

struct MarketData 
{
    std::string ticker;
    double spotPrice;
    double strike;
    double timeToExpiration;
    double marketPrice;
    double riskFreeRate;
    std::string optionType;

    MarketData(): 
    spotPrice(0.0), 
    strike(0.0), 
    timeToExpiration(0.0), 
    marketPrice(0.0), 
    riskFreeRate(0.0) 
    {
        
    }
    
};

// Функции для работы с MarketData
class MarketDataLoader 
{
public:
    virtual ~MarketDataLoader() = default;
    
    // Загрузка данных из разных источников
    virtual MarketData loadFromCSV(const std::string& filename) = 0;
    virtual MarketData loadFromAPI(const std::string& symbol) = 0;
    virtual MarketData loadManualInput() = 0;
    
    // Валидация данных
    static bool validateMarketData(const MarketData& data);
};
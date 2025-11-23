// src/market/market_data_loader.cpp
#include <iostream>
#include <limits>

#include "../../include/market/market_data.hpp"
#include "../../include/market/csv_parser.hpp"

bool MarketDataLoader::validateMarketData(const MarketData& data) 
{
    if(data.spotPrice <= 0) 
    {
        std::cerr << "Invalid spot price: " << data.spotPrice << std::endl;
        return false;
    }
    if(data.strike <= 0) 
    {
        std::cerr << "Invalid strike price: " << data.strike << std::endl;
        return false;
    }
    if(data.timeToExpiration < 0) 
    {
        std::cerr << "Invalid time to expiration: " << data.timeToExpiration << std::endl;
        return false;
    }
    if(data.riskFreeRate < 0 || data.riskFreeRate > 1) 
    {
        std::cerr << "Invalid risk free rate: " << data.riskFreeRate << std::endl;
        return false;
    }
    
    return true;
}

// Пример реализации конкретного загрузчика
class SimpleMarketDataLoader : public MarketDataLoader 
{
public:
    MarketData loadFromCSV(const std::string& filename) override {
        CSVParser parser(filename);
        auto allData = parser.parseMarketData();
        
        if (!allData.empty()) {
            return allData[0]; // Возвращаем первую запись
        }
        
        return MarketData{};
    }
    
    MarketData loadFromAPI(const std::string& symbol) override {
        // ЗАГЛУШКА - здесь будет реальный API вызов
        // Например к Yahoo Finance, Alpha Vantage и т.д.
        
        MarketData data;
        data.ticker = symbol;
        data.spotPrice = 150.0;    // Пример: цена Apple
        data.riskFreeRate = 0.05;  // 5% ставка (можно взять с сайта ЦБ)
        
        std::cout << "Введите недостающие данные для " << symbol << ":\n";
        std::cout << "Strike price: ";
        std::cin >> data.strike;
        std::cout << "Time to expiration (лет): ";
        std::cin >> data.timeToExpiration;
        std::cout << "Market price: ";
        std::cin >> data.marketPrice;
        
        data.optionType = "call";
        
        return data;
    }
    
    MarketData loadManualInput() override {
        MarketData data;
        
        std::cout << "=== Ручной ввод рыночных данных ===\n";
        std::cout << "Ticker: ";
        std::cin >> data.ticker;
        std::cout << "Spot price: ";
        std::cin >> data.spotPrice;
        std::cout << "Strike price: ";
        std::cin >> data.strike;
        std::cout << "Time to expiration (лет): ";
        std::cin >> data.timeToExpiration;
        std::cout << "Market price: ";
        std::cin >> data.marketPrice;
        std::cout << "Risk-free rate (например 0.05 для 5%): ";
        std::cin >> data.riskFreeRate;
        std::cout << "Option type (call/put): ";
        std::cin >> data.optionType;
        
        
        if (!validateMarketData(data)) {
            std::cerr << "Invalid data entered!\n";
            return MarketData{};
        }
        
        return data;
    }
};
// src/market/csv_parser.cpp
#include <iostream>
#include <algorithm>

#include "../../include/market/csv_parser.hpp"

// Вспомогательная функция для преобразования даты
double dateStringToDays(const std::string& dateStr) 
{
    // Парсим дату вида "Fri Oct 03 2025"
    // В реальной реализации нужно полноценно парсить дату
    // Здесь упрощенный вариант - возвращаем дни до экспирации
    // В реальном коде нужно вычислять разницу между текущей датой и датой экспирации
    return 30.0; // временное значение
}

double cleanNumber(const std::string& str) 
{
    std::string cleaned;
    for(char c : str) 
    {
        if (std::isdigit(c) || c == '.' || c == '-') 
        {
            cleaned += c;
        }
    }

    return cleaned.empty() ? 0.0 : std::stod(cleaned);
}

std::vector<MarketData> CSVParser::parseMarketData() 
{
    std::vector<MarketData> result;
    std::ifstream file(filename);
    
    if(!file.is_open()) 
    {
        std::cerr << "Error: Cannot open file " << filename << std::endl;
        return result;
    }
    
    std::string line;
    bool firstLine = true;
    double currentSpotPrice = 239.7001; // Из заголовка файла
    
    while(std::getline(file, line)) 
    {
        if(firstLine) 
        {
            firstLine = false;
            continue; // Пропускаем заголовок
        }
        
        if(!line.empty()) 
        {
            MarketData data = parseLine(line);
            data.spotPrice = currentSpotPrice; // Устанавливаем спотовую цену
            if (MarketDataLoader::validateMarketData(data)) 
            {
                result.push_back(data);
            }
        }
    }
    
    file.close();
    return result;
}

MarketData CSVParser::parseLine(const std::string& line) 
{
    MarketData data;
    std::stringstream ss(line);
    std::string token;
    std::vector<std::string> tokens;
    
    // Разбиваем строку на токены
    while(std::getline(ss, token, delimiter)) 
    {
        tokens.push_back(token);
    }
    
    if(tokens.size() >= 17) 
    {
        try 
        {
            // Парсим CALL опционы (колонки 0-9)
            data.ticker = tokens[1]; // Symbol call option
            data.strike = cleanNumber(tokens[11]); // Strike price
            data.marketPrice = cleanNumber(tokens[2]); // Last sale call
            data.timeToExpiration = dateStringToDays(tokens[0]); // Expiration date
            data.riskFreeRate = 0.05; // Предполагаем безрисковую ставку
            data.optionType = "CALL";
            
            // Также можно создать запись для PUT опциона
            MarketData putData;
            putData.ticker = tokens[12]; // Symbol put option
            putData.strike = cleanNumber(tokens[11]); // Тот же страйк
            putData.marketPrice = cleanNumber(tokens[13]); // Last sale put
            putData.timeToExpiration = dateStringToDays(tokens[0]);
            putData.riskFreeRate = 0.05;
            putData.optionType = "PUT";
            
        } 
        catch(const std::exception& e) 
        {
            std::cerr << "Error parsing line: " << e.what() << std::endl;
        }
    }
    
    return data;
}

MarketData CSVParser::findDataByTicker(const std::string& ticker) 
{
    auto allData = parseMarketData();
    
    for(const auto& data : allData) 
    {
        if(data.ticker == ticker) 
        {
            return data;
        }
    }
    
    // Возвращаем пустые данные если не нашли
    return MarketData{};
}
// src/market/csv_parser.cpp
#include <iostream>
#include <algorithm>

#include "../../include/market/csv_parser.hpp"

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
    
    while(std::getline(file, line)) 
    {
        if(firstLine) 
        {
            firstLine = false; // Пропускаем заголовок
            continue;
        }
        
        if(!line.empty())
        {
            MarketData data = parseLine(line);
            if(MarketDataLoader::validateMarketData(data)) 
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
    int column = 0;
    
    while(std::getline(ss, token, delimiter)) 
    {
        switch(column) 
        {
            case 0: data.ticker = token; break;
            case 1: data.spotPrice = std::stod(token); break;
            case 2: data.strike = std::stod(token); break;
            case 3: data.timeToExpiration = std::stod(token); break;
            case 4: data.marketPrice = std::stod(token); break;
            case 5: data.riskFreeRate = std::stod(token); break;
            case 6: data.optionType = token; break;
        }

        column++;
    }
    
    return data;
}

MarketData CSVParser::findDataByTicker(const std::string& ticker) 
{
    auto allData = parseMarketData();
    
    for(const auto& data : allData) 
    {
        if (data.ticker == ticker) 
        {
            return data;
        }
    }
    
    // Возвращаем пустые данные если не нашли
    return MarketData{};
}
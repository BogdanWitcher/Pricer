// include/market/csv_parser.hpp
#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <fstream>
#include <sstream>
#include "market_data.hpp"

class CSVParser
{
private:
    std::string filename;
    char delimiter;

public:
    CSVParser(const std::string& file = "", char delim = ','): 
    filename(file), 
    delimiter(delim) 
    {

    }

    // Парсинг CSV файла в вектор MarketData
    std::vector<MarketData> parseMarketData();
    
    // Парсинг одной строки
    MarketData parseLine(const std::string& line);
    
    // Поиск данных по тикеру
    MarketData findDataByTicker(const std::string& ticker);
    
    void setFilename(const std::string& file) {filename = file;}
    void setDelimiter(char delim) {delimiter = delim;}
};
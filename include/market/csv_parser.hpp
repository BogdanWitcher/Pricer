// include/market/csv_parser.hpp
#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <fstream>
#include <sstream>

#include "market_data.hpp"

/**
 * @brief Класс для парсинга CSV файлов с рыночными данными
 * 
 * Обеспечивает чтение и обработку данных из CSV файлов,
 * преобразование в структуры MarketData и поиск по тикерам.
 */
class CSVParser
{
private:

    std::string filename;   ///< Имя файла для парсинга
    char delimiter; ///< Разделитель полей в CSV

public:
     /**
     * @brief Конструктор парсера CSV
     * @param file Путь к CSV файлу
     * @param delim Разделитель полей (по умолчанию ',')
     */
    CSVParser(const std::string& file = "", char delim = ','): 
    filename(file), 
    delimiter(delim) 
    {

    }

    /**
     * @brief Парсинг всего CSV файла в вектор MarketData
     * @return Вектор структур MarketData со всеми данными из файла
     */
    std::vector<MarketData> parseMarketData();
    
    /**
     * @brief Парсинг одной строки CSV
     * @param line Строка для парсинга
     * @return Структура MarketData с данными из строки
     *
    */
    MarketData parseLine(const std::string& line);
    
    /**
     * @brief Поиск данных по тикеру в CSV файле
     * @param ticker Тикер для поиска
     * @return Структура MarketData с данными по указанному тикеру
     */
    MarketData findDataByTicker(const std::string& ticker);
    
    /// @brief Установка имени файла
    void setFilename(const std::string& file) {filename = file;}

    /// @brief Установка разделителя
    void setDelimiter(char delim) {delimiter = delim;}
};
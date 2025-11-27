// include/math/numerical_methods.hpp
#pragma once

/**
 * \mainpage Option Pricer - Документация
 * 
 * \section intro_sec Введение
 * 
 * Option Pricer - комплексная библиотека для оценки финансовых опционов и анализа рисков.
 * Библиотека предоставляет реализацию классических моделей ценообразования, методов
 * численного анализа и инструментов для работы с рыночными данными.
 * 
 * \section architecture_sec Архитектура библиотеки
 * 
 * Библиотека организована в несколько модулей:
 * 
 * \subsection math_module Модуль математических функций
 * 
 * Содержит функции для:
 * - Численного интегрирования (метод Гауссовой квадратуры)
 * - Статистических распределений (нормальное распределение)
 * - Финансовых вычислений
 * 
 * Основные классы и функции:
 * - math::standart_integral_gaussian_quadrature - интегрирование Гауссовой квадратурой
 * - math::normalPDF - плотность нормального распределения
 * - math::normalCDF - функция нормального распределения
 * 
 * \subsection market_module Модуль рыночных данных
 * 
 * Обеспечивает загрузку и обработку рыночных данных:
 * - CSVParser - парсинг CSV файлов с данными по опционам
 * - MarketData - структура для хранения параметров опционов
 * - MarketDataLoader - абстрактный класс для загрузки данных из различных источников
 * 
 * \subsection models_module Модуль моделей ценообразования
 * 
 * Содержит реализации моделей оценки опционов:
 * - PricingModel - абстрактный базовый класс для всех моделей
 * - BlackScholesModel - классическая модель Блэка-Шоулза
 * - Различные типы опционов (Vanilla, Barrier) с расчетом греков
 * 
 * \section usage_sec Использование
 * 
 * Пример оценки опциона колл с помощью модели Блэка-Шоулза:
 * \code{.cpp}
 * #include "market/csv_parser.hpp"
 * #include "models/black_scholes.hpp"
 * 
 * // Загрузка рыночных данных
 * CSVParser parser("options_data.csv");
 * auto marketData = parser.parseMarketData();
 * 
 * // Создание и калибровка модели
 * auto bsModel = std::make_shared<black_sholes_model::BlackScholesModel>(marketData[0]);
 * bsModel->colibrateModel();
 * 
 * // Расчет цены опциона
 * black_sholes_model::OptionCall callOption(100.0, 105.0, 0.25);
 * double price = callOption.calculatePrice(*bsModel);
 * \endcode
 * 
 * \section features_sec Основные возможности
 * 
 * - Оценка ванильных опционов (колл/пут)
 * - Расчет барьерных опционов
 * - Определение подразумеваемой волатильности
 * - Расчет греков (дельта, гамма, вега)
 * - Загрузка данных из CSV файлов
 * - Модульная архитектура для расширения
 */

namespace math
{
    #ifndef M_PI
        #define M_PI 3.14159265358979323846
    #endif // M_PI

    /**
     * @brief Вычисляет интеграл функции методом Гауссовой квадратуры
     * 
     * @param a Нижний предел интегрирования
     * @param b Верхний предел интегрирования
     * @param n Количество узлов для квадратурной формулы
     * @param func Указатель на функцию, которую нужно проинтегрировать
     * @return double Результат численного интегрирования
     */
    double standart_integral_gaussian_quadrature(double a, double b, int n, double (*func)(double));

    /**
     * @brief Вычисляет интеграл функции с использованием указанного метода интегрирования
     * 
     * @param a Нижний предел интегрирования
     * @param b Верхний предел интегрирования
     * @param N Количество подинтервалов для разбиения
     * @param n Параметр точности для метода интегрирования (зависит от метода)
     * @param func Указатель на функцию, которую нужно проинтегрировать
     * @param integralMethod Указатель на функцию метода интегрирования
     * @return double Результат численного интегрирования
     */
    double integral(double a, double b, int N, int n, double (*func)(double), double (*integralMethod)(double, double, int, double (*)(double)));

    /**
     * @brief Вычисляет значение функции плотности стандартного нормального распределения
     * 
     * @param x Аргумент функции
     * @return double Значение плотности вероятности в точке x
     */
    double normalPDF(double x);

    /**
     * @brief Вычисляет значение функции распределения стандартного нормального закона
     * 
     * @param x Аргумент функции
     * @return double Вероятность того, что случайная величина примет значение ≤ x
     */
    double normalCDF(double x);

} // namespace math
// main.cpp
#include <iostream>
#include <memory>
#include <vector>
#include <iomanip>
#include <cmath>
#include <ctime>

#include "include/models/black_sholes.hpp"
#include "include/market/market_data.hpp"
#include "include/math/numerical_methods.hpp"

int main() 
{
    srand(static_cast<unsigned int>(time(0)));
    
    std::vector<MarketData> syntheticData;
    std::vector<std::string> tickers = {"AAPL", "GOOGL", "MSFT", "AMZN", "TSLA", "NVDA"};
    std::vector<double> spotPrices = {150.0, 2800.0, 330.0, 180.0, 250.0, 900.0};
    
    MarketData md;
    double volatility;
    double d1;
    double d2;

    for(int i = 0; i < 6; ++i) 
    {
        md.ticker = tickers[i];
        md.spotPrice = spotPrices[i] * (0.9 + 0.2 * (rand() % 100) / 100.0);
        md.strike = md.spotPrice * (0.8 + 0.4 * (rand() % 100) / 100.0);
        md.timeToExpiration = 30.0 / 365 + (rand() % 180) / 365.0;
        md.riskFreeRate = 0.03 + (rand() % 10) / 1000.0;
        
        volatility = 0.2 + (rand() % 15) / 100.0;
        d1 = (log(md.spotPrice / md.strike) + (md.riskFreeRate + volatility * volatility / 2) * md.timeToExpiration) 
                   / (volatility * sqrt(md.timeToExpiration));
        d2 = d1 - volatility * sqrt(md.timeToExpiration);
        
        if(i % 2 == 0) 
        {
            md.optionType = "Call";
            md.marketPrice = md.spotPrice * math::normalCDF(d1) - md.strike * exp(-md.riskFreeRate * md.timeToExpiration) * math::normalCDF(d2);
        } 
        else 
        {
            md.optionType = "Put";
            md.marketPrice = md.strike * exp(-md.riskFreeRate * md.timeToExpiration) * math::normalCDF(-d2) - md.spotPrice * math::normalCDF(-d1);
        }
        
        md.marketPrice *= (0.95 + 0.1 * (rand() % 100) / 100.0);
        syntheticData.push_back(md);
    }
    
    // Калибруем модель на первом опционе
    auto calibrationData = std::make_shared<MarketData>(syntheticData[0]);
    black_sholes_model::BlackScholesModel bs(calibrationData);
    bs.colibrateModel();
    
    std::cout << bs.getStatus() << std::endl;
    std::cout << "volatility (Sigma) = " << bs.volatility(1, 1) << std::endl;
    
    std::cout << "\n=== Pricing ===" << std::endl;
    
    // Создаем массив опционов: 2 колла, 2 пута, 3 барьерных
    std::vector<std::shared_ptr<black_sholes_model::Option>> options;
    
    // 2 опциона колл
    options.push_back(std::make_shared<black_sholes_model::OptionCall>(
        syntheticData[0].spotPrice, 
        syntheticData[0].strike, 
        syntheticData[0].timeToExpiration
    ));
    
    options.push_back(std::make_shared<black_sholes_model::OptionCall>(
        syntheticData[2].spotPrice, 
        syntheticData[2].strike, 
        syntheticData[2].timeToExpiration
    ));
    
    // 2 опциона пут
    options.push_back(std::make_shared<black_sholes_model::OptionPut>(
        syntheticData[1].spotPrice, 
        syntheticData[1].strike, 
        syntheticData[1].timeToExpiration
    ));
    
    options.push_back(std::make_shared<black_sholes_model::OptionPut>(
        syntheticData[3].spotPrice, 
        syntheticData[3].strike, 
        syntheticData[3].timeToExpiration
    ));
    
    // 3 барьерных опциона разных типов
    options.push_back(std::make_shared<black_sholes_model::BarrierOption>(
        syntheticData[4].spotPrice, 
        syntheticData[4].strike, 
        syntheticData[4].timeToExpiration,
        syntheticData[4].spotPrice * 1.1, 
        "up-and-out"
    ));
    
    options.push_back(std::make_shared<black_sholes_model::BarrierOption>(
        syntheticData[5].spotPrice, 
        syntheticData[5].strike, 
        syntheticData[5].timeToExpiration,
        syntheticData[5].spotPrice * 0.9, 
        "down-and-in"
    ));
    
    options.push_back(std::make_shared<black_sholes_model::BarrierOption>(
        syntheticData[0].spotPrice, 
        syntheticData[0].strike, 
        syntheticData[0].timeToExpiration * 1.5, 
        syntheticData[0].spotPrice * 1.15, 
        "up-and-in"
    ));
    
    // Расчет цен для всех опционов
    double price;
    double expirationDays;

    for(int i = 0; i < options.size(); ++i) 
    {
        price = options[i]->calculatePrice(bs);
        expirationDays = 0.0;
        
        if(i < 2) 
        {
            expirationDays = syntheticData[i*2].timeToExpiration * 365;
        } 
        else if(i < 4) 
        {
            expirationDays = syntheticData[i*2-3].timeToExpiration * 365;
        } 
        else 
        {
            if (i == 4) expirationDays = syntheticData[4].timeToExpiration * 365;
            else if (i == 5) expirationDays = syntheticData[5].timeToExpiration * 365;
            else expirationDays = syntheticData[0].timeToExpiration * 1.5 * 365;
        }
        
        std::cout << "Option " << (i+1) << ": ";
        std::cout << "Type: " << options[i]->getType() 
                  << ", Expiration: " << std::fixed << std::setprecision(1) << expirationDays << " days"
                  << ", Price: " << std::setprecision(2) << price << std::endl;
    }
    
    std::cout << "\n=== Detail Analyse Option Call ===" << std::endl;
    
    auto callOption = std::make_shared<black_sholes_model::OptionCall>(
        syntheticData[0].spotPrice, 
        syntheticData[0].strike, 
        syntheticData[0].timeToExpiration
    );
    
    double callPrice = callOption->calculatePrice(bs);
    double delta = callOption->calculateDelta(bs);
    double gamma = callOption->calculateGamma(bs);
    double vega = callOption->calculateVega(bs);
    
    std::cout << "Option CALL:" << std::endl;
    std::cout << "Type: " << callOption->getType() 
              << ", Expiration: " << std::fixed << std::setprecision(1) << (syntheticData[0].timeToExpiration * 365) << " days"
              << ", Price: " << std::setprecision(2) << callPrice << std::endl;
    std::cout << "Greeks: Delta=" << std::fixed << std::setprecision(4) << delta
              << ", Gamma=" << gamma
              << ", Vega=" << vega << std::endl;
              
    return 0;
}
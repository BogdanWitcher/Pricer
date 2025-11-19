// include/market/market_data.hpp
#pragma once

#include <string>

struct MarketData 
{
    std::string ticker;
    double spotPrice;
    double strike;
    double timeToExpiration;
    double marketPrice;
    double riskFreeRate;
    std::string optionType; 
};

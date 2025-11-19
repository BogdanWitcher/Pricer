#include <iostream>
#include <memory>

#include "include/models/black_sholes.hpp"
#include "include/market/market_data.hpp"

int main()
{
    auto data = std::make_shared<MarketData>();

    data->ticker = "AAPL";
    data->spotPrice = 270.21;
    data->strike = 232.5;
    data->timeToExpiration = 76.0 / 252;
    data->riskFreeRate = 0.04;
    data->marketPrice = (8.75 + 9.1) / 2;
    data->optionType = "Call";

    black_sholes_model::BlackScholesModel bs(data);
    
    bs.colibrateModel();

    std::cout << bs.getStatus() << std::endl;
    std::cout << "Sigma = " << bs.volatility(1, 1) << std::endl;
    std::cout << "Risk Rate = " << bs.riskRate(1) << std::endl;


    
    return 0;
}
// include/options/VanillaOption.hpp
#pragma once

#include <string>
#include <memory>

#include "option.hpp"
#include "option_type.hpp"

class VanilliaOption : public Option
{
private:

    OptionType __type;

public:
    
    VanilliaOption(double strike, double timeTpExpiration, std::shared_ptr<PricingModel> model, OptionType type): 
    Option(strike, timeTpExpiration, model),
    __type(type)
    {
        
    }

    std::string getType() const override;

};
#include "Settlement.h"

Settlement::Settlement(const std::string &name, SettlementType type)
: name(name), type(type),constructionLimit(
          (type == SettlementType::VILLAGE) ? 1 : 
          (type == SettlementType::CITY) ? 2 : 3)
{
}

const string &Settlement::getName() const
{
    return name;
}

SettlementType Settlement::getType() const
{
    return SettlementType();
}

const string Settlement::toString() const
{
    return "Settlement Name: " + name + 
           ", Type: " + (type == SettlementType::VILLAGE ? "Village" : 
                         type == SettlementType::CITY ? "City" : "Metropolis") +
           ", Construction Limit: " + std::to_string(constructionLimit);
}
int Settlement::getConstructionLimit() const {
    return constructionLimit;
}
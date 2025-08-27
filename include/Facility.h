#pragma once
#include <string>
#include <vector>
using std::string;
using std::vector;

enum class FacilityStatus {
    UNDER_CONSTRUCTIONS,
    OPERATIONAL,
};

enum class FacilityCategory {
    LIFE_QUALITY,
    ECONOMY,
    ENVIRONMENT,
};


class FacilityType {
    public:
        FacilityType(const string &name, const FacilityCategory category, const int price, const int lifeQuality_score, const int economy_score, const int environment_score);
        const string &getName() const;
        int getCost() const;
        int getLifeQualityScore() const;
        int getEnvironmentScore() const;
        int getEconomyScore() const;
        FacilityCategory getCategory() const;

    protected:
        const string name;
        const FacilityCategory category;
        const int price;
        const int lifeQuality_score;
        const int economy_score;
        const int environment_score;
};



class Facility: public FacilityType {

    public:
        Facility(const string &name, const string &settlementName, const FacilityCategory category, const int price, const int lifeQuality_score, const int economy_score, const int environment_score);
        Facility(const FacilityType &type, const string &settlementName);
        const string &getSettlementName() const;
        int getTimeLeft();
        FacilityStatus step();
        void setStatus(FacilityStatus status);
        const FacilityStatus& getStatus() const;
        const string toString() const;
        

    private:
        const string settlementName;
        FacilityStatus status;
        int timeLeft;
};
#include "SelectionPolicy.h"
#include <algorithm>
using namespace std;

    



NaiveSelection::NaiveSelection():lastSelectedIndex(-1)
{
}

const FacilityType &NaiveSelection::selectFacility(const vector<FacilityType> &facilitiesOptions)
{
    lastSelectedIndex = (lastSelectedIndex + 1) % facilitiesOptions.size();
    return facilitiesOptions[lastSelectedIndex];
}

const string NaiveSelection::toString() const
{
    return "nve";
}

NaiveSelection *NaiveSelection::clone() const
{
    return new NaiveSelection(*this);
}

EconomySelection::EconomySelection():lastSelectedIndex(-1)
{
}

const FacilityType &EconomySelection::selectFacility(const vector<FacilityType> &facilitiesOptions)
{
    int i = lastSelectedIndex +1;
    while(facilitiesOptions[i].getCategory() != FacilityCategory::ECONOMY){
       i = (i+1) % facilitiesOptions.size();
    }
    lastSelectedIndex =  i;
    return facilitiesOptions[lastSelectedIndex];
}

const string EconomySelection::toString() const
{
    return "eco";
}

EconomySelection *EconomySelection::clone() const
{
    return new EconomySelection(*this);
}

SustainabilitySelection::SustainabilitySelection():lastSelectedIndex(-1)
{
}

const FacilityType &SustainabilitySelection::selectFacility(const vector<FacilityType> &facilitiesOptions)
{
    int i = lastSelectedIndex + 1;
    while(facilitiesOptions[i].getCategory() != FacilityCategory::ENVIRONMENT){
        i = (i+1) % facilitiesOptions.size();
    }
    lastSelectedIndex =  i;
    return facilitiesOptions[lastSelectedIndex];
}

const string SustainabilitySelection::toString() const
{
    return "env";
}

SustainabilitySelection *SustainabilitySelection::clone() const
{
    return new SustainabilitySelection(*this);
}

const int BalancedSelection::Distance(int lq_s, int ec_s, int en_s)
{
    int arr[3] = {lq_s, ec_s, en_s};
    sort(arr, arr + 3);
    return arr[2] - arr[0];

}

BalancedSelection::BalancedSelection(int LifeQualityScore, int EconomyScore, int EnvironmentScore):
LifeQualityScore(LifeQualityScore), EconomyScore(EconomyScore), EnvironmentScore(EnvironmentScore)
{}

const FacilityType &BalancedSelection::selectFacility(const vector<FacilityType> &facilitiesOptions)
{
    int index = 0 ,minBalance = INT32_MAX;
    int lqs,ecs,ens, distance;
    for (std::vector<FacilityType>::size_type i = 0; i < facilitiesOptions.size(); i++)
    {
        lqs = facilitiesOptions[i].getLifeQualityScore() + LifeQualityScore;
        ecs = facilitiesOptions[i].getEconomyScore() + EconomyScore;
        ens = facilitiesOptions[i].getEnvironmentScore() + EnvironmentScore;
        distance = Distance(lqs,ecs,ens);
        if (distance < minBalance){
            minBalance = distance;
            index = i;
        }
    }
    LifeQualityScore += facilitiesOptions[index].getLifeQualityScore();
    EconomyScore += facilitiesOptions[index].getEconomyScore();
    EnvironmentScore += facilitiesOptions[index].getEnvironmentScore();
    return facilitiesOptions[index];
}

const string BalancedSelection::toString() const
{
    return "bal";
}

BalancedSelection *BalancedSelection::clone() const
{
    return new BalancedSelection(*this);
}

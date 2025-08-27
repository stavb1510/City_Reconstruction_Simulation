#include "Plan.h"
#include <iostream>

Plan::Plan(const int planId, const Settlement &settlement, SelectionPolicy *selectionPolicy, const vector<FacilityType> &facilityOptions)
    : plan_id(planId),
      settlement(settlement),
      selectionPolicy(selectionPolicy),
      status(PlanStatus::AVALIABLE),
      facilities(),
      underConstruction(),
      facilityOptions(facilityOptions),
      life_quality_score(0),
      economy_score(0),
      environment_score(0) {}

Plan::Plan(const int planId, const Settlement &settlement, SelectionPolicy *selectionPolicy, PlanStatus status, vector<Facility *> newFacilities, 
           vector<Facility *> newUnderConstruction, const vector<FacilityType> &facilityOptions, int life_quality_score, int economy_score, int environment_score)
    : plan_id(planId),
      settlement(settlement),
      selectionPolicy(selectionPolicy),
      status(status),
      facilities(),
      underConstruction(),
      facilityOptions(facilityOptions),
      life_quality_score(life_quality_score),
      economy_score(economy_score),
      environment_score(environment_score) 
{
    for (Facility *facility : newFacilities) {
        facilities.push_back(new Facility(*facility));
    }

    for (Facility *facility : newUnderConstruction) {
        underConstruction.push_back(new Facility(*facility));
    }
}
// copy constructor
Plan::Plan(const Plan &other)
    : plan_id(other.plan_id),
      settlement(other.settlement),
      selectionPolicy(other.selectionPolicy->clone()),
      status(other.status),
      facilities(),
      underConstruction(),
      facilityOptions(other.facilityOptions),
      life_quality_score(other.life_quality_score),
      economy_score(other.economy_score),
      environment_score(other.environment_score)
{
    for (const Facility *fac : other.underConstruction) {
        underConstruction.push_back(new Facility(*fac));
    }

    for (const Facility *fac : other.facilities) {
        facilities.push_back(new Facility(*fac));
    }
}

// destructor
Plan::~Plan()
{
    for (Facility* fac : underConstruction) {
        delete fac;
    }

    for (Facility* fac : facilities) {
        delete fac;
    }

    delete selectionPolicy;
}
Plan::Plan(Plan &&other) : 
        plan_id(other.plan_id),
        settlement(other.settlement),
        selectionPolicy(other.selectionPolicy),
        status(other.status),
        facilities(std::move(other.facilities)),
        underConstruction(std::move(other.underConstruction)),
        facilityOptions(other.facilityOptions),
        life_quality_score(other.life_quality_score),
        economy_score(other.economy_score),
        environment_score(other.environment_score)
{
    other.selectionPolicy = nullptr;
}
    

const int Plan::getlifeQualityScore() const
{
    return life_quality_score;
}

const int Plan::getEconomyScore() const
{
    return economy_score;
}

const int Plan::getEnvironmentScore() const
{
    return environment_score;
}

const Settlement &Plan::getSettlement() const
{
    return settlement;
}

void Plan::setSelectionPolicy(SelectionPolicy *selectionPolicy)
{
    if(this->selectionPolicy){
        delete this->selectionPolicy;
    }
    this->selectionPolicy = selectionPolicy;
}

void Plan::step()
{
    if(status == PlanStatus:: AVALIABLE) {
        int C_limit = settlement.getConstructionLimit() - underConstruction.size();
        while (C_limit != 0)
        {
            Facility *fac = new Facility(this ->selectionPolicy->selectFacility(facilityOptions),
            settlement.getName());
            addFacility(fac);
            C_limit--;
        }
        status = PlanStatus::BUSY;
    }
    FacilityStatus facStatus = FacilityStatus::UNDER_CONSTRUCTIONS;
    for(std::vector<Facility*>::size_type i = 0; i < underConstruction.size(); i++){
        facStatus = underConstruction[i]->step();
        if(facStatus == FacilityStatus::OPERATIONAL){
            underConstruction[i]->setStatus(FacilityStatus::OPERATIONAL);
            life_quality_score += underConstruction[i]->getLifeQualityScore();
            economy_score += underConstruction[i]->getEconomyScore();
            environment_score += underConstruction[i]->getEnvironmentScore();
            facilities.push_back(underConstruction[i]);
            underConstruction.erase(underConstruction.begin() + i);
            i--;
        }
    }
    if (underConstruction.size() < static_cast<std::vector<Facility*>::size_type>(settlement.getConstructionLimit())) {
        status = PlanStatus::AVALIABLE;
    }
    
}

void Plan::printStatus()
{
    if(status == PlanStatus::AVALIABLE){
        std::cout << "AVALIABLE"  << std::endl;
    }
    else{
        std::cout << "BUSY" << std::endl;
    }
}

const vector<Facility *> &Plan::getFacilities() const
{
    return facilities;
}

const vector<Facility *> &Plan::getUnderConstruction() const
{
    return underConstruction;
}

void Plan::addFacility(Facility *facility)
{
    underConstruction.push_back(facility);
}

const string Plan::toString() const
{
    return "PlanID: " +  std::to_string(getPlanId()) + "\n" +
    "SettlementName: "  + getSettlement().getName() + "\n" +
    "LifeQuality_Score: "  + std::to_string(getlifeQualityScore()) + "\n" +
    "Economy_Score: " + std::to_string(getEconomyScore()) + "\n" +
    "Environment_Score: " + std::to_string(getEnvironmentScore()) + "\n";
}

const int Plan::getPlanId() const
{
    return plan_id;
}

SelectionPolicy &Plan::getSelectionPolicy()
{
    return *selectionPolicy;
}

SelectionPolicy *Plan::getPtrSelectionPolicy() const
{
    return selectionPolicy;
}

PlanStatus Plan::getPlanStatus() const
{
    return status;
}

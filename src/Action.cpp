#include "Action.h"
#include <iostream>
#include <fstream>

BaseAction::BaseAction(): errorMsg(""), status(ActionStatus::COMPLETED)
{
}
Simulation* backup = nullptr;

ActionStatus BaseAction::getStatus() const
{
    return status;
}

const string BaseAction::StatusToString(ActionStatus status) const 
{
    string s;
    if (status == ActionStatus::COMPLETED){
        s = "COMPLETED";
    }
    else {
        s = "ERROR";
    }
    return s;
}

void BaseAction::complete()
{
    status = ActionStatus::COMPLETED;
}

void BaseAction::error(string errorMsg)
{
    this->errorMsg = errorMsg;
    status = ActionStatus::ERROR;
    std::cout << getErrorMsg() << std::endl;
}

const string &BaseAction::getErrorMsg() const
{
    return errorMsg;
}

SimulateStep::SimulateStep(const int numOfSteps):
BaseAction(),
numOfSteps(numOfSteps)
{}

void SimulateStep::act(Simulation &simulation)
{
    for (int i = 0; i < numOfSteps; i++) {
        simulation.step();
    }
}
const string SimulateStep::toString() const
{
    return "step " + std::to_string(numOfSteps)+ " " + StatusToString(getStatus());
}

SimulateStep *SimulateStep::clone() const
{
    return new SimulateStep(*this);
}

AddPlan::AddPlan(const string &settlementName, const string &selectionPolicy):
BaseAction(),
settlementName(settlementName),
selectionPolicy(selectionPolicy)
{
}

void AddPlan::act(Simulation &simulation)
{
    SelectionPolicy *policy = nullptr;
    if(selectionPolicy == "bal") {
        policy = new BalancedSelection(0,0,0);
    } 
    else if(selectionPolicy == "eco") {
        policy = new EconomySelection();
    }
    else if (selectionPolicy == "env") {
        policy = new SustainabilitySelection();
    } else if(selectionPolicy == "nve") {
        policy = new NaiveSelection();
    }
    if (simulation.isSettlementExists(settlementName) && policy != nullptr){
        simulation.addPlan(simulation.getSettlement(settlementName),policy);
        complete();
    }
    else {
        error("Cannot create this plan");
    }
}

const string AddPlan::toString() const
{
    string s = "plan " + settlementName + " " + selectionPolicy + " " + StatusToString(getStatus());
    return s;
}

AddPlan *AddPlan::clone() const
{
    return new AddPlan(*this);
}

AddSettlement::AddSettlement(const string &settlementName, SettlementType settlementType):
BaseAction(),
settlementName(settlementName),
settlementType(settlementType)
{
}

void AddSettlement::act(Simulation &simulation)
{
    Settlement *s = new Settlement(settlementName,settlementType);
    if(simulation.addSettlement(s)) {
        complete();
    }
    else{
        error("Settlement already exists");
        delete s;
    }
}

AddSettlement *AddSettlement::clone() const
{
return new AddSettlement(*this);
}

const string AddSettlement::toString() const
{
    string type;
    if(settlementType == SettlementType:: VILLAGE) {
        type = "0";
    } else if(settlementType == SettlementType:: CITY) {
        type = "1";
    } else {
        type = "2";
    }
    return "settlement " + settlementName + " " + type + " " + StatusToString(getStatus());
}

AddFacility::AddFacility(const string &facilityName, const FacilityCategory facilityCategory, const int price, const int lifeQualityScore, const int economyScore, const int environmentScore):
BaseAction(),
facilityName(facilityName),
facilityCategory(facilityCategory),
price(price),
lifeQualityScore(lifeQualityScore),
economyScore(economyScore),
environmentScore(environmentScore)
{
}

void AddFacility::act(Simulation &simulation)
{
    if(simulation.addFacility(FacilityType(facilityName,facilityCategory,price,lifeQualityScore,economyScore,environmentScore))){
        complete();
    }else {
        error("Facility already exists");
    }
}

AddFacility *AddFacility::clone() const
{
    return new AddFacility(*this);
}

const string AddFacility::toString() const
{
    string category;
    if(facilityCategory == FacilityCategory:: LIFE_QUALITY) {
        category = "0";
    } else if (facilityCategory == FacilityCategory:: ECONOMY) {
        category = "1";
    }else {
        category = "2";
    }
    return "facility" + facilityName + " " + category + " " + std::to_string(price) + " " + std::to_string(lifeQualityScore) + " " +
    std::to_string(economyScore) + " " + std::to_string(environmentScore);
}

PrintPlanStatus::PrintPlanStatus(int planId):
BaseAction(),
planId(planId)
{
}

void PrintPlanStatus::act(Simulation &simulation)
{
    if (planId >= simulation.getPlanID() || planId < 0){
        error("Error: Plan doesn't exist");
    }
    else{
        Plan &p = simulation.getPlan(planId);
        std:: cout << "PlanID: " + std::to_string(p.getPlanId()) << std::endl;
        std:: cout << "SettlementName: " + p.getSettlement().getName() << std::endl;
        std:: cout << "PlanStatus: ";
        p.printStatus();
        std:: cout << "SelectionPolicy: " + p.getSelectionPolicy().toString()<< std::endl;
        std:: cout << "LifeQualityScore: " + std::to_string(p.getlifeQualityScore()) << std::endl;
        std:: cout << "EconomyScore: " + std::to_string(p.getEconomyScore()) << std::endl;
        std:: cout << "EnvrionmentScore: " + std::to_string(p.getEnvironmentScore()) << std::endl;
        for(Facility *fac : p.getUnderConstruction()){
            std:: cout << fac->toString() << std::endl;
        }
        for (Facility *fac : p.getFacilities()){
            std:: cout <<fac->toString() << std::endl;
        }
        
    }
}

PrintPlanStatus *PrintPlanStatus::clone() const
{
    return new PrintPlanStatus(*this);
}
const string PrintPlanStatus::toString() const
{
    return "planStatus " + std::to_string(planId) + " " + StatusToString(getStatus());
}

ChangePlanPolicy::ChangePlanPolicy(const int planId, const string &newPolicy):
BaseAction(),
planId(planId),
newPolicy(newPolicy)
{}

void ChangePlanPolicy::act(Simulation &simulation)
{
    Plan &p = simulation.getPlan(planId);
    SelectionPolicy &previousPolicy = p.getSelectionPolicy();
    if (planId > simulation.getPlanID() || planId < 0 || previousPolicy.toString() == newPolicy){
        error("Cannot change selection policy");
    }
    else{ 
        SelectionPolicy *policy = nullptr;
        if(newPolicy == "bal") {
            int life = p.getlifeQualityScore();
            int eco = p.getEconomyScore();
            int env = p.getEnvironmentScore();
            for(const Facility *fac : p.getUnderConstruction()){
                life += fac->getLifeQualityScore();
                eco += fac->getEconomyScore();
                env += fac->getEnvironmentScore();
            }
            policy = new BalancedSelection(life, eco, env);
        }
        else if(newPolicy == "eco") {
            policy = new EconomySelection();
        }
        else if (newPolicy == "env") {
            policy = new SustainabilitySelection();
        } 
        else if(newPolicy == "nve") {
            policy = new NaiveSelection();
        }
        std:: cout << "PlanID: " + std::to_string(planId) << std::endl;
        std:: cout << "previousPolicy: "+ previousPolicy.toString() << std::endl;
        std:: cout << "newPolicy: " + policy->toString() << std::endl;
        simulation.getPlan(planId).setSelectionPolicy(policy);
    }
}

ChangePlanPolicy *ChangePlanPolicy::clone() const
{
    return new ChangePlanPolicy(*this);
}

const string ChangePlanPolicy::toString() const
{
    return "changePolicy " + std::to_string(planId) + " " + newPolicy + " " + StatusToString(getStatus());
}

PrintActionsLog::PrintActionsLog()
{
}

void PrintActionsLog::act(Simulation &simulation)
{
    for(BaseAction *action : simulation.getActionLog()){
        std:: cout << action->toString() << std::endl;
    }
}

PrintActionsLog *PrintActionsLog::clone() const
{
    return new PrintActionsLog();
}

const string PrintActionsLog::toString() const
{
    return  "PrintActionsLog COMPLETED";
}

Close::Close()
{
}

void Close::act(Simulation &simulation)
{
    complete();
    simulation.close();
    if(backup != nullptr){
        delete backup;
        backup = nullptr;
    }
}

Close *Close::clone() const
{
    return new Close();
}

const string Close::toString() const
{
    return "close " + StatusToString(getStatus());
}

BackupSimulation::BackupSimulation()
{
}

void BackupSimulation::act(Simulation &simulation)
{
    if(backup != nullptr){
        delete backup;
    }
    backup = new Simulation(simulation);
    complete();
}

BackupSimulation *BackupSimulation::clone() const
{
    return new BackupSimulation();
}

const string BackupSimulation::toString() const
{
    return "backup COMPLETED";
}

RestoreSimulation::RestoreSimulation()
{
}

void RestoreSimulation::act(Simulation &simulation)
{
    if(backup == nullptr){
        error("No backup available");
    }
    else {
         simulation = *backup; //using copy assignment
         complete();
    }
}

RestoreSimulation *RestoreSimulation::clone() const
{
    return new RestoreSimulation();
}

const string RestoreSimulation::toString() const
{
    if(getStatus() == ActionStatus:: ERROR) {
        return "restore ERROR";
    }
    else {
        return "restore COMPLETED";
    }
}

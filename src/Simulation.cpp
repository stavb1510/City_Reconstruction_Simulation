#include "Simulation.h"
#include <iostream>
#include <Auxiliary.h>
#include <fstream>
#include <Action.h>

//constructor
Simulation::Simulation(const string &configFilePath): isRunning(false),
      planCounter(0),
      actionsLog(), 
      plans(),
      settlements(),
      facilitiesOptions()
{
    std::ifstream configFile(configFilePath);
    string line;
    while (std::getline(configFile, line)) {
        vector<string> arguments = Auxiliary::parseArguments(line);

        if (arguments[0][0] == '#') {
            continue;
        }
        string command = arguments[0];
        if (command == "settlement") {
            string name = arguments[1];
            int type = std::stoi(arguments[2]);
            settlements.push_back(new Settlement(name, static_cast<SettlementType>(type)));
        } 
        else if (command == "facility") {
            string name = arguments[1];
            int category = std::stoi(arguments[2]);
            int price = std::stoi(arguments[3]);
            int lifeQualityImpact = std::stoi(arguments[4]);
            int economyImpact = std::stoi(arguments[5]);
            int environmentImpact = std::stoi(arguments[6]);
            facilitiesOptions.push_back(FacilityType(name, static_cast<FacilityCategory>(category), price, lifeQualityImpact, economyImpact, environmentImpact));
        } 
        else if (command == "plan") {
            string settlementName = arguments[1];
            string selectionPolicy = arguments[2];
            SelectionPolicy* policy = nullptr;
            if (selectionPolicy == "eco") {
                policy = new EconomySelection(); 
            } else if (selectionPolicy == "bal") {
                policy = new BalancedSelection(0,0,0);
            } else if (selectionPolicy == "nve") {
                policy = new NaiveSelection();
            } else if (selectionPolicy == "env") {
                policy = new SustainabilitySelection();
            }

            if (isSettlementExists(settlementName)) {
                addPlan(getSettlement(settlementName), policy);
            } else {
                delete policy;
            }
        } 
    }
}

void Simulation::start()
{
    open();
    std::cout << "The simulation has started" << std::endl;
    while (isRunning) {
        BaseAction *action;
        string command;
        std::getline(std::cin, command);
        vector<string> arguments = Auxiliary:: parseArguments(command);
        const string &requestedAction = arguments[0];
        if(requestedAction == "plan"){                    
            const string& settlementName = arguments[1];
            const string& selectionPolicy = arguments[2];
            action = new AddPlan(settlementName, selectionPolicy);                    
        }
        else if(requestedAction == "step"){
            action = new SimulateStep(std::stoi(arguments[1]));
        }
        else if(requestedAction == "settlement"){
            const string& settlementName = arguments[1];
            int type = std:: stoi(arguments[2]);
            if(type == 0) {
                action = new AddSettlement(settlementName, SettlementType::VILLAGE );
            } else if(type == 1) {
                action = new AddSettlement(settlementName, SettlementType::CITY );
            } else if (type == 2){
                action = new AddSettlement(settlementName, SettlementType::METROPOLIS );
            }               
        }
        else if(requestedAction == "facility")
        {
            string facilityName = arguments[1];
            int categoryInt = std::stoi(arguments[2]);
            FacilityCategory category = static_cast<FacilityCategory>(categoryInt);
            int price = std::stoi(arguments[3]);
            int lifeQualityScore = std::stoi(arguments[4]);
            int economyScore = std::stoi(arguments[5]);
            int environmentScore = std::stoi(arguments[6]);
            action = new AddFacility(facilityName, category, price, lifeQualityScore, economyScore,environmentScore);
        }
        else if(requestedAction == "planStatus")
        {
            action = new PrintPlanStatus(std::stoi(arguments[1]));
        }
        else if(requestedAction == "changeplanpolicy")
        {
            ChangePlanPolicy* change = new ChangePlanPolicy(std::stoi(arguments[1]),arguments[2]);
            action = change;
        }
        else if(requestedAction == "log")
        {
            action = new PrintActionsLog();
        }
        else if(requestedAction == "close")
        {
            action = new Close();
        }
        else if(requestedAction == "backup")
        {
            action = new BackupSimulation();
        }
        else if(requestedAction == "restore")
        {
            action = new RestoreSimulation();
        }
        else {
            std::cout << "Command not found" <<std::endl;
            continue;
        }    
        action->act(*this);
        actionsLog.push_back(action);  
        }
}

void Simulation::addPlan(const Settlement &settlement, SelectionPolicy *selectionPolicy)
{
    plans.push_back(Plan(planCounter, settlement, selectionPolicy,facilitiesOptions));
    planCounter++;
}

void Simulation::addAction(BaseAction *action)
{
    actionsLog.push_back(action);
}

bool Simulation::addSettlement(Settlement *settlement)
{
    if(!isSettlementExists(settlement->getName())){
        settlements.push_back(settlement);
        return true;
    }
    return false;
}

bool Simulation::addFacility(FacilityType facility)
{
    for(FacilityType fac : facilitiesOptions){
        if(fac.getName() == facility.getName()){
            return false;
        }
    }
    facilitiesOptions.push_back(facility);
    return true;
}

bool Simulation::isSettlementExists(const string &settlementName)
{
    for(Settlement *s : settlements) {
        if(s->getName() == settlementName) {
            return true;
        } 
    }
    return false;
}

Settlement & Simulation::getSettlement(const string & settlementName)
{
    for(std::vector<Settlement*>::size_type i = 0; i < settlements.size(); i++){
        if (settlements[i]->getName() == settlementName){
            return *settlements[i];
        }
    }
    return *settlements[0]; //impossible case
}

Plan &Simulation::getPlan(const int planID)
{
    for(Plan &p : plans) {
        if(p.getPlanId() == planID) {
            return p;
        }
    }
    return plans[0]; //impossible case
}

const int Simulation::getPlanID() const
{
    return planCounter;
}

vector<BaseAction*> Simulation::getActionLog()
{
    return actionsLog;
}

void Simulation::step()
{
    for(Plan &p : plans){    
        p.step();
    }
}
void Simulation::close()
{
     for (const Plan& plan : plans) {
        std:: cout << plan.toString() << std::endl;
    }
    isRunning = false;
}
void Simulation::open()
{
    isRunning = true;
}
// destructor
Simulation::~Simulation()
{
    for (BaseAction* action : actionsLog) {
        delete action;
    }
    actionsLog.clear();

    for (Settlement* settlement : settlements) {
        delete settlement;
    }
    settlements.clear();
}
//copy constructor
Simulation::Simulation(const Simulation &other)
    : isRunning(other.isRunning),
      planCounter(other.planCounter),
      actionsLog(),
      plans(),
      settlements(),
      facilitiesOptions(other.facilitiesOptions)
      {
        
        for (BaseAction* action : other.actionsLog) {
            actionsLog.push_back(action->clone()); 
        }

        for (Settlement* settlement : other.settlements) {
            settlements.push_back(new Settlement(*settlement));
        }
        for (const Plan& p : other.plans) {
            const Settlement& settlement = this->getSettlement(p.getSettlement().getName());
            Plan newPlan(
            p.getPlanId(),
            settlement,                                 
            p.getPtrSelectionPolicy()->clone(),    
            p.getPlanStatus(),
            p.getFacilities(), 
            p.getUnderConstruction(),
            facilitiesOptions, 
            p.getlifeQualityScore(),
            p.getEconomyScore(),
            p.getEnvironmentScore());
         plans.push_back(newPlan);
      }
}
// copy assignment operator
Simulation &Simulation::operator=(const Simulation &other)
{
    if (this == &other) {
        return *this;
    }
    isRunning = other.isRunning;
    planCounter = other.planCounter;

    for (Settlement* settlement : settlements) {
        delete settlement;
    }
    settlements.clear();

    for (Settlement* settlement : other.settlements) {
        settlements.push_back(new Settlement(*settlement)); 
    }
    plans.clear();
    for (const Plan& p : other.plans) {
        Plan newPlan(p.getPlanId(),this->getSettlement(p.getSettlement().getName()),p.getPtrSelectionPolicy()->clone(),
        p.getPlanStatus(),p.getFacilities(),p.getUnderConstruction(),facilitiesOptions,
        p.getlifeQualityScore(),p.getEconomyScore(),p.getEnvironmentScore());
        plans.push_back(newPlan);
    }

    for (BaseAction* action : actionsLog) {
        delete action;
    }
    actionsLog.clear();
    for (BaseAction* action : other.actionsLog) {
        actionsLog.push_back(action->clone()); 
    }

    facilitiesOptions.clear();
    for (const FacilityType& f : other.facilitiesOptions) {
        this->facilitiesOptions.push_back(FacilityType(f)); 
    }

    return *this;
}

// move constructor
Simulation::Simulation(Simulation &&other):
    isRunning(other.isRunning),
    planCounter(other.planCounter),
    actionsLog(std::move(other.actionsLog)),
    plans(std::move(other.plans)),
    settlements(std::move(other.settlements)),
    facilitiesOptions(std::move(other.facilitiesOptions)){
    other.isRunning = false;
    other.planCounter = 0;
}     

//move assignment operator
Simulation &Simulation::operator=(Simulation &&other)
{
    if (this == &other) {
        return *this;
    }

    for (BaseAction* action : actionsLog) {
        delete action;
    }
    actionsLog.clear();

    for (Settlement* settlement : settlements) {
        delete settlement;
    }
    settlements.clear();

    isRunning = other.isRunning;
    planCounter = other.planCounter;
    plans = std::move(other.plans);
    facilitiesOptions = std::move(other.facilitiesOptions);
    actionsLog = std::move(other.actionsLog);
    settlements = std::move(other.settlements);

    other.isRunning = false;
    other.planCounter = 0;

    return *this;
}

#pragma once
#include <string>
#include <vector>
#include "Facility.h"
#include "Plan.h"
#include "Settlement.h"
using std::string;
using std::vector;

class BaseAction;
class SelectionPolicy;

class Simulation {
    public:
        Simulation(const string &configFilePath);
        void start();
        void addPlan(const Settlement &settlement, SelectionPolicy *selectionPolicy);
        void addAction(BaseAction *action);
        bool addSettlement(Settlement *settlement);
        bool addFacility(FacilityType facility);
        bool isSettlementExists(const string &settlementName);
        Settlement &getSettlement(const string &settlementName);
        Plan &getPlan(const int planID);
        const int getPlanID() const;
        vector<BaseAction*> getActionLog();
        void step();
        void close();
        void open();
        //destructor
        ~Simulation();
        //copy constructor
        Simulation(const Simulation& other);
        //copy assignment operator
        Simulation& operator=(const Simulation& other);
        //move constructor
        Simulation(Simulation&& other);
        // move aassignment operator
        Simulation& operator=(Simulation&& other);

    private:
        bool isRunning;
        int planCounter; //For assigning unique plan IDs
        vector<BaseAction*> actionsLog;
        vector<Plan> plans;
        vector<Settlement*> settlements;
        vector<FacilityType> facilitiesOptions;
};
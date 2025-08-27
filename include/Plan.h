#pragma once
#include <vector>
#include "Facility.h"
#include "Settlement.h"
#include "SelectionPolicy.h"
using std::vector;

enum class PlanStatus {
    AVALIABLE,
    BUSY,
};

class Plan {
    public:
        Plan(const int planId, const Settlement &settlement, SelectionPolicy *selectionPolicy, const vector<FacilityType> &facilityOptions);
        Plan(const int planId, const Settlement &settlement, SelectionPolicy *selectionPolicy, PlanStatus status, vector<Facility *> newFacilities, vector<Facility *> newUnderConstruction, const vector<FacilityType> &facilityOptions, int life_quality_score, int economy_score, int environment_score);
        // copy constructor
        Plan(const Plan &other);
        //copy assignment operator
        Plan& operator=(const Plan& other) =delete;
        //destractor
        ~Plan();
        //move constructor
        Plan(Plan &&other);
        //move assignment operator
        Plan& operator=(Plan&& other) =delete;

        const int getlifeQualityScore() const;
        const int getEconomyScore() const;
        const int getEnvironmentScore() const;
        const Settlement &getSettlement() const;
        void setSelectionPolicy(SelectionPolicy *selectionPolicy);
        void step();
        void printStatus();
        const vector<Facility*> &getFacilities() const;
        const vector<Facility*> &getUnderConstruction() const;
        void addFacility(Facility* facility);
        const string toString() const;
        const int getPlanId() const;
        SelectionPolicy &getSelectionPolicy();
        SelectionPolicy *getPtrSelectionPolicy() const;
        PlanStatus getPlanStatus() const;

    private:
        int plan_id;
        const Settlement &settlement;
        SelectionPolicy *selectionPolicy; //What happens if we change this to a reference?
        PlanStatus status;
        vector<Facility*> facilities;
        vector<Facility*> underConstruction;
        const vector<FacilityType> &facilityOptions;
        int life_quality_score, economy_score, environment_score;
};
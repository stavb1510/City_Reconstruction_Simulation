#include "Simulation.h"
#include "Settlement.h"
#include <iostream>
#include "Facility.h"
#include <algorithm>
#include "Plan.h"
#include "SelectionPolicy.h"




using namespace std;

extern Simulation* backup;

int main(int argc, char** argv){
    if(argc!=2){
        cout << "usage: simulation <config_path>" << endl;
        return 0;
    }
    string configurationFile = argv[1];
    Simulation simulation(configurationFile);
    simulation.start();
    if(backup!=nullptr){
    	delete backup;
    	backup = nullptr;
    }
    
    return 0;
}
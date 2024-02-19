/********************************************************************************/
/* PERMUTATION FLOW-SHOP SCHEDULING PROBLEM                                                                              
/* headerfile for solver.cpp
/********************************************************************************/
#ifndef _SOLVER_H_
#define _SOLVER_H_

#include <vector>
#include <string>

#include "pfspinstance.h"
#include "configuration.h"

struct solverResult{
        std::vector<int> sequence;
        long int objectiveScore;
        long int duration;
        long int durationLimit;
        long int neighboursTested;
        long int neighbourhoodsTested;
        float initAnnealingTemp;
        float finalAnnealingTemp;
        int annealingSteps;
        long int memorySize;
        long int memoryRetrievals;
};   

solverResult solveSchedule(PfspInstance pfspInstance,Configuration configuration);

#endif
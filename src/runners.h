/********************************************************************************/
/* PERMUTATION FLOW-SHOP SCHEDULING PROBLEM                                                                             
/* headerfile for runners.cpp
/********************************************************************************/
#ifndef RUNNER_H
#define RUNNER_H

    #include <string>
    #include "configuration.h"

    void runOneTest(Configuration configuration);
    void runAllTestsInFolder(std::string folder,SolverMemory solverMemory,int nbrOfIterations);
    void runVNDTestsInFolder(string folder,Neigbourhood neighboorhoodMethod,SolverMemory solverMemory,int nbrOfIterations);
    void runAnnealPertubationTestsInFolder(string folder,string testType, SolverMemory solverMemory,int nbrOfIterations);

#endif
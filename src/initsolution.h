/********************************************************************************/
/* PERMUTATION FLOW-SHOP SCHEDULING PROBLEM                                                                              
/* header file for initsolution.cpp
/********************************************************************************/
#ifndef _INITSOLUTION_H_
#define _INITSOLUTION_H_

#include <vector>

#include "pfspinstance.h"

using std::vector;

vector<int> getPermutatedInitSolution(PfspInstance pfspInstance);
vector<int> getSRZHeuristicInitSolution(PfspInstance pfspInstance,bool chooseRandomWhenEqual=true);
#endif
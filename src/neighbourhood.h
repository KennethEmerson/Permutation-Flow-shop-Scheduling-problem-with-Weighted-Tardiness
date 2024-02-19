/********************************************************************************/
/* PERMUTATION FLOW-SHOP SCHEDULING PROBLEM                                                                              
/* headerfile for neighbourhood.cpp
/********************************************************************************/
#ifndef _NEIGHBOURHOOD_H_
#define _NEIGHBOURHOOD_H_

#include <vector>

using std::vector;

vector<vector<int>> neighbourhoodTranspose(vector<int> &solution);
vector<vector<int>> neighbourhoodExchange(vector<int> &solution);
vector<vector<int>> neighbourhoodInsert(vector<int> &solution);
#endif
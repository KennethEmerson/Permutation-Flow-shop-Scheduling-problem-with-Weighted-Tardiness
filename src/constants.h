/********************************************************************************/
/* PERMUTATION FLOW-SHOP SCHEDULING PROBLEM                                                                         
/* header file containing all constants required to set the configuration of the
/* Algorithms
/********************************************************************************/

#ifndef CONSTANTS_H_
#define CONSTANTS_H_

#include <string>
using std::string;

const bool DEBUG=false;

// constants for the testruns
const int NBR_OF_ITERATIONS = 5;
const long int MAX_DURATION_5 = 5000;
const long int MAX_DURATION_50 = 160000;
const long int MAX_DURATION_100 = 2100000;

// constants used by the Simulated annealing testrun
const float ANNEAL_DECAY = 0.99995;
const float ANNEAL_RUN_LIMIT = 0;
const int INIT_ANNEAL_TEMP_MULTIPLIER = 2;
const int MINIMAL_ANNEAL_TEMP = 5000;

// constants used by the ILS testrun
const int PERTUBATION_VALUE_0 = 0;
const int PERTUBATION_VALUE_5 = 3;
const int PERTUBATION_VALUE_50 = 5;
const int PERTUBATION_VALUE_100 = 10;

// defaults for the configuration of a single test
const string DEFAULT_INITSOLUTION = "RANDOM";
const string DEFAULT_NEIGHBOURHOOD = "TRANSPOSE";
const string DEFAULT_PIVOTINGRULE = "FIRST";
const string DEFAULT_SOLVERMEMORY = "NONE";
const string DEFAULT_ANNEALING_DECAY = "0.992";
const string DEFAULT_ANNEALING_LIMIT = "0";
const string DEFAULT_PERTUBATION_VALUE = "0";

#endif
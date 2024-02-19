/********************************************************************************/
/* PERMUTATION FLOW-SHOP SCHEDULING PROBLEM                                                                               
/* This file contains the implementation of the configuration class
/* used for managing all the optimalisation configuration parameters
/********************************************************************************/

#include <string>
#include <vector>
#include "configuration.h"

using std::string, std::vector;

/**********************************************************************************/
/**
 * Configuration constructor to create a Configuration object based
 * on the arguments provided.
 * @param datafile the file containing the instance data 
 * @param initsolution the initial solution to be used
 * @param neighbourhood the method used to define the neighbourhood of the solution
 * @param pivotrule the pivotrule to be used
 * @param solverMemory indicates whether and which memory the solver should use.
 * @param annealingDecay the annealing decay to use (if applicable)
 * @param annealingLimit the annealing limit to use (if applicable)
 * @param pertubationValue the number of pertubations to use when using ILS
 * @param chooseRandomWhenEqual boolean value indicating if a random choice should
 *                              be made when two options are equally good.
 * @param seed the seed that should be used.
 * @return a Configuration object containing all configuration parameters
 */
/**********************************************************************************/
Configuration::Configuration(
        string datafile,
        InitSolution initSolution,
        Neigbourhood neighbourhood, 
        Pivotrules pivotRule,
        SolverMemory solverMemory,
        float annealingDecay,
        int annealingLimit,
        int pertubationValue,
        bool chooseRandomWhenEqual,
        int seed){

    this->datafile = datafile;
    this->initSolution = initSolution;
    this->neighbourhood = neighbourhood;
    this->pivotingRule = pivotRule;
    this->solverMemory = solverMemory;
    this->annealingDecay = annealingDecay;
    this->annealingLimit = annealingLimit;
    this->pertubationValue = pertubationValue;
    this->chooseRandomWhenEqual = chooseRandomWhenEqual;
    this->seed = seed;
};


/**********************************************************************************/
/**
 * Configuration destructor
 */
/**********************************************************************************/
Configuration::~Configuration(){
};


/**********************************************************************************/
/**
 * function to obtain the datafile path as set in the configuration
 * @return the path of the datafile
 */
/**********************************************************************************/
string Configuration::getDatafile(){
    return this->datafile;
};


/**********************************************************************************/
/**
 * function to obtain the initial solution setting as set in the configuration
 * object.
 * @return initial solution setting type
 */
/**********************************************************************************/
InitSolution Configuration::getInitSolution(){
    return this->initSolution;
};


/**********************************************************************************/
/**
 * function to obtain the neighboorhood setting as set in the configuration
 * object
 * @return the neighboorhood setting type
 */
/**********************************************************************************/
Neigbourhood Configuration::getNeighbourhood(){
    return this->neighbourhood;
};


/**********************************************************************************/
/**
 * function to obtain the neighboorhood setting as a vector as
 * set in the configuration object
 * @return vector containing the neighbourhoods strategies to use in the correct
 *         order. 
 */
/**********************************************************************************/
vector<Neigbourhood> Configuration::getNeighbourhoodVector(){
    vector<Neigbourhood> result;
    switch (this->neighbourhood){
        case TRANSPOSE: result ={TRANSPOSE}; break;
        case EXCHANGE:  result ={EXCHANGE}; break;
        case INSERT:    result ={INSERT}; break;
        case TEI:       result ={TRANSPOSE,EXCHANGE,INSERT};break;
        case TIE:       result ={TRANSPOSE,INSERT,EXCHANGE};break;
    }
    return  result;
}


/**********************************************************************************/
/**
 * function ot obtain the pivoting rule as set in the configuration
 * @return the pivoting rule as set in the configuration
 */
/**********************************************************************************/
Pivotrules Configuration::getPivotingRule(){
    return this->pivotingRule;
};


/**********************************************************************************/
/**
 * function to obtain the solver memory setting as set in the configuration
 * @return solver memory as set in the configuration
 */
/**********************************************************************************/
SolverMemory Configuration::getSolverMemory(){
    return this->solverMemory;
};


/**********************************************************************************/
/**
 * function to obtain the annealing decay setting as set in the configuration
 * @return the simulated Annealing decay as set in the configuration
 */
/**********************************************************************************/
float Configuration::getannealingDecay(){
    return this->annealingDecay;
};


/**********************************************************************************/
/**
 * function to obtain the annealing limit setting as set in the configuration
 * @return the simulated Annealing limit as set in the configuration
 */
/**********************************************************************************/
int Configuration::getAnnealingLimit(){
    return this->annealingLimit;
};


/**********************************************************************************/
/**
 * function to obtain the solver memory setting as set in the configuration
 * @return the ILS pertubation value as set in the configuration.
 * This will determine how much jobs are shifted to the right during each ILS 
 * pertubation.
 */
/**********************************************************************************/
int Configuration::getPertubationValue(){
    return this->pertubationValue;
};


/**********************************************************************************/
/**
 * function to obtain the setting indicating whether a random choice should
 * be made when two options are equally good.
 * @return a boolean flag indicating whether a random choice must be made when two
 * solution are equally good
 */
/**********************************************************************************/
bool Configuration::getChooseRandomWhenEqual(){
    return this->chooseRandomWhenEqual;
};


/**********************************************************************************/
/**
 * function to obtain the seed as set in the configuration
 * @return the seed as set in the configuration
 */
/**********************************************************************************/
int Configuration::getSeed(){
    return this->seed;
};


/**********************************************************************************/
/**
 * function to set the seed in the configuration
 * @param seed the new value of the seed as to be set in the configuration
 */
/**********************************************************************************/
void Configuration::setSeed(int seed){
    this->seed = seed;
};


/**********************************************************************************/
/**
 * internal function used to convert a boolean value to a corresponding string
 * @param value the boolean value to convert
 * @return the string representation of the boolean value
 */
/**********************************************************************************/
string boolToString(bool value){
    if(value) return "TRUE";
    else return "FALSE";
}


/**********************************************************************************/
/**
 * function to get a printable string with the summary of the configuration
 * @return configuration summary as a printable string
 */
/**********************************************************************************/
string Configuration::asString(){
    
    string str = "datafile: ";
    str += this->datafile;
    str += "\ninitial solution:";
    str += INIT_SOLUTION_STRINGS[this->initSolution];
    str += "\nneighbourhood: ";
    str += NEIGHBOURHOOD_STRINGS[this->neighbourhood];
    str += "\npivoting rule:  ";
    str += PIVOTING_RULE_STRINGS[this->pivotingRule];
    str += "\nsolver memory: ";
    str += SOLVER_MEMORY_STRINGS[this->solverMemory];
    str += "\nannealing decay (if applicable): ";
    str += std::to_string(this->annealingDecay);
    str += "\nannealing limit (if applicable): ";
    str += std::to_string(this->annealingLimit);
    str += "\nnumber of pertubations on intermediate solution: ";
    str += std::to_string(this->pertubationValue);
    str += "\nselect randomly when results are equal: ";
    str += boolToString(this->chooseRandomWhenEqual);
    str += "\nseed: ";
    str += std::to_string(this->seed);
    
    return str;
};



/********************************************************************************/
/* PERMUTATION FLOW-SHOP SCHEDULING PROBLEM                                                                          
/* header file for configuration.cpp
/********************************************************************************/

#ifndef CONFIGURATION_H_
#define CONFIGURATION_H_

#include <string>
#include <vector>
using std::string,std::vector;

// enum types for the given parameters
enum InitSolution {RANDOM, SRZH};
enum Neigbourhood {TRANSPOSE,EXCHANGE,INSERT,TEI,TIE};
enum Pivotrules {FIRST, BEST,ANNEAL};
enum SolverMemory {NONE,HASH,MAP};

// string versions of the enum types, used for printing the values
const string INIT_SOLUTION_STRINGS[2] {"RANDOM", "SRZH"};
const string NEIGHBOURHOOD_STRINGS[5] {"TRANSPOSE","EXCHANGE","INSERT","TEI","TIE"};
const string PIVOTING_RULE_STRINGS[3] {"FIRST","BEST","ANNEAL"};
const string SOLVER_MEMORY_STRINGS[3] {"NONE","HASH","MAP"};


// class definition
class Configuration{
    private:
        string datafile;
        Pivotrules pivotingRule;
        Neigbourhood neighbourhood;
        InitSolution initSolution;
        SolverMemory solverMemory;
        float annealingDecay;
        int annealingLimit;
        int pertubationValue;
        bool chooseRandomWhenEqual;
        int seed;
    
    public:
        Configuration(
            string datafile,
            InitSolution initsolution,
            Neigbourhood neighbourhood, 
            Pivotrules pivotrule,
            SolverMemory SolverMemory,
            float annealingDecay,
            int annealingLimit,
            int pertubationValue,
            bool chooseRandomWhenEqual,
            int seed);

        Configuration(int argc,char *argv[]);
        ~Configuration();
        
        Pivotrules getPivotingRule();
        Neigbourhood getNeighbourhood();
        vector<Neigbourhood> getNeighbourhoodVector();
        InitSolution getInitSolution();
        SolverMemory getSolverMemory();
        float getannealingDecay();
        int getAnnealingLimit();
        int getPertubationValue();
        bool getChooseRandomWhenEqual();
        int getSeed();
        void setSeed(int seed);
        string getDatafile();

        string asString();
        
};

#endif

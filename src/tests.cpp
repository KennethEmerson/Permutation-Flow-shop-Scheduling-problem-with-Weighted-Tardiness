

#include <iostream>
#include <vector>
#include <string>
#include <numeric>

#include "pfspinstance.h"
#include "configuration.h"
#include "initsolution.h"
#include "neighbourhood.h"
#include "helpers.h"

using std::string, std::endl;
using std::vector;
using std::find;



int main(int argc, char *argv[]) {
    
    /****************************************************************************************/
    // LOAD CONFIGURATION
    /****************************************************************************************/

    printTitle("Start tests with following configuration");
    Configuration config("./dummydata/example_data.txt",RANDOM,TRANSPOSE,BEST,NONE,0.9,0,0,false,0); 
    srand (config.getSeed());
    std::cout << config.asString() << endl;
    

    /****************************************************************************************/
    // LOAD DATA FROM FILE
    /****************************************************************************************/
    
    printTitle("Loading data from file");
    PfspInstance instance;
    instance.readDataFromFile(config.getDatafile());
    

    /****************************************************************************************/
    // TEST INITIAL SOLUTION
    /****************************************************************************************/
    printTitle("select an initial solution with Random Permutation");
    vector<int> initSolution = getPermutatedInitSolution(instance);
    std::cout << "init permutated solution: "; 
    printVector(initSolution);    
    std::cout << "due to constant seed, above answer should always be: 0 3 1 4 5 2" << endl;

    printTitle("select an initial solution with the SRZHeuristic");
    initSolution = getSRZHeuristicInitSolution(instance,false);
    std::cout << "init heuristic solution: "; 
    printVector(initSolution);
    std::cout << "result should match results in slides, being: 0 4 3 5 2 1" <<endl;

    /****************************************************************************************/
    // TEST NEIGHBOURHOODS
    /****************************************************************************************/
    vector<int> solution {0,1,2,3,4,5};

    printTitle("create transpose neighbourhood based on initial solution 0 1 2 3 4 5");
    vector<vector<int>> neigbourhood = neighbourhoodTranspose(solution);
    printVector(solution);
    printVectorMatrix(neigbourhood);
    

    printTitle("create exchange neighbourhood based on initial solution 0 1 2 3 4 5");
    neigbourhood = neighbourhoodExchange(solution);
    printVector(solution);
    printVectorMatrix(neigbourhood);

    printTitle("create insert neighbourhood based on initial solution 0 1 2 3 4 5");
    neigbourhood = neighbourhoodInsert(solution);
    printVector(solution);
    printVectorMatrix(neigbourhood);
    
    
    printTitle("calculate weighted tardiness of dummy data as provided in slides for solution ");
    std::cout << "initial solution: ";
    printVector(solution);
    solutionMetrics metrics = instance.computeSolutionMetrics(solution);
    std::cout << "completion times: ";
    printVector(metrics.completionTimes);
    std::cout << "sum of completion times: " << metrics.sumOfCompletionTimes << endl;
    std::cout << "sum of weighted completion times: " << metrics.sumOfweightedCompletionTimes << endl;
    std::cout << "makespan: " <<  metrics.makespan << endl;
    std::cout << "weighted tardiness: " <<  metrics.tardiness << endl;

    printHorizontalLine();
    
    return 0;
}
/********************************************************************************/
/* PERMUTATION FLOW-SHOP SCHEDULING PROBLEM                                                                            
/* contains the actual solver function
/********************************************************************************/
#include <iostream>
#include <limits>
#include <chrono>
#include <map>
#include <unordered_map>
#include <random>
#include <algorithm>
#include <thread>

#include "solver.h"

#include "constants.h"
#include "helpers.h"
#include "configuration.h"
#include "initsolution.h"
#include "neighbourhood.h"
#include "pfspinstance.h"

using std::endl;
using std::vector;


/**********************************************************************************/
/**
 * helper hashfunction to hash a vector
 * inspired by https://stackoverflow.com/questions/29855908/c-unordered-set-of-vectors
 */
/**********************************************************************************/
struct VectorHash {
    size_t operator()(const std::vector<int>& v) const {
        std::hash<int> hasher;
        size_t seed = 0;
        for (int i : v) {
            seed ^= hasher(i) + 0x9e3779b9 + (seed<<6) + (seed>>2);
        }
        return seed;
    }
};


/**********************************************************************************/
/**
 * calculates the Metropolis Probability for a given solution objective score
 * based on the given best objective score and the annealing temperature
 * @param result the actual objective score result
 * @param bestResult the best objective score found so far
 * @param temperature the annealing temperature to use
 * @return the Metropolis Probability for the given solution
 */
/**********************************************************************************/
float computeMetropolisProbability(int result, int bestResult,float temperature){
    if (result<bestResult) return 1;
    else return std::exp((bestResult-result)/temperature);
}


/**********************************************************************************/
/**
 * pertubate a given random number of jobs by moving them 
 * @param nbrOfMoves: the number of jobs to move
 * @param solution: the solution to pertubate 
 * @param seed: the seed to use 
 * @return the pertubated solution 
 */
/**********************************************************************************/
vector<int> pertubateSolution(int nbrOfMoves, vector<int> solution,std::mt19937 & generator)
{
    //TODO:include check that nbr is large enough
    
    if(nbrOfMoves < 2) 
        generateError("ERROR. file:solver.cpp, method:getPermutatedTestInitSolution.\nnbr should be larger than 1");

    vector< int > pertubatedSolution = solution;
    
    // fill shuffleIndex with sequential indexvalues starting from 1 (to overcome job 0)
    vector<int> shuffleIndex(solution.size()-1);
    std::iota(shuffleIndex.begin(), shuffleIndex.end(), 1);
    
    // shuffle the shuffleIndex
    std::shuffle(shuffleIndex.begin(), shuffleIndex.end(), generator);
    
    // take subsequent shuffled indexes from shuffleIndex and use them to move
    // jobs in solution so that value on first index goes to second index, second to third,..
    // and job at last index goes to the first index
    int first,second;
    first = pertubatedSolution[shuffleIndex[0]];
    for(int i= 0; i < nbrOfMoves-1; ++i){
        second = pertubatedSolution[shuffleIndex[i+1]];
        pertubatedSolution[shuffleIndex[i+1]] = first;
        first =second;
    }
    pertubatedSolution[shuffleIndex[0]] = second;
    return pertubatedSolution;
};


/**********************************************************************************/
/**
 *  solve one schedule 
 * @param pfspInstance the instance to use in the test
 * @param configuration the configuration object to use
 * @return a solverResult object containing the results
 */
/**********************************************************************************/
solverResult solveSchedule(PfspInstance pfspInstance,Configuration config){

    //timing inspired by: https://www.geeksforgeeks.org/measure-execution-time-function-cpp/
    auto start = std::chrono::high_resolution_clock::now();
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
    
    // select the max duration the solver can run based on the number of jobs in the instance
    long int maxDuration; 
    switch(pfspInstance.getNbrOfJobs()){
        case 5: maxDuration = MAX_DURATION_5;break;
        case 50: maxDuration = MAX_DURATION_50;break;
        case 100: maxDuration = MAX_DURATION_100;break;
        default: generateError("ERROR. file:solver.cpp, method:solveSchedule. NbrOfJobs must be 50 or 100");
    }

    // set random generator
    std::mt19937 generator(config.getSeed()); 
    std::uniform_real_distribution<> getRandomFloat(0, 1);


    // define variables
    int i,j;
    bool goToNextNeighbourhood = true;
    bool continueToNextNeighbour = true;
    bool betterNeighbourFound = false;
    long int neighboursCount = 0;
    long int neighbourhoodscount = 0;
    float annealingDecay = config.getannealingDecay();
    float initAnnealingTemp, annealingTemp;

    std::map<vector<int>, long int> memoryMap;
    std::unordered_map<vector<int>, long int,VectorHash> memoryHash;
    long int memoryUsedCount = 0;

    Neigbourhood neighbourhoodMethod;
    vector<vector<int>> neighbourhood;
    vector<int> neighbour;
    vector<int> bestSolution;
    vector<int> initSolution;
    long int bestObjectiveScore;
    long int neighbourObjectiveScore;
    int annealcounter = 0;
    int totalAnnealcounter = 0;



    /**********************************************/
    // CREATE INITIAL SOLUTION
    /**********************************************/
    if (config.getInitSolution()==RANDOM){
        bestSolution = getPermutatedInitSolution(pfspInstance);
    }
    else if ((config.getInitSolution()==SRZH)){
        bestSolution = getSRZHeuristicInitSolution(pfspInstance,config.getChooseRandomWhenEqual());
    }
    else generateError("ERROR. file:solver.cpp, method:solveSchedule. chosen intitial solution is not implemented");
            
    bestObjectiveScore  = pfspInstance.computeSolutionTardiness(bestSolution);
    annealingTemp = bestObjectiveScore * INIT_ANNEAL_TEMP_MULTIPLIER;
;
    initAnnealingTemp = annealingTemp;
    initSolution = bestSolution;

    /**********************************************/
    // iterate over neighbourhoods as long as going to
    // a next neighbourhood is allowed and the max
    // duration is not exceeded
    /**********************************************/
    while(goToNextNeighbourhood && duration < std::chrono::milliseconds(maxDuration)){
        stop = std::chrono::high_resolution_clock::now();
        duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);

        goToNextNeighbourhood = false;
        continueToNextNeighbour = true;
        ++neighbourhoodscount;

        // get vector containing the neighbourhood construction methods to use
        // and iterate over them as long as no better solution is found
        vector<Neigbourhood> neighbourhoodVector = config.getNeighbourhoodVector(); 
        for(j=0;j < neighbourhoodVector.size() && !goToNextNeighbourhood;++j){
            
            // create neighbourhood from initSolution as provided from the previous iteration
            neighbourhoodMethod = neighbourhoodVector[j];
            switch (neighbourhoodMethod){
                case TRANSPOSE: neighbourhood = neighbourhoodTranspose(initSolution); break;
                case EXCHANGE:  neighbourhood = neighbourhoodExchange(initSolution); break;
                case INSERT:    neighbourhood = neighbourhoodInsert(initSolution); break;
                default: generateError("ERROR. file:solver.cpp, method:solveSchedule. chosen neighbourhood is not implemented");
            }

            vector<long int> neighbourObjectiveScores(neighbourhood.size());

            // create neighbourhood index to decide the order in which neighbours get evaluated
            // if annealing is used, randomize the order in which neighbours get selected
            std::vector<int> neighbourSelectionOrder(neighbourhood.size());
            std::iota(neighbourSelectionOrder.begin(), neighbourSelectionOrder.end(), 0);
            if(config.getPivotingRule() == ANNEAL)
                std::shuffle(neighbourSelectionOrder.begin(), neighbourSelectionOrder.end(), generator);

        
            //-------------------------------------------------------------------------------------
            // iterate over neighbours in neighbourhood 
            //-------------------------------------------------------------------------------------
            // loop through all neighbours in neighbourhood as long as continueToNextNeighbour flag is high 
            for( i = 0; (i < neighbourhood.size()) && continueToNextNeighbour; ++i){
                ++neighboursCount;
                neighbour = neighbourhood[neighbourSelectionOrder[i]];
                
                
                //-------------------------------------------------------------------------------------
                // get neighbour score    
                //-------------------------------------------------------------------------------------
                //using map memory to store and retrieve computed Tardiness 
                if(config.getSolverMemory() == MAP){
                    if (auto search = memoryMap.find(neighbour); search != memoryMap.end()){
                            neighbourObjectiveScore = search->second;
                            ++memoryUsedCount;
                    }
                    else{
                        neighbourObjectiveScore = pfspInstance.computeSolutionTardiness(neighbour);
                        memoryMap[neighbour] = neighbourObjectiveScore;
                    }
                }

                //using unordered set with hash memory to store and retrieve computed Tardiness 
                else if(config.getSolverMemory()==HASH){
                    if (auto search = memoryHash.find(neighbour); search != memoryHash.end()){
                            neighbourObjectiveScore = search->second;
                            ++memoryUsedCount;
                    }
                    else{
                        neighbourObjectiveScore = pfspInstance.computeSolutionTardiness(neighbour);
                        memoryHash[neighbour] = neighbourObjectiveScore;
                    }
                }

                // without memory
                else if(config.getSolverMemory() == NONE){
                    neighbourObjectiveScore = pfspInstance.computeSolutionTardiness(neighbour);
                } 
                
                neighbourObjectiveScores[i] = neighbourObjectiveScore;

                //-------------------------------------------------------------------------------------
                // evaluate neighbour score    
                // test whether new score is better than previous
                // if ChooseRandomWhenEqual is selected a random choose will be made when objective scores are equal
                //-------------------------------------------------------------------------------------
                if((neighbourObjectiveScore < bestObjectiveScore) || 
                    (config.getChooseRandomWhenEqual() && (neighbourObjectiveScore == bestObjectiveScore) && (generator() % 2) == 0))
                {
                    bestObjectiveScore = neighbourObjectiveScore;
                    bestSolution = neighbour;
                    initSolution = bestSolution;
                    if(config.getPivotingRule() == FIRST) continueToNextNeighbour = false;
                    goToNextNeighbourhood = true;
                    annealcounter = 0;
                }
                

                             
            } // END OF FOR LOOP FOR SEARCHING BEST IN NEIGHBOURHOOD

            // if simulated annealing is set, allow choosing a worse solution for the next neighbourhood if no better solution was found
            if(config.getPivotingRule() == ANNEAL && 
                !goToNextNeighbourhood && 
                (annealcounter < config.getAnnealingLimit() || config.getAnnealingLimit() == 0)){
                
                // calculate the Metropolis probabilities
                std::vector<float> probabilities(neighbourObjectiveScores.size());
                std::transform(
                    neighbourObjectiveScores.begin(),
                    neighbourObjectiveScores.end(),
                    probabilities.begin(),
                    [bestObjectiveScore,annealingTemp](int x) { return computeMetropolisProbability(x,bestObjectiveScore,annealingTemp);}); 
                std::discrete_distribution<> distribution(probabilities.begin(), probabilities.end());
                
                // choose a new initSolution based on the Metropolis probabilities
                int chosenValue = distribution(generator);
                initSolution = neighbourhood[chosenValue];
                ++annealcounter;
                ++totalAnnealcounter;
                annealingTemp = annealingTemp * annealingDecay;
                if(annealingTemp <= MINIMAL_ANNEAL_TEMP) annealingTemp = MINIMAL_ANNEAL_TEMP;
                goToNextNeighbourhood = true;

            }
            
            // If ILS is selected, pertubate the best found solution so far and check if this pertubed solution
            // results in a better objective score. If yes, use the pertubed solution as new best solution.
            // if no, keep the original best solution.
            if(config.getPertubationValue() > 2){
                vector<int> pertubatedSolution = pertubateSolution(config.getPertubationValue(),bestSolution,generator);
                long int pertubatedObjectiveScore = pfspInstance.computeSolutionTardiness(pertubatedSolution);
                if (pertubatedObjectiveScore < bestObjectiveScore) {
                    initSolution = pertubatedSolution;
                    bestSolution = pertubatedSolution;
                }
                else initSolution = bestSolution;
                goToNextNeighbourhood = true;
            }      
        }
    } 
 
    //-------------------------------------------------------------------------------------
    // finalize solving
    //-------------------------------------------------------------------------------------
    stop = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
    

    int memorySize;
    if(config.getSolverMemory() == MAP) memorySize = memoryMap.size();
    else if(config.getSolverMemory()==HASH) memorySize = memoryHash.size();
    else memorySize = 0;

    solverResult result = {
        .sequence = bestSolution,
        .objectiveScore = bestObjectiveScore,
        .duration = duration.count(),
        .durationLimit = maxDuration,
        .neighboursTested = neighboursCount,
        .neighbourhoodsTested = neighbourhoodscount,
        .initAnnealingTemp = initAnnealingTemp,
        .finalAnnealingTemp = annealingTemp,
        .annealingSteps = totalAnnealcounter,
        .memorySize = memorySize,
        .memoryRetrievals = memoryUsedCount
        };
    return result;

}


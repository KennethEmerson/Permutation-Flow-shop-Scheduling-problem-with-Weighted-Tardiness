/********************************************************************************/
/* PERMUTATION FLOW-SHOP SCHEDULING PROBLEM                                                                            
/* This file contains the function for creating the initial solution
/********************************************************************************/
#include <iostream>
#include <cstdlib>
#include <vector>
#include <algorithm>
#include <limits>

#include "initsolution.h"

#include "pfspinstance.h"
#include "helpers.h"

using std::vector, std::endl;

/**********************************************************************************/
/**
 * Method to get a random int between to given limits
 * @param min the minimum (inclusive) limit of the random value
 * @param max the maximum (exclusive) limit of the random value
 * @return a random int 
 */
/**********************************************************************************/

int generateRndPosition(int min, int max)
{
    return (std::rand() % max + min );
}


/**********************************************************************************/
/**
 * Method to create a random permutated initial solution for a given PfspInstance
 * @param pfspInstance the pfspInstance for which to create an initial solution 
 * @return a random permutated initial solution 
 */
/**********************************************************************************/
vector<int> getPermutatedInitSolution(PfspInstance pfspInstance)
{
    int nbrOfJobs = pfspInstance.getNbrOfJobs();
    vector< int > initSolution(nbrOfJobs+1);
    vector<bool> alreadyTaken(nbrOfJobs+1, false); // nbJobs elements with value false
    vector<int > choosenNumber(nbrOfJobs+1, 0);

    int nbj;
    int rnd, i, j, nbFalse;

    nbj = 0;
    for (i = nbrOfJobs; i >= 1; --i)
    {
        rnd = generateRndPosition(1, i);
        nbFalse = 0;

        /* find the rndth cell with value = false : */
        for (j = 1; nbFalse < rnd; ++j)
        if ( ! alreadyTaken[j] )
            ++nbFalse;
        --j;

        initSolution[j] = i;

        ++nbj;
        choosenNumber[nbj] = j;

        alreadyTaken[j] = true;
    }
    return initSolution;
};


/**********************************************************************************/
/**
 * Method to create a random permutated initial solution for a given PfspInstance
 * @param pfspInstance the pfspInstance for which to create an initial solution 
 * @param chooseRandomWhenEqual indicates if a random choice must be made when solutions
 *                              are equally good
 * @return a random permutated initial solution 
 */
/**********************************************************************************/
vector<int> getSRZHeuristicInitSolution(PfspInstance pfspInstance,bool chooseRandomWhenEqual)
{
    struct Job{
        int index;
        double timeWeightedSum;
    };

    Job job0 = {0,0};
    int nbrOfJobs = pfspInstance.getNbrOfJobs();
    vector<Job> jobs(nbrOfJobs+1,job0);
    int i,j;

    for(i = 1; i < nbrOfJobs+1; ++i)
    {
        Job job;
        job.index = i;
        job.timeWeightedSum = pfspInstance.getWeightedSumOfProcessTimes(i);
        jobs[i] = job;
    }

    // inspired by: https://stackoverflow.com/questions/2758080/how-to-sort-an-stl-vector
    std::sort(jobs.begin(), jobs.end(),
          [] (Job const& a, Job const& b) { return a.timeWeightedSum < b.timeWeightedSum;});

    vector <int> solution;
    solution.push_back(jobs[0].index);
    solution.push_back(jobs[1].index);
    
    for(i =2;i < nbrOfJobs+1;++i){
        int currentBestWct = std::numeric_limits<int>::max(); // set current best to max value
        vector <int> currentBestCandidate;
        
        for(j = 1; j < solution.size() + 1; ++j){
            vector<int>  candidate(solution); 
                        
            int tempjob = jobs[i].index;
            candidate.insert(candidate.begin() + j,tempjob);
            int candidateWct = pfspInstance.computeWeightedCompletionTimes(candidate);
            
            if(chooseRandomWhenEqual){
                if((candidateWct < currentBestWct) || ((candidateWct == currentBestWct) && ((rand() % 2) == 0))){
                    currentBestCandidate = candidate;
                    currentBestWct = candidateWct;
                }
            }
            else if(candidateWct <= currentBestWct){        
                currentBestCandidate = candidate;
                currentBestWct = candidateWct;
            }
        }
        solution = currentBestCandidate; 
    }
    return solution;  
}



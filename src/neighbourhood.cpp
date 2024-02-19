/********************************************************************************/
/* PERMUTATION FLOW-SHOP SCHEDULING PROBLEM                                                                              
/* file contains functions to create neighbourhoods
/********************************************************************************/
#include <vector>

#include "neighbourhood.h"

using std::vector, std::begin, std::end;;


/*************************************************************************/
/**
 * function to create a neighbourhood of a given a solution
 * using the transpose method
 * 
 * @param solution (vector<int>) tha actual solution given the sequence of jobs
 *                 prefixed with 0 at index 0 
 * @return vector<vector<int>> neighbourhood of the solution
 */
/*************************************************************************/
vector<vector<int>> neighbourhoodTranspose(vector<int> &solution){
    int i,j,tempjob;
    const int nbrJobs = solution.size() -1;
    vector<vector<int>> neighbourhood;
    
    for ( i = 1; i < nbrJobs; ++i ){
        vector<int> neighbour(solution); 
        tempjob = neighbour[i];
        neighbour[i] = neighbour[i+1];
        neighbour[i+1] = tempjob;
        neighbourhood.push_back(neighbour);
    }   
    return neighbourhood;
}


/*************************************************************************/
/**
 * function to create a neighbourhood of a given a solution
 * using the exchange method
 * 
 * @param solution (vector<int>) tha actual solution given the sequence of jobs
 *                 prefixed with 0 at index 0 
 * @return vector<vector<int>> neighbourhood of the solution
 */
/*************************************************************************/
vector<vector<int>> neighbourhoodExchange(vector<int> &solution){
    int i,j,tempjob;
    const int nbrJobs = solution.size() -1;
    vector<vector<int>> neighbourhood;
    
    for ( i = 1; i < nbrJobs; ++i ){
        for(j = i+1; j <= nbrJobs; ++j){
            vector<int> neighbour(solution); 
            int tempjob = neighbour[i];
            neighbour[i] = neighbour[j];
            neighbour[j] = tempjob;

            neighbourhood.push_back(neighbour);
        }   
    }
return neighbourhood;
}


/*************************************************************************/
/**
 * function to create a neighbourhood of a given a solution
 * using the insert method
 * 
 * @param solution (vector<int>) tha actual solution given the sequence of jobs
 *                 prefixed with 0 at index 0 
 * @return vector<vector<int>> neighbourhood of the solution
 */
/*************************************************************************/
vector<vector<int>> neighbourhoodInsert(vector<int> &solution){
    int i,j,tempjob;
    const int nbrJobs = solution.size() -1;
    vector<vector<int>> neighbourhood;
    
    for ( i = 1; i < nbrJobs; ++i ){
        for(j = i+1; j <= nbrJobs; ++j){
            vector<int> neighbour(solution); 
            
            // store in tempjob and erase first to prevent increasing vector size
            int tempjob = neighbour[i];
            neighbour.erase(neighbour.begin()+ i);
            neighbour.insert(neighbour.begin() + j,tempjob);
            
            neighbourhood.push_back(neighbour);
        }
    }   
    return neighbourhood;
}
/***************************************************************************
 *   Copyright (C) 2012 by Jérémie Dubois-Lacoste   *
 *   jeremie.dl@gmail.com   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/


#include <iostream>
#include <fstream>
#include <cmath>
#include <string>
#include <numeric>
#include <algorithm>

#include "pfspinstance.h"

#include "helpers.h"


using std::string, std::cout, std::endl;

/**********************************************************************************/
/**
 * constructor
 */
/**********************************************************************************/
PfspInstance::PfspInstance()
{
}


/**********************************************************************************/
/**
 * destructor
 */
/**********************************************************************************/
PfspInstance::~PfspInstance()
{
}


/**********************************************************************************/
/**
 * aux method to allow the memory for the processing times matrix 
 * @param nbJ number of jobs
 * @param nbM number of machines
 */
/**********************************************************************************/
/* : */
void PfspInstance::allowMatrixMemory(int nbJ, int nbM)
{
    processingTimesMatrix.resize(nbJ+1);          // 1st dimension

    for (int cpt = 0; cpt < nbJ+1; ++cpt)
        processingTimesMatrix[cpt].resize(nbM+1); // 2nd dimension

    dueDates.resize(nbJ+1);
    priority.resize(nbJ+1);
}


/**********************************************************************************/
/**
 * 
 * @param
 */
/**********************************************************************************/
/* Read the instance from file : */
bool PfspInstance::readDataFromFile(string fileName,bool verbose)
{
	bool everythingOK = true;
	int j, m; // iterators
	long int readValue;
	string str;
	std::ifstream fileIn;
	int aux2;
	string fileNameOK = "";

	aux2 = (fileName.rfind('/'));
    string auxFilename;
	if (aux2 == std::string::npos)
		fileNameOK = fileName;
	else
		fileNameOK = fileName.substr(aux2+1);

	if (verbose) cout << "name : " << fileNameOK << endl;
	if (verbose) cout << "file : " << fileName << endl;

	fileIn.open(fileName);

	if ( fileIn.is_open() ) {
        if (verbose) cout << "File " << fileName << " is now open, start to read..." << std::endl;

		fileIn >> nbrOfJobs;
        if (verbose) cout << "Number of jobs : " << nbrOfJobs << std::endl;
		fileIn >> nbrOfMachines;
        if (verbose) cout << "Number of machines : " << nbrOfMachines << std::endl;
        if (verbose) cout << "Allow memory for the matrix..." << std::endl;
		allowMatrixMemory(nbrOfJobs, nbrOfMachines);
        if (verbose) cout << "Memory allowed." << std::endl;
        if (verbose) cout << "Start to read matrix..." << std::endl;

		for (j = 1; j <= nbrOfJobs; ++j)
		{
			for (m = 1; m <= nbrOfMachines; ++m)
			{
				fileIn >> readValue; // The number of each machine, not important !
				fileIn >> readValue; // Process Time

				processingTimesMatrix[j][m] = readValue;
			}
		}
        fileIn >> str; // this is not read

		for (j = 1; j <= nbrOfJobs; ++j)
		{
			fileIn >> readValue; // -1
			fileIn >> readValue;
			dueDates[j] = readValue;
			fileIn >> readValue; // -1
			fileIn >> readValue;
            priority[j] = readValue;
		}

        if (verbose) cout << "All is read from file." << std::endl;
		fileIn.close();
	}
	else
	{
		printHorizontalLine();
        cout << 
        "ERROR. file:pfspInstance.cpp, method:readDataFromFile," <<
        "error while opening file: " << endl << fileName << std::endl <<
        "program terminated" << endl;
        printHorizontalLine();
		everythingOK = false;
	}

	return everythingOK;
}


/**********************************************************************************/
/**
 * 
 */
/**********************************************************************************/
int PfspInstance::getNbrOfJobs()
{
  return nbrOfJobs;
}


/**********************************************************************************/
/**
 * 
 */
/**********************************************************************************/
int PfspInstance::getNbrOfMachines()
{
  return nbrOfMachines;
}


/**********************************************************************************/
/**
 * 
 */
/**********************************************************************************/
long int PfspInstance::getPriority(int job){
    if (job == 0){
        std::cout << "ERROR. file:pfspInstance.cpp, method:getPriority. job 0 can not have a priority" << endl;
    }
        return this->priority[job];
}

/**********************************************************************************/
/**
 * 
 */
/**********************************************************************************/
long int PfspInstance::getDueDate(int job){
    if (job == 0){
        std::cout << "ERROR. file:pfspInstance.cpp, method:getPriority. job 0 can not have a due date" << endl;
    }
        return this->dueDates[job];
}

/**********************************************************************************/
/**
 * 
 */
/**********************************************************************************/
long int PfspInstance::getProcessTime(int job, int machine)
{
    if (job == 0)
        return 0;
    else
    {
        if ((job < 1) || (job > nbrOfJobs) || (machine < 1) || (machine > nbrOfMachines))
        std::cout << "ERROR. file:pfspInstance.cpp, method:getProcessTime. Out of bound. job=" << job
            << ", machine=" << machine << std::endl;

        return processingTimesMatrix[job][machine];
    }
}


/**********************************************************************************/
/**
 * 
 */
/**********************************************************************************/
long int PfspInstance::getSumOfProcessTimes(int job){
    if (job == 0)
        return 0;
    else{
        if ((job < 1) || (job > nbrOfJobs))
        std::cout << "ERROR. file:pfspInstance.cpp, method:getSumOfProcessTimes. Out of bound. job=" << job << std::endl;
    
        return std::accumulate(processingTimesMatrix[job].begin(), processingTimesMatrix[job].end(),0);
    }

}


/**********************************************************************************/
/**
 * 
 */
/**********************************************************************************/
double PfspInstance::getWeightedSumOfProcessTimes(int job){
    if (job == 0)
        return 0;
    else{
        int sumOfProcessTimes = this->getSumOfProcessTimes(job);
        int weight = this->getPriority(job);
        return static_cast<double>(sumOfProcessTimes)/weight;
    }
};


/**********************************************************************************/
/**
 * 
 */
/**********************************************************************************/
vector< long int > PfspInstance::computeCompletionTimes(vector< int > & sol)
{
	int j, m;
	int jobNumber;

	/* We need end times on previous machine : */
	vector< long int > previousMachineEndTime ( sol.size());
	/* And the end time of the previous job, on the same machine : */
	long int previousJobEndTime;
    
	/* 1st machine : */
	previousMachineEndTime[0] = 0;
	for ( j = 1; j < sol.size(); ++j )
	{
		jobNumber = sol[j];
		previousMachineEndTime[j] = previousMachineEndTime[j-1] + processingTimesMatrix[jobNumber][1];
	}
   
	for ( m = 2; m <= nbrOfMachines; ++m )
	{
		previousMachineEndTime[1] +=
				processingTimesMatrix[sol[1]][m];
		previousJobEndTime = previousMachineEndTime[1];

       
		for ( j = 2; j < sol.size(); ++j )
		{
            jobNumber = sol[j];
            
			if ( previousMachineEndTime[j] > previousJobEndTime )
			{
                previousMachineEndTime[j] = previousMachineEndTime[j] + processingTimesMatrix[jobNumber][m];
				previousJobEndTime = previousMachineEndTime[j];   
			}
			else
			{
                previousJobEndTime += processingTimesMatrix[jobNumber][m];
				previousMachineEndTime[j] = previousJobEndTime;
			}
		}
	}
    return previousMachineEndTime;
};


/**********************************************************************************/
/**
 * 
 */
/**********************************************************************************/
/* Compute the weighted sum of completion time of a given solution */
long int PfspInstance::computeWeightedCompletionTimes(vector< int > & sol)
{
	vector< long int > previousMachineEndTime = this->computeCompletionTimes(sol);
    long int wct = 0;
    int j;

	for ( j = 1; j< sol.size(); ++j )
        wct += previousMachineEndTime[j] * priority[sol[j]];
    
	return wct;
}

long int PfspInstance::computeSolutionTardiness(vector< int > & sol)
{
    int j;
    long int totalTardiness = 0;
    vector< long int > previousMachineEndTime = this->computeCompletionTimes(sol);

    for ( j = 1; j< sol.size(); ++j ){
        long int jobDueDate = this->getDueDate(j);
        long int jobCompletionDate = previousMachineEndTime[j];
        long int jobPriority = this->getPriority(j);
        totalTardiness += std::max(jobCompletionDate-jobDueDate, 0l)* jobPriority;

    }
    return totalTardiness;
}

solutionMetrics PfspInstance::computeSolutionMetrics(vector< int > & sol){
    int j;
    long int ct = 0;
    long int wct = 0;
    long int totalTardiness = 0;
    
    vector< long int > completionTimes = this->computeCompletionTimes(sol);
    const int complTimeSize = completionTimes.size();

    for ( j = 1; j< complTimeSize; ++j ){
        ct += completionTimes[j];
        wct += completionTimes[j] * priority[sol[j]];

        long int jobDueDate = this->getDueDate(j);
        long int jobCompletionDate = completionTimes[j];
        long int jobPriority = this->getPriority(j);
        totalTardiness += std::max(jobCompletionDate-jobDueDate, 0l)* jobPriority;

    }

    solutionMetrics metrics= {
        .solution=sol, 
        .completionTimes = completionTimes,
        .sumOfCompletionTimes = ct,
        .sumOfweightedCompletionTimes = wct,
        .makespan = completionTimes[complTimeSize-1],
        .tardiness = totalTardiness,
         };

    return metrics;
}


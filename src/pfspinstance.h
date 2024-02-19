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


#ifndef _PFSPINSTANCEWT_H_
#define _PFSPINSTANCEWT_H_

#include <string>
#include <vector>

using std::vector, std::string;

struct solutionMetrics{
    vector<int> solution;
    vector<long int> completionTimes;
    long int sumOfCompletionTimes;
    long int sumOfweightedCompletionTimes;
    long int makespan;
    long int tardiness;
};

class PfspInstance{
    private:
        int nbrOfJobs;
        int nbrOfMachines;
        std::vector< long int > dueDates;
        std::vector< long int > priority;

        std::vector< vector <long int> > processingTimesMatrix;

    public:
        PfspInstance();
        ~PfspInstance();

        /* Read write privates attributs : */
        int getNbrOfJobs();
        int getNbrOfMachines();

        /* Allow the memory for the processing times matrix : */
        void allowMatrixMemory(int nbJ, int nbM);

        /* Read\Write values in the matrix : */
        long int getProcessTime(int job, int machine);
        long int getSumOfProcessTimes(int job);
        double getWeightedSumOfProcessTimes(int job);
        long int getDueDate(int job);
        long int getPriority(int job);
    

        /* Read Data from a file : */
        bool readDataFromFile(string fileName,bool verbose= false);

        vector<long int> computeCompletionTimes(vector< int > & sol);
        long int computeWeightedCompletionTimes(vector< int > & sol);
        long int computeSolutionTardiness(vector< int > & sol);
        solutionMetrics computeSolutionMetrics(vector< int > & sol);
    };

#endif

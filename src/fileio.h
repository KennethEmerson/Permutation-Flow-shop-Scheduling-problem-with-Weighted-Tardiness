/********************************************************************************/
/* PERMUTATION FLOW-SHOP SCHEDULING PROBLEM                                                                              
/* the header file for fileio.cpp
/********************************************************************************/
#ifndef FILIO_H_
#define FILIO_H_

#include <string>
#include "solver.h"

class FileHandler{
    private:
        string filename;
        string time;

    public:
        FileHandler();
        ~FileHandler();
        string getFileName();
        void saveHeaderToFile();
        void save_result_to_file(solverResult solution, Configuration configuration);
};

vector<string> getAllFilesInDir(string directory);
#endif
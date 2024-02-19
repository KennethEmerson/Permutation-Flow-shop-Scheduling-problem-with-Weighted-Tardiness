/********************************************************************************/
/* PERMUTATION FLOW-SHOP SCHEDULING PROBLEM                                                                             
/* file contains all fileio operations
/********************************************************************************/
#include <string>
#include <fstream>
#include <chrono>
#include <filesystem>

#include"fileio.h"

#include "solver.h"
#include "helpers.h"
#include "configuration.h"

using std::endl, std::string;
namespace fs = std::filesystem;


//time strings based on:
//https://cplusplus.com/doc/tutorial/files/
//https://en.cppreference.com/w/cpp/chrono/c/strftime
//https://en.cppreference.com/w/cpp/chrono/c/strftime


/**********************************************************************************/
/**
 * create a filehandler object
 */
/**********************************************************************************/
FileHandler::FileHandler(){
    std::time_t time = std::time({});
    char timeString[std::size("yyyy-mm-dd hh:mm:ss")];
    std::strftime(std::data(timeString), std::size(timeString), "%F %T", std::gmtime(&time));
    std::string stringify(timeString);
    this->time = stringify;

    char timeStringFile[std::size("yyyymmdd_hh:mm:ss")];
    std::strftime(std::data(timeStringFile), std::size(timeStringFile), "%Y%m%d_%H%M%S", gmtime(&time));
    std::string timeString3(timeStringFile);
    this->filename =  "result_" + timeString3 + ".txt";
}


/**********************************************************************************/
/**
 *  filehandler object destructor
 */
/**********************************************************************************/
FileHandler::~FileHandler(){};


/**********************************************************************************/
/**
 * returns the filename used for storign the results
 * @return the filename used
 */
/**********************************************************************************/
string FileHandler::getFileName(){
    return this->filename;
}


/**********************************************************************************/
/**
 *  saves a predefined header to the file
 */
/**********************************************************************************/
void FileHandler::saveHeaderToFile(){
    std::ofstream resultfile;
    resultfile.open(this->filename, std::ios_base::app);
    resultfile << "-----------------------------------------------" << endl;
    resultfile << "Permutation Flowshop Scheduling Problem Results" << endl;
    resultfile << "Test performed at : " << this->time << endl; 
    resultfile << "-----------------------------------------------" << endl << endl;
    
    string columns = "file,";
    columns += "objectiveScore,";
    columns += "duration (ms),";
    columns += "max duration (ms),";
    columns += "memorytype,";
    columns += "memorysize,";
    columns += "memoryretrievals,";
    columns += "initSolution,";
    columns += "neighbourhood,";
    columns += "pivotRule,";
    columns += "annealDecay,";
    columns += "annealLimit,";
    columns += "annealSteps,";
    columns += "initAnnealTemp,";
    columns += "finalAnnealTemp,";
    columns += "nbrOfPertubations,";
    columns += "seed,";
    columns += "nbrOfNeighbourhoodsTested,";
    columns += "nbrOfNeighboursTested,";
    columns += "sequence";
    
    resultfile << columns << endl;
    resultfile.close();
    };


/**********************************************************************************/
/**
 * saves the results stored in the solution and configuration objects in the file
 * @param solution the solution object returned by the solver 
 * @param configuration the configuration object containing the configuration of
 *                      the test 
 */
/**********************************************************************************/
void FileHandler::save_result_to_file(solverResult solution, Configuration configuration){
    int aux;
    string fileNameOK = "";
    string datafile = configuration.getDatafile();
    aux = (datafile.rfind('/'));
    string auxFilename;
	if (aux == std::string::npos)
		fileNameOK = datafile;
	else
		fileNameOK = datafile.substr(aux+1);


    string str = fileNameOK;
    str += ",";
    str += std::to_string(solution.objectiveScore);
    str += ",";
    str += std::to_string(solution.duration);
    str += ",";
    str += std::to_string(solution.durationLimit);
    str += ",";
    str += SOLVER_MEMORY_STRINGS[configuration.getSolverMemory()];
    str += ",";
    str += std::to_string(solution.memorySize);
    str += ",";
    str += std::to_string(solution.memoryRetrievals);
    str += ",";
    str += INIT_SOLUTION_STRINGS[configuration.getInitSolution()];
    str += ",";
    str += NEIGHBOURHOOD_STRINGS[configuration.getNeighbourhood()];
    str += ",";
    str += PIVOTING_RULE_STRINGS[configuration.getPivotingRule()];
    str += ",";
    str += std::to_string(configuration.getannealingDecay());
    str += ",";
    str += std::to_string(configuration.getAnnealingLimit());
    str += ",";
    str += std::to_string(solution.annealingSteps);
    str += ",";
    str += std::to_string(solution.initAnnealingTemp);
    str += ",";
    str += std::to_string(solution.finalAnnealingTemp);
    str += ",";
    str += std::to_string(configuration.getPertubationValue());
    str += ",";
    str += std::to_string(configuration.getSeed());
    str += ",";
    str += std::to_string(solution.neighbourhoodsTested);
    str += ",";
    str += std::to_string(solution.neighboursTested);
    str += ",";
    str += vectorToString(solution.sequence);
    
    
    std::ofstream resultfile;
    resultfile.open(this->filename, std::ios_base::app);
    resultfile << str << endl;
    resultfile.close();
};


/**********************************************************************************/
/**
 * returns a vector with the filenames as found in a specific directory
 * @param directory: the directory to use
 * @return a vector of strings representing the filenames 
 */
/**********************************************************************************/
vector<string> getAllFilesInDir(string directory){
    vector<string> files;
    for (const auto & entry : fs::directory_iterator(directory)) 
        files.push_back(entry.path());
    return files;
};

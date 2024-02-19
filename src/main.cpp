/********************************************************************************/
/* PERMUTATION FLOW-SHOP SCHEDULING PROBLEM                                                                              
/* the main program
/********************************************************************************/
#include <string>
#include <filesystem>

#include "constants.h"
#include "configuration.h"
#include "configparser.h"
#include "helpers.h"
#include "runners.h" 

namespace fs = std::filesystem;

/**********************************************************************************/
/**
 * launches the program based on the command line parameters given
 */
/**********************************************************************************/
int main(int argc, char *argv[]) {

    string errorString;
    
    // run test on one file if first parameter is a file
    if(argc>1 && fs::is_regular_file(argv[1])){
        Configuration configuration = parseToConfiguration(argc,argv);
        runOneTest(configuration);
    }
    
    // else run test on all files in directory
    else if(argc>1 && fs::is_directory(argv[1])){
        SolverMemory solverMemory = NONE;
        
        // select memoization
        if(argc>3){
            string solverMemoryString = argv[3];
            if(solverMemoryString.compare("NONE")==0) solverMemory = NONE;
            else if(solverMemoryString.compare("HASH")==0) solverMemory = HASH;
            else if(solverMemoryString.compare("MAP")==0) solverMemory = MAP;
            else generateError("third command line parameter is not a valid solver memory");
        }

        // select testtype
        if(argc>2){
            string testType = argv[2];
            if(testType.compare("NONVND")==0){
                runAllTestsInFolder(argv[1],solverMemory, NBR_OF_ITERATIONS);
            }
            else if(testType.compare("TEI")==0){
                runVNDTestsInFolder(argv[1],TEI,solverMemory,NBR_OF_ITERATIONS);
            }
            else if(testType.compare("TIE")==0){
                runVNDTestsInFolder(argv[1],TIE,solverMemory,NBR_OF_ITERATIONS);
            }
            else if(testType.compare("ANNEAL")==0 || testType.compare("PERTUB")==0){
                runAnnealPertubationTestsInFolder(argv[1],testType,solverMemory,NBR_OF_ITERATIONS);
            }
            else generateError("second command line parameter is not a valid test type");
        }
        else{
            runAllTestsInFolder(argv[1],solverMemory,NBR_OF_ITERATIONS);
        }
    }

    // if the given first command line parameter is not a valid file or folder,
    // print an errorstring to the terminal and stop the program.
    else{
        errorString =  "command should at least provide a data file or folderpath\n";
        errorString += "please consult readme file for details.\n";
        errorString += "program terminated"; 
        generateError(errorString);

    };
    return 0;
}
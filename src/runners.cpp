/********************************************************************************/
/* PERMUTATION FLOW-SHOP SCHEDULING PROBLEM                                                                            
/* file contains functions to run the tests
/********************************************************************************/
#include <iostream>
#include <vector>
#include <string>
#include <filesystem>
#include <random>
#include <algorithm>
#include <future>
#include <ranges>
#include <thread>
#include <tuple>

#include "runners.h"

#include "constants.h"
#include "helpers.h"
#include "pfspinstance.h"
#include "configuration.h"
#include "configparser.h"
#include "solver.h"
#include "fileio.h"

using std::string, std::endl;
using std::vector;
using std::tuple;
namespace fs = std::filesystem;


/**********************************************************************************/
/**
 *  runs one test based on the given configuration
 * @param configuration the configuration object to be used
 */
/**********************************************************************************/
void runOneTest(Configuration configuration){
    
    // LOAD CONFIGURATION
    printTitle("Start optimization");
    std::cout << configuration.asString() << endl; 
    srand (configuration.getSeed());
    FileHandler fileHandler;
    
    // LOAD DATA FROM FILE
    printTitle("Loading data from file");
    PfspInstance pfspInstance;
    bool dataLoaded = pfspInstance.readDataFromFile(configuration.getDatafile(),true);
    if(!dataLoaded) exit(0);
    
    // SOLVE the instance
    solverResult result = solveSchedule(pfspInstance,configuration);
    
    // print the results on the teminal
    printTitle("final results:");
    std::cout << "best solution : ";
    printVector(result.sequence);
    std::cout << "objective function score: " << result.objectiveScore << endl;
    std::cout << "duration (ms): " << result.duration << endl;
    std::cout << "max duration (ms): " << result.durationLimit << endl;
    std::cout << "initial annealing temp : " << result.initAnnealingTemp << endl;
    std::cout << "final annealing temp : " << result.finalAnnealingTemp << endl;
    std::cout << "annealing steps: " << result.annealingSteps << endl;
    std::cout << "neighbours tested: " << result.neighboursTested << endl;
    std::cout << "neighbourhoods tested: " << result.neighbourhoodsTested << endl;
    std::cout << "neighbours stored in memory: " << result.memorySize << endl;
    std::cout << "neighbours retrieved from memory: " << result.memoryRetrievals << endl;
    printHorizontalLine();

    // save result in file
    fileHandler.saveHeaderToFile();
    fileHandler.save_result_to_file(result,configuration);
}


/**********************************************************************************/
/**
 * runs tests on multiple files
 * @param folder folder where the data files are stored
 * @param solverMemory solver memory to use
 * @param nbrOfIterations the number of iterations to use
 */
/**********************************************************************************/
void runAllTestsInFolder(string folder,SolverMemory solverMemory,int nbrOfIterations){
    string errorString;
    //check if directory is valid
    if(!fs::is_directory(folder)) {
        errorString =  "the provided directory " + folder + " could not be found\n";
        errorString += "program terminated";
        generateError(errorString);
    }

    //initialize variables
    int i;
    auto start = std::chrono::high_resolution_clock::now();
    vector <InitSolution> initSolutions {RANDOM,SRZH};
    vector <Neigbourhood> neighbourhoods {TRANSPOSE,EXCHANGE,INSERT};
    vector <Pivotrules> pivotRules {FIRST,BEST};
    FileHandler fileHandler;
    string file;

    // write header to file
    fileHandler.saveHeaderToFile();
    printTitle("Start optimizations");

    // loop over all files in directory
    for (const auto & entry : fs::directory_iterator(folder))
    { 
        auto startFile = std::chrono::high_resolution_clock::now();
        file = entry.path().string();
        std::cout << "solving file: " << file << " with " << nbrOfIterations <<" iterations"  << endl;

        // load instance from file
        PfspInstance pfspInstance;
        bool dataLoaded = pfspInstance.readDataFromFile(file,false);
        if(!dataLoaded) exit(0);
        
        // repeat the solving loop based on the nbr of iterations
        for(i=0; i< nbrOfIterations;++i){
            std::cout << "iteration: " << i << endl;
            // set seed for different runs on file
            int seed = time(NULL);
            srand (seed);

            // loop over all set initial solutions
            for (InitSolution initSolution : initSolutions)
            {
                // loop over all set neighbourhoods
                for ( Neigbourhood neighbourhood : neighbourhoods)
                {
                    // loop over all set pivot rules
                    for ( Pivotrules pivotRule : pivotRules)
                    {        
                        //create configuration based on settings of loops
                        Configuration configuration(
                            file,
                            initSolution,
                            neighbourhood,
                            pivotRule,
                            solverMemory,
                            ANNEAL_DECAY,
                            ANNEAL_RUN_LIMIT,
                            PERTUBATION_VALUE_0,
                            true,
                            seed);
                        
                        // solve instance with 
                        solverResult result = solveSchedule(pfspInstance,configuration);
                        
                        fileHandler.save_result_to_file(result,configuration);
                    };
                };    
            };
        };

        // calculate time required for solving one instance
        auto stopFile = std::chrono::high_resolution_clock::now();
        auto fileDuration = std::chrono::duration_cast<std::chrono::seconds>(stopFile - startFile);
        std::cout << "duration: " << fileDuration.count() << endl;
    };

    // calculate time required for solving all instances
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::seconds>(stop - start);
    
    // print overall duration and filelocation
    printHorizontalLine();
    std::cout << "optimalisation finished and results stored to file" << endl;
    std::cout << "with filename: " << fileHandler.getFileName() << endl;
    std::cout << "total duration (seconds): " << duration.count() << endl;
    printHorizontalLine();
}


/**********************************************************************************/
/**
 * runs VND tests on multiple files 
 * @param folder folder where the data files are stored
 * @param neighboorhoodMethod the specific neighboorhood method to use
 * @param solverMemory solver memory to use
 * @param nbrOfIterations the number of iterations to use
 */
/**********************************************************************************/
void runVNDTestsInFolder(string folder,Neigbourhood neighboorhoodMethod,SolverMemory solverMemory,int nbrOfIterations){
    string errorString;
    //check if directory is valid
    if(!fs::is_directory(folder)) {
        errorString =  "the provided directory " + folder + " could not be found\n";
        errorString += "program terminated";
        generateError(errorString);
    }

    //initialize variables
    int i;
    auto start = std::chrono::high_resolution_clock::now();
    FileHandler fileHandler;
    string file;

    // write header to file
    fileHandler.saveHeaderToFile();
    printTitle("Start optimizations");

    // loop over all files in directory
    for (const auto & entry : fs::directory_iterator(folder))
    { 
        auto startFile = std::chrono::high_resolution_clock::now();
        file = entry.path().string();
        std::cout << "solving file: " << file << " with " << nbrOfIterations <<" iterations"  << endl;

        // load instance from file
        PfspInstance pfspInstance;
        bool dataLoaded = pfspInstance.readDataFromFile(file,false);
        if(!dataLoaded) exit(0);
        
        // repeat the solving loop based on the nbr of iterations
        for(i=0; i< nbrOfIterations;++i){
            std::cout << "iteration: " << i << endl;
            // set seed for different runs on file (+i is required to guarantee different seeds)
            int seed = time(NULL) + i;
            srand (seed);
    
            //create configuration based on settings of loops
            Configuration configuration(
                file,
                SRZH,
                neighboorhoodMethod,
                FIRST,
                solverMemory,
                ANNEAL_DECAY,
                ANNEAL_RUN_LIMIT,
                PERTUBATION_VALUE_0,
                true,
                seed);
            
            // solve instance with the given configuration
            solverResult result = solveSchedule(pfspInstance,configuration);
            
            fileHandler.save_result_to_file(result,configuration);
                  
        };

        // calculate time required for solving one instance
        auto stopFile = std::chrono::high_resolution_clock::now();
        auto fileDuration = std::chrono::duration_cast<std::chrono::seconds>(stopFile - startFile);
        std::cout << "duration: " << fileDuration.count() << endl;
    };

    // calculate time required for solving all instances
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::seconds>(stop - start);
    
    // print overall duration and filelocation
    printHorizontalLine();
    std::cout << "optimalisation finished and results stored to file" << endl;
    std::cout << "with filename: " << fileHandler.getFileName() << endl;
    std::cout << "total duration (seconds): " << duration.count() << endl;
    printHorizontalLine();
}


/**********************************************************************************/
/**
 * helper function used to perform one test in a separate thread
 * @param pfspInstance the pfspInstance to solve
 * @param config the default configuration to use 
 * @param seed the seed to use by the specific thread
 * @return tuple containing the configuration and solveresult
 */
/**********************************************************************************/
tuple<Configuration,solverResult> runOneThread(PfspInstance pfspInstance,Configuration sourceConfig,int seed){

    // a separate config is build with the specfic seed and the general config parameters
    // in order to be thread safe
    Configuration config(
            sourceConfig.getDatafile(),
            sourceConfig.getInitSolution(),
            sourceConfig.getNeighbourhood(),
            sourceConfig.getPivotingRule(),
            sourceConfig.getSolverMemory(),
            sourceConfig.getannealingDecay(),
            sourceConfig.getAnnealingLimit(),
            sourceConfig.getPertubationValue(),
            true,
            seed);

    // solve the instance based on the given configuration
    //inspired by : https://stackoverflow.com/questions/14718124/how-to-easily-make-stdcout-thread-safe
    std::stringstream msg;
    msg << "thread: " << std::this_thread::get_id() << " with seed: " << config.getSeed() << "\n"; 
    std::cout << msg.str();
    solverResult solverResult = solveSchedule(pfspInstance,config);
    return {config,solverResult};
}

/**********************************************************************************/
/**
 * helper function used to determine if a thread has finished
 * inspired by https://stackoverflow.com/questions/42041197/c-threads-for-background-loading/42042110#42042110
 * @param f the pointer to the future under supervision
 * @return True if the future has resolved, False otherwise
 */
/**********************************************************************************/
bool isReady(const std::future<tuple<Configuration,solverResult>> &f)
{
    return f.wait_for(std::chrono::seconds(0)) == std::future_status::ready;
}


/**********************************************************************************/
/**
 * runs Simulate Annealing or ILS tests on multiple files using multiple threads
 * @param folder folder where the data files are stored
 * @param testType "ANNEAL" for simulated annealing or "PERTUB" for ILS
 * @param solverMemory solver memory to use
 * @param nbrOfIterations nbr of iterations and thus nb
 */
/**********************************************************************************/
void runAnnealPertubationTestsInFolder(string folder,string testType, SolverMemory solverMemory,int nbrOfIterations){
    
    auto start = std::chrono::high_resolution_clock::now();
    FileHandler fileHandler;
    Pivotrules pivotRule; 
    string file,errorString;
    float annealDecay;
    int annealLimit,pertubationValue,i;
    
    //check if directory is valid
    if(!fs::is_directory(folder)) {
        errorString =  "the provided directory " + folder + " could not be found\n";
        errorString += "program terminated";
        generateError(errorString);
    }

    fileHandler.saveHeaderToFile();
    printTitle("Start optimizations");

    // loop over all files in directory
    for (const auto & entry : fs::directory_iterator(folder))
    { 
        auto startFile = std::chrono::high_resolution_clock::now();
        file = entry.path().string();
        std::cout << "solving file: " << file << " with " << nbrOfIterations <<" iterations"  << endl;


        // load instance from file
        PfspInstance pfspInstance;
        bool dataLoaded = pfspInstance.readDataFromFile(file,false);
        if(!dataLoaded) exit(0);
        
        // set run specific values
        if (testType.compare("ANNEAL") == 0){
            annealDecay = ANNEAL_DECAY;
            annealLimit = ANNEAL_RUN_LIMIT;
            pertubationValue = 0;
            pivotRule = ANNEAL;
        }
        else if (testType.compare("PERTUB") == 0){
            annealDecay = ANNEAL_DECAY;
            annealLimit = 0;
            pivotRule = FIRST;
            switch(pfspInstance.getNbrOfJobs()){
                case 5: pertubationValue = PERTUBATION_VALUE_5;break;
                case 50: pertubationValue = PERTUBATION_VALUE_50;break;
                case 100: pertubationValue = PERTUBATION_VALUE_100;break;
                defaul: generateError("pertubation value for given number of jobs is not defined");
            }
        }
        else {
            errorString = "ERROR. file:runners.cpp, method:readDataFromFile\n";
            errorString += "an undefined testype was provided";
            generateError(errorString);
        }
        
        // create generic configuration
        Configuration configuration(
            file,
            SRZH,
            EXCHANGE,
            pivotRule,
            solverMemory,
            annealDecay,
            annealLimit,
            pertubationValue,
            true,
            0);
        
        // Generate a vector of custom seeds (one for each thread)
        // inspired by https://stackoverflow.com/questions/50798713/c-generating-random-vector
        std::random_device rd;
        std::mt19937 generator(rd());
        std::vector<int> seeds(nbrOfIterations); 
        std::uniform_int_distribution<int> dis(0, 100000);
        std::generate(seeds.begin(), seeds.end(), [&](){ return dis(generator); });
        

        // create vector of futures which catches results of threads
        // inspired by https://stackoverflow.com/questions/42041197/c-threads-for-background-loading/42042110#42042110
        vector<std::future<tuple<Configuration,solverResult>>> futures(nbrOfIterations);
        for(int i = 0; i < futures.size();++i)
        {
            futures[i] = std::async(std::launch::async, runOneThread, pfspInstance,configuration,seeds[i]);
        }

        // boolean vector which keeps track of whether thread has finished or not
        // inspired by https://stackoverflow.com/questions/42041197/c-threads-for-background-loading/42042110#42042110
        vector<bool> finishedTasks(nbrOfIterations);
        std::fill(finishedTasks.begin(), finishedTasks.end(), false);

        int numFinishedTasks = 0;

        // loop that keeps iterating while not all threads are finished
        // during each iteration all non resolved futures are checked in order to establish
        // if they have been resolved or not, if so, the results returnd by the future
        // are stored in the file
        do {
            for(size_t i = 0; i < nbrOfIterations; ++i)
            {
                // check if task was not finished before and is ready now
                if(!finishedTasks[i] && isReady(futures[i]))
                {
                    finishedTasks[i] = true;
                    numFinishedTasks++;
                    auto [threadConfig,result] = futures[i].get();
                    fileHandler.save_result_to_file(result,threadConfig);
                }
            }
        }
        while(numFinishedTasks < nbrOfIterations);
    }
    
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::seconds>(stop - start);
    
    // print overall duration and filelocation
    printHorizontalLine();
    std::cout << "Hardware concurrency: " << std::thread::hardware_concurrency() << "\n";
    std::cout << "optimalisation finished and results stored to file" << endl;
    std::cout << "with filename: " << fileHandler.getFileName() << endl;
    std::cout << "total duration (seconds): " << duration.count() << endl;
    printHorizontalLine();
}


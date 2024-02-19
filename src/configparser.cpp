/********************************************************************************/
/* PERMUTATION FLOW-SHOP SCHEDULING PROBLEM                                                                     
/* This file contains the parser for creating a configuration object based on
/* the given command line parameters
/********************************************************************************/

#include <string>
#include <chrono>
#include <filesystem>
#include <random>

#include "configparser.h"

#include "constants.h"
#include "configuration.h"
#include "helpers.h"

using std::string;
namespace fs = std::filesystem;

// default settings to use



/**********************************************************************************/
/**
 * function to create a Configuration object based on the command line parameters
 * @param argc number of command line parameters 
 * @param argv array of char[] containing the command line parameters 
 * @returns Configuration object 
 */
/**********************************************************************************/
Configuration parseToConfiguration(int argc,char *argv[]){
    
    // define default values
    string errorString;
    string tempDatafile = "unknown";
    string tempInitSolution = DEFAULT_INITSOLUTION;    
    string tempNeighbourhood = DEFAULT_NEIGHBOURHOOD;
    string tempPivotingRule = DEFAULT_PIVOTINGRULE;
    string tempSolverMemory = DEFAULT_SOLVERMEMORY;
    string tempAnnealingDecay = DEFAULT_ANNEALING_DECAY;
    string tempAnnealingLimit = DEFAULT_ANNEALING_LIMIT;
    string tempPertubationValue = DEFAULT_PERTUBATION_VALUE;
    string tempseed = "N";
    
    // define final variables
    InitSolution initSolution;
    Neigbourhood neighbourhood;
    Pivotrules pivotRule;
    SolverMemory solverMemory;
    int annealingLimit, pertubationValue, seed;
    float annealingDecay;

    // define internal flags used within the function
    bool seedIsGiven = false;
    int correctInput = true;

    // print error and stop program if no command line arguments are given.
    if(argc==1){
        errorString =  "command should at least provide a data file or folderpath\n";
        errorString += "please consult readme file for details.\n";
        errorString += "program terminated"; 
        generateError(errorString);

    };

    /*****************************************************************************/
    /* parse inputs
    /*****************************************************************************/
    //first command line parameter must be the file containing the data
    tempDatafile = argv[1];

    // read and parse the remaining command line arguments
    for (int i = 2; i < argc; i+=2){    
        if((argv[i][0]== '-') && ((i+1)<argc)){                    
                                        
            switch (argv[i][1]){                                
                case 'I':
                case 'i': tempInitSolution = argv[i+1];break; 
                case 'N':
                case 'n': tempNeighbourhood = argv[i+1];break;     
                case 'P':
                case 'p': tempPivotingRule = argv[i+1];break;     
                case 'M':
                case 'm': tempSolverMemory = argv[i+1];break;
                case 'D':
                case 'd': tempAnnealingDecay = argv[i+1]; break;
                case 'L':
                case 'l': tempAnnealingLimit = argv[i+1]; break;
                case 'r':
                case 'R': tempPertubationValue = argv[i+1]; break;
                case 's':
                case 'S': tempseed = argv[i+1];seedIsGiven = true; break;   
                default: correctInput = 0;
                }
        }   
        // if a command line argument is not recognized, set correct input flag to false
        else correctInput = false;
    };

    /*****************************************************************************/
    /* input validation
    /*****************************************************************************/
    // if correctInput is still true, try to convert inputs to correct values
    if(correctInput){

        // check if given file exists, if not, give error and stop program
        if(!fs::is_regular_file(tempDatafile)) {
            errorString =  "the provided file " + tempDatafile + " could not be found\n";
            errorString += "program terminated";
            generateError(errorString);
        }

        // check if init solution is a valid choice, 
        // if yes set config variable, if no set correctinput flag to false
        if(tempInitSolution.compare("RANDOM")==0) initSolution = RANDOM;
        else if (tempInitSolution.compare("SRZH")==0) initSolution = SRZH;
        else correctInput = false;

        // check if neighbourhood is a valid choice, 
        // if yes set config variable, if no set correctinput flag to false
        if(tempNeighbourhood.compare("TRANSPOSE")==0) neighbourhood = TRANSPOSE;
        else if (tempNeighbourhood.compare("EXCHANGE")==0) neighbourhood = EXCHANGE;
        else if (tempNeighbourhood.compare("INSERT")==0) neighbourhood = INSERT;
        else if (tempNeighbourhood.compare("TEI")==0) neighbourhood = TEI;
        else if (tempNeighbourhood.compare("TIE")==0) neighbourhood = TIE;
        else correctInput = false;

        // check if pivotingrule is a valid choice, 
        // if yes set config variable, if no set correctinput flag to false
        if(tempPivotingRule.compare("FIRST")==0) pivotRule = FIRST;
        else if (tempPivotingRule.compare("BEST")==0) pivotRule = BEST;
        else if (tempPivotingRule.compare("ANNEAL")==0) pivotRule = ANNEAL;
        else correctInput = false;

        // check if solvermemory is a valid choice, 
        // if yes set config variable, if no set correctinput flag to false
        if(tempSolverMemory.compare("NONE")==0) solverMemory = NONE;
        else if (tempSolverMemory.compare("HASH")==0) solverMemory = HASH;
        else if (tempSolverMemory.compare("MAP")==0) solverMemory = MAP;
        else correctInput = false;
        
        //check if tempAnnealingDecay is a float between 0 and 1
        // if yes set config variable, if no set correctinput flag to false
        if(isFloatNumber(tempAnnealingDecay)) annealingDecay = std::stof(tempAnnealingDecay);
        else correctInput =0;
        if(annealingDecay <=0 || annealingDecay >1) correctInput =0;

        //check if tempAnnealingLimit is an integer
        // if yes set config variable, if no set correctinput flag to false
        if(isStringNumber(tempAnnealingLimit)) annealingLimit = std::stoi(tempAnnealingLimit);
        else correctInput =0;

        //check if tempPertubationValue is an integer
        // if yes set config variable, if no set correctinput flag to false
        if(isStringNumber(tempPertubationValue)) pertubationValue = std::stoi(tempPertubationValue);
        else correctInput =0;

        // if seed is given, check if seed is number, if yes, convert and set config variable, 
        // if no set correctinput flag to false
        // if seed is not given, choose random seed
        if(seedIsGiven){
            if(isStringNumber(tempseed)) seed = std::stoi(tempseed);
            else correctInput =0;
        }
        else {std::random_device rd; seed = rd();}
    }

    /*****************************************************************************/
    /* deal with errors or create configuration object
    /*****************************************************************************/
    //if correct input flag is false, give error message and stop program
    if(!correctInput){
        errorString =  "unkown command, please consult readme file for details.\n";
        errorString += "program terminated";
        generateError(errorString);
    }

    //if data file is not given, give error message and stop program
    if(tempDatafile.compare("unknown")== 0){
        errorString =  "command should at least provide data filepath,\n";
        errorString += "please consult readme file for details.\n";
        errorString += "program terminated";
        generateError(errorString);
    }

    Configuration configuration(
        tempDatafile, 
        initSolution,
        neighbourhood,
        pivotRule,
        solverMemory,
        annealingDecay,
        annealingLimit,
        pertubationValue,
        true,
        seed);
    return configuration;
}
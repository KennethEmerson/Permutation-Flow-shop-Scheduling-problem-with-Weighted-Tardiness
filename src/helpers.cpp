/********************************************************************************/
/* PERMUTATION FLOW-SHOP SCHEDULING PROBLEM                                                                             
/* This file contains helper functions used throughout the codebase
/********************************************************************************/
#include <vector>
#include <iostream>
#include <string>

#include "helpers.h"

using std::endl, std::vector, std::string;


/**********************************************************************************/
/**
 * converts a vector to a string
 * @param vect pointer to the vector to be transformed
 * @returns a string with all vecotr values as substrings 
 */
/**********************************************************************************/
string vectorToString(vector<int> &vect){
    string str;
    for (int i: vect){
        str += std::to_string(i);
        str += " ";
    }
    return str;
};


/**********************************************************************************/
/**
 *  prints a vector containing floats to a terminal
 *  @param vect pointer to the vector to be printed
 */
/**********************************************************************************/
void printVector(vector<float> &vect){
    for (float i: vect)
        std::cout << i << ' ';
    std::cout << endl;
};


/**********************************************************************************/
/**
 *  prints a vector containing booleans to a terminal
 *  @param vect pointer to the vector to be printed
 */
/**********************************************************************************/
void printVector(vector<bool> &vect){
    for (bool i: vect)
        std::cout << i << ' ';
    std::cout << endl;
};


/**********************************************************************************/
/**
 *  prints a vector containing long ints to a terminal
 *  @param vect pointer to the vector to be printed
 */
/**********************************************************************************/
void printVector(vector<long int> &vect){
    for (long int i: vect)
        std::cout << i << ' ';
    std::cout << endl;
};


/**********************************************************************************/
/**
 *  prints a vector containing integers to a terminal
 *  @param vect pointer to the vector to be printed
 */
/**********************************************************************************/
void printVector(vector<int> &vect){
    for (int i: vect)
        std::cout << i << ' ';
    std::cout << endl;
};


/**********************************************************************************/
/**
 *  prints a vector matrix containing long ints to a terminal
 *  @param vect pointer to the vector matrix to be printed
 */
/**********************************************************************************/
void printVectorMatrix(vector<vector< long int>> &matrix){
    for (vector<long int> row: matrix){
        for (long int j: row)
            std::cout << j << ' ';
        std::cout << endl;
    }
}


/**********************************************************************************/
/**
 *  prints a vector matrix to a terminal
 *  @param vect pointer to the vector matrix to be printed
 */
/**********************************************************************************/
void printVectorMatrix(vector<vector<int>> &matrix){
    for (vector<int> row: matrix){
        for (int j: row)
            std::cout << j << ' ';
        std::cout << endl;
    }
}


/**********************************************************************************/
/**
 *  prints a horizontal line to the terminal
 */
/**********************************************************************************/
void printHorizontalLine(){
    std::cout << std::string(80, '-') << endl;
}


/**********************************************************************************/
/**
 *  prints a title to the terminal
 * @param title the title to be printed
 */
/**********************************************************************************/
void printTitle(std::string title){
    std::cout << endl;
    printHorizontalLine();
    std::cout << title << endl;
    printHorizontalLine();
}

/**********************************************************************************/
/**
 *  prints an error to the terminal and terminates the program
 * @param message the error message to be printed
 */
/**********************************************************************************/
void generateError(string message){
    printHorizontalLine();
    std::cout << message << endl;
    printHorizontalLine();
    exit(0);
};


/**********************************************************************************/
/**
 * helperfunction to test if a string represents a positive unsigned float
 * inspired by : https://stackoverflow.com/questions/447206/c-isfloat-function
 * @param input the input string to test
 * @returns True if string represents an float, false otherwise
 */
/**********************************************************************************/
bool isFloatNumber(const std::string& string){
    std::string::const_iterator it = string.begin();
    bool decimalPoint = false;
    int minSize = 0;
    
    while(it != string.end()){
      if(*it == '.'){
        if(!decimalPoint) decimalPoint = true;
        else break;
      }else if(!std::isdigit(*it) && (it+1 != string.end() || !decimalPoint)){
        break;
      }
      ++it;
    }
    return string.size()>minSize && it == string.end();
  }


/**********************************************************************************/
/**
 * helperfunction to test if a string represents an integer,
 * based on https://www.delftstack.com/howto/cpp/how-to-determine-if-a-string-is-number-cpp/
 * @param input the input string to test
 * @returns True if string represents an integer, false otherwise
 */
/**********************************************************************************/
bool isStringNumber(string input){
    int isNumber = true;
    for (char const &c : input) {
        if (std::isdigit(c) == 0) isNumber = false;; 
    }
    return isNumber;        
}

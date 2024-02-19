/********************************************************************************/
/* PERMUTATION FLOW-SHOP SCHEDULING PROBLEM                                                                              
/* headerfile for helpers.cpp
/********************************************************************************/
#ifndef _HELPERS_H_
#define _HELPERS_H_

    #include <vector>
    #include <string>

    using std::vector, std::string;

    string vectorToString(vector<int> &vect);
    void printVector(vector<bool> &vect);
    void printVector(vector<float> &vect);
    void printVector(vector<long int> &vect);
    void printVectorMatrix(vector<vector<long int>> &matrix);
    void printVector(vector<int> &vect);
    void printVectorMatrix(vector<vector<int>> &matrix);
    void printHorizontalLine();
    void printTitle(string title);
    void generateError(string message);
    bool isFloatNumber(const std::string& string);
    bool isStringNumber(string input);

#endif
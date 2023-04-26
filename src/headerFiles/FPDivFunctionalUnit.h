//
// Created by Anushri Hatti on 4/25/23.
//

#ifndef TOMASULOSIMULATOR_FPDIVFUNCTIONALUNIT_H
#define TOMASULOSIMULATOR_FPDIVFUNCTIONALUNIT_H

#include <map>
#include <string>
#include <vector>
#include <tuple>

using namespace std;

class FPDivFunctionalUnit {
public:
    int latency;
    map<int, float> results;
    map<int, tuple<int, float, float, string, int>> executing_operations;


    FPDivFunctionalUnit(int latency);
    void scheduleExecution(int instruction_id, float operand1, float operand2, string operation_type);
    vector<int> stallOrExecute();
    float execute(tuple<int, float, float, string, int> instruction_to_execute);
};


#endif //TOMASULOSIMULATOR_FPDIVFUNCTIONALUNIT_H

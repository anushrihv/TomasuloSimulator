//
// Created by Anushri Hatti on 4/25/23.
//

#ifndef TOMASULOSIMULATOR_FPMULFUNCTIONALUNIT_H
#define TOMASULOSIMULATOR_FPMULFUNCTIONALUNIT_H

#include <map>
#include <string>
#include <vector>
#include <tuple>

using namespace std;

class FPMulFunctionalUnit {
public:
    int latency;
    map<int, float> results;
    map<int, tuple<int, float, float, string, int>> executing_operations;


    FPMulFunctionalUnit(int latency);
    void scheduleExecution(int instruction_id, float operand1, float operand2, string operation_type);
    vector<int> stallOrExecute();
    float execute(tuple<int, float, float, string, int> instruction_to_execute);
    int getResult(int instructionID);
};


#endif //TOMASULOSIMULATOR_FPMULFUNCTIONALUNIT_H

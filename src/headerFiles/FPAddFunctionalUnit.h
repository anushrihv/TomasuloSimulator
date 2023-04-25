//
// Created by Anushri Hatti on 4/25/23.
//

#ifndef TOMASULOSIMULATOR_FPADDFUNCTIONALUNIT_H
#define TOMASULOSIMULATOR_FPADDFUNCTIONALUNIT_H

#include <map>
#include <string>
#include <vector>
#include <tuple>

using namespace std;

/*
 * tuple: instruction_id, operand1, operand2, operation_type, cycles_remaining
 */
class FPAddFunctionalUnit {
public:
    int latency;
    map<int, float> results;
    map<int, tuple<int, float, float, string, int>> executing_operations;


    FPAddFunctionalUnit(int latency);
    void scheduleExecution(int instruction_id, float operand1, float operand2, string operation_type);
    vector<int> stallOrExecute();
    float execute(tuple<int, float, float, string, int> instruction_to_execute);
};


#endif //TOMASULOSIMULATOR_FPADDFUNCTIONALUNIT_H

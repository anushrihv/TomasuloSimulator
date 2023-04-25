//
// Created by Anushri Hatti on 4/25/23.
//

#ifndef TOMASULOSIMULATOR_INTFUNCTIONALUNIT_H
#define TOMASULOSIMULATOR_INTFUNCTIONALUNIT_H

#include <map>
#include <string>
#include <vector>
#include <tuple>

using namespace std;

/*
 * tuple: instruction_id, operand1, operand2, operation_type, cycles_remaining
 */
class IntFunctionalUnit {
public:
    int latency;
    map<int, int> results;
    map<int, tuple<int, int, int, string, int>> executing_operations;


    IntFunctionalUnit(int latency);
    void scheduleExecution(int instruction_id, int operand1, int operand2, string operation_type);
    void stallOrExecute();
    int execute(tuple<int, int, int, string, int> instruction_to_execute);
};

#endif //TOMASULOSIMULATOR_INTFUNCTIONALUNIT_H

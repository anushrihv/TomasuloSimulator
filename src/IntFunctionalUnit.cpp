//
// Created by Anushri Hatti on 4/25/23.
//

#include "headerFiles/IntFunctionalUnit.h"
#include <string>
#include <tuple>
#include <iostream>

using namespace std;

IntFunctionalUnit::IntFunctionalUnit(int latency) {
    this->latency = latency;
}

void IntFunctionalUnit::scheduleExecution(int instruction_id, int operand1, int operand2, string operation_type) {
    // add to executing instructions
    tuple<int, int, int, string, int> instruction_to_execute = make_tuple(instruction_id, operand1, operand2,
                                                                          operation_type, this->latency);
    this->executing_operations[instruction_id] = instruction_to_execute;
}

vector<int> IntFunctionalUnit::stallOrExecute() {
    vector<int> instructions_executed;
    for (const auto& pair : executing_operations) {
        int instruction_id = pair.first;
        tuple<int, int, int, string, int> instruction = pair.second;
        int cyclesRemaining = get<4>(instruction);
        cyclesRemaining--;

        if (cyclesRemaining == 0) {
            cout << "EXECUTE STAGE for instruction " + to_string(instruction_id) << std::endl;
            int result = execute(instruction);

            // write result to buffer
            this->results[instruction_id] = result;

            // remove from list of executing instructions
            instructions_executed.push_back(instruction_id);
        } else {
            executing_operations[instruction_id] = make_tuple(instruction_id, get<1>(instruction), get<2>(instruction),
                                                              get<3>(instruction), cyclesRemaining);
        }
    }

    for(int instructionID : instructions_executed) {
        executing_operations.erase(instructionID);
    }

    return instructions_executed;
}

int IntFunctionalUnit::execute(tuple<int, int, int, string, int> instruction_to_execute) {
    int operand1 = get<1>(instruction_to_execute);
    int operand2 = get<2>(instruction_to_execute);
    return operand1 + operand2;
}

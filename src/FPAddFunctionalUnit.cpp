//
// Created by Anushri Hatti on 4/25/23.
//

#include "headerFiles/FPAddFunctionalUnit.h"
#include <iostream>

FPAddFunctionalUnit::FPAddFunctionalUnit(int latency) {
    this->latency = latency;
}

void FPAddFunctionalUnit::scheduleExecution(int instruction_id, float operand1, float operand2, std::string operation_type) {
    // add to executing instructions
    tuple<int, float, float, string, int> instruction_to_execute = make_tuple(instruction_id, operand1, operand2,
                                                                          operation_type, this->latency);
    this->executing_operations[instruction_id] = instruction_to_execute;
}

vector<int> FPAddFunctionalUnit::stallOrExecute() {
    vector<int> instructions_executed;
    for (auto& pair : executing_operations) {
        int instruction_id = pair.first;
        tuple<int, float, float, string, int>& instruction = pair.second;
        int cyclesRemaining = std::get<4>(instruction);
        cyclesRemaining--;

        if (cyclesRemaining == 0) {
            cout << "EXECUTE STAGE for instruction " + to_string(instruction_id) << std::endl;
            float result = execute(instruction);

            // write result to buffer
            this->results[instruction_id] = result;

            // remove from list of executing instructions
            instructions_executed.push_back(instruction_id);
        } else {
            //std::get<0>(instruction) = cyclesRemaining;
            executing_operations[instruction_id] = make_tuple(instruction_id, get<1>(instruction), get<2>(instruction),
                                                              get<3>(instruction), cyclesRemaining);
        }
    }

    for(int instructionID : instructions_executed) {
        executing_operations.erase(instructionID);
    }

    return instructions_executed;
}

float FPAddFunctionalUnit::execute(tuple<int, float, float, string, int> instruction_to_execute) {
    float operand1 = get<1>(instruction_to_execute);
    float operand2 = get<2>(instruction_to_execute);
    string op = get<3>(instruction_to_execute);
    if (op == "fadd") {
        return operand1 + operand2;
    } else {
        return operand1 - operand2;
    }
}

int FPAddFunctionalUnit::getResult(int instructionID) {
    return results[instructionID];
}

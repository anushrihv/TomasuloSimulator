//
// Created by Anushri Hatti on 4/21/23.
//
#include <vector>
#include <string>
#include <iostream>
#include "headerFiles/instruction_queue.h"

using namespace std;


void InstructionQueue::addNewInstruction(string instruction) {
    int size = instructions.size();
    instruction = to_string(size) + " " + instruction;
    instructions.push_back(instruction);
}

string InstructionQueue::getNextInstruction() {
    if (pc < instructions.size()) {
        string instruction_to_send = instructions[pc];
        pc++;
        return instruction_to_send;
    } else {
        cout << "No new instructions to fetch from the instruction queue";
        return "";
    }
}

bool InstructionQueue::hasNextInstruction() {
    return pc < instructions.size();
}

string InstructionQueue::peekNextInstruction() {
    if (hasNextInstruction()) {
        return instructions[pc];
    } else {
        return "";
    }
}

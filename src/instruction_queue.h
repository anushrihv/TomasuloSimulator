//
// Created by Anushri Hatti on 4/21/23.
//

#ifndef UNTITLED_INSTRUCTION_QUEUE_H
#define UNTITLED_INSTRUCTION_QUEUE_H

#include <vector>
#include <string>
#include <iostream>

using namespace std;

class InstructionQueue {
private:
    vector<string> instructions;
    int pc;
public:
    void addNewInstruction(string instruction);

    string getNextInstruction();

    bool hasNextInstruction();

    string peekNextInstruction();
};

#endif //UNTITLED_INSTRUCTION_QUEUE_H

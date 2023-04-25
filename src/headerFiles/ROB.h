//
// Created by Anushri Hatti on 4/22/23.
//

#ifndef UNTITLED_ROB_H
#define UNTITLED_ROB_H

#include <array>
#include "ROBEntry.h"

using namespace std;

class ROB {
private:
    int size;
    int start;
    int end;
    ROBEntry* rob_entries;
public:
    ROB(int size);
    bool isFull();
    int addROBEntry(ROBEntry robEntry);
    void updateROBEntry(ROBEntry robEntry);
    int getROBIDbyInstructionID(int instructionID);
};


#endif //UNTITLED_ROB_H

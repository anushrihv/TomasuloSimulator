//
// Created by Anushri Hatti on 4/22/23.
//

#include "headerFiles/ROB.h"
#include <iostream>

ROB::ROB(int size) {
    start = 0;
    end = 0;
    this->rob_entries = new ROBEntry[size];
    for (int i = 0; i < size; i++) {
        this->rob_entries[i] = ROBEntry();
    }
}

bool ROB::isFull() {
    return ((end + 1) % this->size) == start;
}

bool ROB::isEmpty() {
    return start == end;
}

int ROB::addROBEntry(ROBEntry robEntry) {
    int robEntryID;
    if (isFull()) {
        cout << "ROB is full. Cannot add an entry";
        return -1;
    }
    rob_entries[end] = robEntry;
    robEntryID = end;
    end = (end + 1) % size;
    return robEntryID;
}

void ROB::updateROBEntry(const ROBEntry& robEntry) {
    for (int i = 0; i < size; i++) {
        if(rob_entries[i].instruction_id == robEntry.instruction_id) {
            rob_entries[i] = robEntry;
            cout << "Updated ROB Entry for instruction ID " + to_string(robEntry.instruction_id);
            return;
        }
    }
}

ROBEntry* ROB::getROBEntrybyInstructionID(int instructionID) {
    for (int i = 0; i < size; i++) {
        if(rob_entries[i].instruction_id == instructionID) {
            return &rob_entries[i];
        }
    }
    ROBEntry* robEntry = new ROBEntry();
    return robEntry;
}

int ROB::getROBIDbyInstructionID(int instructionID) {
    for (int i = 0; i < size; i++) {
        if(rob_entries[i].instruction_id == instructionID) {
            return i;
        }
    }

    return -1;
}

//
// Created by Anushri Hatti on 4/22/23.
//

#include "ROBEntry.h"

ROBEntry::ROBEntry() {
    init();
}

ROBEntry::ROBEntry(int instruction_id, string destination_register, float value, bool done) {
    this->instruction_id = instruction_id;
    this->destination_register = destination_register;
    this->done = done;
    this->value = value;
}

void ROBEntry::init() {
    this->instruction_id = -1;
    this->destination_register="";
    this->done= false;
    this->value=-1;
}

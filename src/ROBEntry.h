//
// Created by Anushri Hatti on 4/22/23.
//

#ifndef UNTITLED_ROBENTRY_H
#define UNTITLED_ROBENTRY_H

#include <string>

using namespace std;

class ROBEntry {
public:
    int instruction_id;
    string destination_register;
    float value;
    bool done;

    void init();
    ROBEntry();
    ROBEntry(int instruction_id, string destination_register, float value, bool done);
};


#endif //UNTITLED_ROBENTRY_H

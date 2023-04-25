//
// Created by Anushri Hatti on 4/22/23.
//

#ifndef UNTITLED_REGISTERFILE_H
#define UNTITLED_REGISTERFILE_H

#include <map>

using namespace std;

class RegisterFile {
public:
    map<string, int> intRegisters;
    map<string, float> floatRegisters;

    RegisterFile();
    float getRegisterValue(string register_name);
};


#endif //UNTITLED_REGISTERFILE_H

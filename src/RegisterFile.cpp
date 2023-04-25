//
// Created by Anushri Hatti on 4/22/23.
//

#include "headerFiles/RegisterFile.h"
#include <string>

RegisterFile::RegisterFile() {
    for(int i=0; i<32; i++) {
        string int_register_name = "R" + to_string(i);
        intRegisters[int_register_name] = 0;
        string float_register_name = "F" + to_string(i);
        floatRegisters[float_register_name] = 0.0;
    }
}

float RegisterFile::getRegisterValue(std::string register_name) {
    char first_char = register_name.front();

    if(first_char == 'R') {
        return intRegisters[register_name];
    } else {
        return floatRegisters[register_name];
    }
}

//
// Created by Anushri Hatti on 4/22/23.
//

#include "RegisterFile.h"
#include <string>

RegisterFile::RegisterFile() {
    for(int i=0; i<32; i++) {
        string register_name = "R" + to_string(i);
        registers[register_name] = 0;
    }
}

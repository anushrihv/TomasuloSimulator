//
// Created by Anushri Hatti on 4/22/23.
//

#include "headerFiles/RAT.h"

RAT::RAT() {
    for(int i=0; i<32; i++) {
        string intRegisterName = "R" + to_string(i);
        string floatRegisterName = "F" + to_string(i);
        string physicalRegisterName = "P" + to_string(i);
        mappings[intRegisterName] = intRegisterName;
        mappings[floatRegisterName] = floatRegisterName;

        freeList.push(physicalRegisterName);
    }
}

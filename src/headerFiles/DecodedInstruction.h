//
// Created by Anushri Hatti on 4/22/23.
//

#ifndef UNTITLED_DECODEDINSTRUCTION_H
#define UNTITLED_DECODEDINSTRUCTION_H

#include <string>

using namespace std;

class DecodedInstruction {
public:
    int instruction_id;
    string instruction_type;
    string destination;
    string source1;
    string source2;

    DecodedInstruction(string instruction);
};


#endif //UNTITLED_DECODEDINSTRUCTION_H

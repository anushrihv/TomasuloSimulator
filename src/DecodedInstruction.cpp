//
// Created by Anushri Hatti on 4/22/23.
//

#include "headerFiles/DecodedInstruction.h"
#include <string>
#include <sstream>
#include <vector>

DecodedInstruction::DecodedInstruction(std::string instruction) {
    istringstream stringStream(instruction);
    string word;
    vector<string> output;

    while (getline(stringStream, word, ' ')) {
        // First split by space
        istringstream substream(word);
        string subWord;
        while (getline(substream, subWord, ',')) {
            // Then split by comma
            output.push_back(subWord);
        }
    }

    this->instruction_id = stoi(output[0]);
    this->instruction_type = output[1];
    this->destination = output[2];

    if (instruction_type == "fsd" ||  instruction_type == "fld") {
        string input = output[3];
        string num;
        string str;
        size_t start = input.find_first_of("0123456789");
        size_t end = input.find('(');
        num = input.substr(start, end-start);
        str = input.substr(end+1, input.length()-end-2);

        this->source1 = num;
        this->source2 = str;
    } else {
        this->source1 = output[3];
        this->source2 = output[4];
    }
}

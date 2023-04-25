//
// Created by Anushri Hatti on 4/22/23.
//

#ifndef UNTITLED_RESERVATIONSTATIONENTRY_H
#define UNTITLED_RESERVATIONSTATIONENTRY_H

#include <string>

using namespace std;

class ReservationStationEntry {
public:
    bool busy;
    int instruction_id;
    string op;
    string vj;
    string vk;
    string qj;
    string qk;
    string destination;

    ReservationStationEntry();

    ReservationStationEntry(int instruction_id, string op, string vj, string vk, string qj, string qk,
                            string destination, bool busy);
};


#endif //UNTITLED_RESERVATIONSTATIONENTRY_H

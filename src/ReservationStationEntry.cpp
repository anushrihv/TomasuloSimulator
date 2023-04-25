//
// Created by Anushri Hatti on 4/22/23.
//

#include "ReservationStationEntry.h"
#include <string>

ReservationStationEntry::ReservationStationEntry() {
    this->instruction_id=-1;
}

ReservationStationEntry::ReservationStationEntry(int instruction_id, string op, string vj, string vk,
                                                 string qj, string qk, string destination, bool busy) {
    this->instruction_id = instruction_id;
    this->op = op;
    this->vj = vj;
    this->vk = vk;
    this->qj = qj;
    this->qk = qk;
    this->destination = destination;
    this->busy = busy;
}

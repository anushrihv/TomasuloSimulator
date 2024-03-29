//
// Created by Anushri Hatti on 4/22/23.
//

#ifndef UNTITLED_RESERVATIONSTATION_H
#define UNTITLED_RESERVATIONSTATION_H

#include <vector>
#include "ReservationStationEntry.h"

using namespace std;

class ReservationStation {
public:
    int max_size;
    vector<ReservationStationEntry> reservation_station_entries;

    bool isFull();
    ReservationStation(int size);
    void addReservationStationEntry(const ReservationStationEntry& reservationStationEntry);
    vector<ReservationStationEntry*> getReadyReservationStationEntries();
    void updateRESEntriesWaitingOnROB(const string& rob, float value);
    void removeRESEntry(int instructionID);
};


#endif //UNTITLED_RESERVATIONSTATION_H

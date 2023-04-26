//
// Created by Anushri Hatti on 4/22/23.
//

#include "headerFiles/ReservationStation.h"
#include "iostream"

using namespace std;

ReservationStation::ReservationStation(int size) {
    this->max_size = size;
}

bool ReservationStation::isFull() {
    return this->reservation_station_entries.size() >= this->max_size;
}

void ReservationStation::addReservationStationEntry(const ReservationStationEntry& reservationStationEntry) {
    if (!isFull()) {
        reservation_station_entries.push_back(reservationStationEntry);
    } else {
        cout << "Reservation station is full. Cannot add entry";
    }
}

vector<ReservationStationEntry*> ReservationStation::getReadyReservationStationEntries() {
    vector<ReservationStationEntry*> res;

    for (ReservationStationEntry& reservationStationEntry : this->reservation_station_entries) {
        if(reservationStationEntry.qj == "" && reservationStationEntry.qk == "" && !reservationStationEntry.executing){
            res.push_back(&reservationStationEntry);
        }
    }

    return res;
}

void ReservationStation::updateRESEntriesWaitingOnROB(const string& rob, float value) {
    for (ReservationStationEntry& reservationStationEntry : this->reservation_station_entries) {
        if(reservationStationEntry.qj == rob){
            reservationStationEntry.qj = "";
            reservationStationEntry.vj = value;
        } else if (reservationStationEntry.qk == rob) {
            reservationStationEntry.qk = "";
            reservationStationEntry.vk = value;
        }
    }
}

void ReservationStation::removeRESEntry(int instructionID) {
    int index = -1;

    for (int i = 0; i < reservation_station_entries.size(); i++) {
        ReservationStationEntry& reservationStationEntry = reservation_station_entries[i];
        if(reservationStationEntry.instruction_id == instructionID) {
            index = i;
            break;
        }
    }

    if(index >= 0){
        reservation_station_entries.erase(reservation_station_entries.begin() + index);
    }

}


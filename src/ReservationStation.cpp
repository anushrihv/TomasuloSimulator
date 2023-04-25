//
// Created by Anushri Hatti on 4/22/23.
//

#include "ReservationStation.h"
#include "iostream"

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

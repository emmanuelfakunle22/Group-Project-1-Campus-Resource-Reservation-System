#ifndef CANCELLATIONHISTORY_H
#define CANCELLATIONHISTORY_H

#include "DataStructures.h"
#include "Reservation.h"

// Tracks cancelled reservations on a stack so that only the MOST
// RECENTLY cancelled reservation can be restored (LIFO / undo).
class CancellationHistory {
private:
    Stack<Reservation> history;

public:
    // Record a newly cancelled reservation. O(1).
    void recordCancellation(const Reservation& reservation) {
        history.push(reservation);
    }

    // Restore (pop) the most recently cancelled reservation.
    // Returns true and fills 'restored' if history was non-empty. O(1).
    bool undoLastCancellation(Reservation& restored) {
        return history.pop(restored);
    }

    bool isEmpty() const { return history.isEmpty(); }
    int size() const { return history.size(); }

    void display() const {
        history.display();
    }
};

#endif // CANCELLATIONHISTORY_H

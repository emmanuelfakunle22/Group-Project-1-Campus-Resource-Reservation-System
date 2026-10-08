#ifndef RESERVATIONMANAGER_H
#define RESERVATIONMANAGER_H

#include <string>
#include <vector>
#include "DataStructures.h"
#include "Resource.h"
#include "Reservation.h"
#include "WaitingList.h"
#include "CancellationHistory.h"

// Result codes returned by createReservation() so the UI layer (main.cpp)
// can print an appropriate, specific message.
enum class CreateResult {
    SUCCESS,
    INVALID_RESOURCE,
    ADDED_TO_WAITLIST,
    INVALID_DATE,
    DUPLICATE_REQUEST
};

// Central class that owns and coordinates all of the system's data:
//   - resources          : std::vector<Resource>        (resource inventory)
//   - activeReservations : LinkedList<Reservation>       (linked list)
//   - waitingList         : WaitingList (queue per resource)
//   - cancellationHistory : CancellationHistory (stack)
class ReservationManager {
private:
    std::vector<Resource> resources;
    LinkedList<Reservation> activeReservations;
    WaitingList waitingList;
    CancellationHistory cancellationHistory;
    int nextReservationID;

public:
    ReservationManager();

    // ---------------- Resource Management ----------------
    bool loadResources(const std::string& filename);
    void displayResources() const;
    void displayResourceAvailability() const;
    bool resourceExists(const std::string& resourceID) const;
    Resource* findResource(const std::string& resourceID);

    // ---------------- Reservation Management ----------------
    // Optional preload of sample/historical reservation records.
    bool loadReservations(const std::string& filename);

    // Creates a reservation for the given student on the given resource.
    // Auto-assigns the reservation ID. If the resource is unavailable,
    // the student is placed on that resource's waiting list instead.
    //
    // Validation performed before any reservation/waitlist entry is made:
    //   - resource ID must exist                  -> INVALID_RESOURCE
    //   - date must be a valid MM/DD/YYYY date     -> INVALID_DATE
    //   - the same student must not already have
    //     an active reservation for this resource  -> DUPLICATE_REQUEST
    CreateResult createReservation(int studentID, const std::string& studentName,
                                    const std::string& resourceID, const std::string& date);

    // Returns true if 'date' is a real calendar date in MM/DD/YYYY format
    // (checks separators, numeric fields, month 1-12, and day-of-month
    // range including leap years).
    static bool isValidDate(const std::string& date);

    // Returns true if 'studentID' already has an active reservation for
    // 'resourceID'. O(n) scan of active reservations.
    bool hasDuplicateReservation(int studentID, const std::string& resourceID) const;

    // Cancels an existing reservation by ID. Returns false if not found.
    // On success: frees the resource, records the cancellation on the
    // history stack, and automatically assigns the resource to the next
    // waiting student (if any) via the FIFO waiting queue.
    bool cancelReservation(int reservationID);

    bool reservationExists(int reservationID) const;
    void displayActiveReservations() const;

    // ---------------- Searching (linear search) ----------------
    void searchResourceByID(const std::string& resourceID) const;
    void searchReservationByID(int reservationID) const;
    void searchReservationsByStudent(int studentID) const;

    // ---------------- Waiting List Management ----------------
    void displayWaitingLists() const;

    // ---------------- Cancellation History (Undo) ----------------
    bool undoCancellation();
    void displayCancellationHistory() const;
};

#endif // RESERVATIONMANAGER_H
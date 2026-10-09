#ifndef RESERVATIONMANAGER_H
#define RESERVATIONMANAGER_H
#include <string>
#include <vector>
#include <map>
#include "DataStructures.h"
#include "Resource.h"
#include "Reservation.h"
#include "Student.h"
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
//   - resources             : std::vector<Resource>        (resource inventory)
//   - activeReservations    : LinkedList<Reservation>       (linked list)
//   - waitingList           : WaitingList (queue per resource)
//   - cancellationHistory   : CancellationHistory (stack)
//   - reservationFrequency  : how many times each resource has ever been
//                              reserved, used for the "most frequently
//                              reserved resources" report
class ReservationManager {
private:
    std::vector<Resource> resources;
    LinkedList<Reservation> activeReservations;
    WaitingList waitingList;
    CancellationHistory cancellationHistory;
    std::map<std::string, int> reservationFrequency;
    int nextReservationID;

public:
    ReservationManager();

    // ---------------- Resource Management ----------------
    bool loadResources(const std::string& filename);
    void displayResources() const;
    void displayResourceAvailability() const;
    Resource* findResource(const std::string& resourceID);

    // Linear search for a single resource by ID; prints the result
    // (or a "not found" message) directly.
    void searchResourceByID(const std::string& resourceID) const;

    // Sorts the resource inventory in place using merge sort.
    void sortResourcesByName();
    void sortResourcesByType();

    // ---------------- Reservation Management ----------------
    // Optional preload of sample/historical reservation records.
    // Also reconciles resource availability against what was loaded:
    // any resource with at least one active reservation is forced
    // Unavailable, and any resource with more than one active
    // reservation loaded against it is reported as a data conflict.
    bool loadReservations(const std::string& filename);

    // Creates a reservation for the given student on the given resource.
    // Auto-assigns the reservation ID. If the resource is unavailable,
    // the student is placed on that resource's waiting list instead
    // (along with the date they requested).
    //
    // Validation performed before any reservation/waitlist entry is made:
    //   - resource ID must exist                  -> INVALID_RESOURCE
    //   - date must be a valid MM/DD/YYYY date     -> INVALID_DATE
    //   - the same student must not already have
    //     an active reservation for this resource  -> DUPLICATE_REQUEST
    CreateResult createReservation(const Student& student,
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
    // waiting student (if any) via the FIFO waiting queue, using THAT
    // student's own requested date (not the cancelled reservation's).
    bool cancelReservation(int reservationID);

    bool reservationExists(int reservationID) const;
    void displayActiveReservations() const;

    // Linear search helpers exposed to the menu.
    void searchReservationByID(int reservationID) const;
    void searchReservationsByStudent(int studentID) const;

    // ---------------- Waiting List Management ----------------
    void displayWaitingLists() const;

    // ---------------- Cancellation History (Undo) ----------------
    bool undoCancellation();
    void displayCancellationHistory() const;

    // ---------------- Sorting ----------------
    // Which field to sort reservations by.
    enum class ReservationSortKey { RESERVATION_ID, STUDENT_NAME, RESOURCE_ID, DATE };

    // Merge-sorts a COPY of the active reservations and prints it (the
    // linked list itself keeps its insertion order).
    void sortAndDisplayReservations(ReservationSortKey key) const;

    // ---------------- Read-only accessors (used by ReportGenerator) ----------------
    const std::vector<Resource>& getResources() const { return resources; }
    const WaitingList& getWaitingList() const { return waitingList; }
    const std::map<std::string, int>& getReservationFrequency() const { return reservationFrequency; }
    int getActiveReservationCount() const { return activeReservations.size(); }

    // Copies the active reservations from the linked list into a vector
    // so they can be sorted without touching the list.
    std::vector<Reservation> getActiveReservationsSnapshot() const;
};

#endif // RESERVATIONMANAGER_H

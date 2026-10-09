#include "ReservationManager.h"
#include "Algorithms.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <cctype>

ReservationManager::ReservationManager() : nextReservationID(301) {}

// ---------------------------------------------------------------
// Resource Management
// ---------------------------------------------------------------

// Loads resources from a pipe-delimited file:
//   ResourceID|ResourceName|ResourceType|Available/Unavailable
bool ReservationManager::loadResources(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cout << "ERROR: Could not open resource file: " << filename << "\n";
        return false;
    }

    resources.clear();
    std::string line;
    int lineNumber = 0;

    while (std::getline(file, line)) {
        lineNumber++;
        if (line.empty()) continue;

        std::stringstream ss(line);
        std::string id, name, type, statusStr;

        if (!std::getline(ss, id, '|') ||
            !std::getline(ss, name, '|') ||
            !std::getline(ss, type, '|') ||
            !std::getline(ss, statusStr, '|')) {
            std::cout << "WARNING: Skipping malformed line " << lineNumber
                      << " in " << filename << "\n";
            continue;
        }

        bool available = (statusStr == "Available");
        resources.push_back(Resource(id, name, type, available));
    }

    file.close();
    std::cout << "Loaded " << resources.size() << " resources from " << filename << ".\n";
    return true;
}

void ReservationManager::displayResources() const {
    if (resources.empty()) {
        std::cout << "  No resources loaded.\n";
        return;
    }
    std::cout << std::left
              << std::setw(8)  << "ID"
              << std::setw(20) << "Name"
              << std::setw(22) << "Type"
              << "Status" << "\n";
    std::cout << std::string(60, '-') << "\n";
    for (const auto& r : resources) {
        r.display();
    }
}

void ReservationManager::displayResourceAvailability() const {
    if (resources.empty()) {
        std::cout << "  No resources loaded.\n";
        return;
    }
    int availableCount = 0;
    for (const auto& r : resources) {
        if (r.isAvailable()) availableCount++;
    }
    std::cout << "Resource Availability Summary\n";
    std::cout << std::string(35, '-') << "\n";
    std::cout << "Total resources:     " << resources.size() << "\n";
    std::cout << "Available:           " << availableCount << "\n";
    std::cout << "Unavailable:         " << (resources.size() - availableCount) << "\n\n";

    std::cout << std::left
              << std::setw(8)  << "ID"
              << std::setw(20) << "Name"
              << "Status" << "\n";
    std::cout << std::string(45, '-') << "\n";
    for (const auto& r : resources) {
        std::cout << std::left << std::setw(8) << r.getID()
                   << std::setw(20) << r.getName()
                   << (r.isAvailable() ? "Available" : "Unavailable") << "\n";
    }
}

Resource* ReservationManager::findResource(const std::string& resourceID) {
    // Linear search through the resource vector for a matching ID.
    int index = linearSearchIndex(resources, [&](const Resource& r) {
        return r.getID() == resourceID;
    });
    if (index == -1) return nullptr;
    return &resources[index];
}

// Linear search for a resource by ID (Resource Management requirement).
void ReservationManager::searchResourceByID(const std::string& resourceID) const {
    int index = linearSearchIndex(resources, [&](const Resource& r) {
        return r.getID() == resourceID;
    });
    if (index == -1) {
        std::cout << "No resource found with ID '" << resourceID << "'.\n";
        return;
    }
    resources[index].display();
}

// Sorts resources ascending by name using merge sort (O(n log n)).
void ReservationManager::sortResourcesByName() {
    mergeSort(resources, [](const Resource& a, const Resource& b) {
        return a.getName() < b.getName();
    });
    std::cout << "Resources sorted by name.\n";
}

// Sorts resources ascending by type using merge sort (O(n log n)).
void ReservationManager::sortResourcesByType() {
    mergeSort(resources, [](const Resource& a, const Resource& b) {
        return a.getType() < b.getType();
    });
    std::cout << "Resources sorted by type.\n";
}

// ---------------------------------------------------------------
// Reservation Management
// ---------------------------------------------------------------

// Loads pre-existing reservation records from a pipe-delimited file:
//   ReservationID|StudentID|StudentName|ResourceID|Date
// This is used to preload sample/historical data for testing.
//
// FIX: this now RECONCILES resource availability against what was
// actually loaded here, instead of blindly trusting resources.txt's
// Available/Unavailable flag. Previously, if resources.txt said a
// resource was "Available" while reservations.txt already had one (or
// even two) active reservations against it, the program would start up
// believing the resource was free, and happily let a new student book
// it a second or third time. Now:
//   - any resourceID that ends up with at least one active reservation
//     loaded here is forced to Unavailable.
//   - if a resourceID has MORE THAN ONE active reservation loaded
//     against it, that is a genuine data conflict (the same resource
//     can only be held by one reservation at a time), so it is reported
//     to the user instead of being silently accepted.
bool ReservationManager::loadReservations(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cout << "ERROR: Could not open reservation file: " << filename << "\n";
        return false;
    }

    std::string line;
    int lineNumber = 0;
    int loadedCount = 0;
    int highestID = nextReservationID - 1;

    // Counts how many active reservations were loaded per resource ID,
    // used for the reconciliation/conflict-detection pass below.
    std::map<std::string, int> loadedCountByResource;

    while (std::getline(file, line)) {
        lineNumber++;
        if (line.empty()) continue;

        std::stringstream ss(line);
        std::string idStr, studIDStr, name, resourceID, date;

        if (!std::getline(ss, idStr, '|') ||
            !std::getline(ss, studIDStr, '|') ||
            !std::getline(ss, name, '|') ||
            !std::getline(ss, resourceID, '|') ||
            !std::getline(ss, date, '|')) {
            std::cout << "WARNING: Skipping malformed line " << lineNumber
                      << " in " << filename << "\n";
            continue;
        }

        try {
            int reservationID = std::stoi(idStr);
            int studentID = std::stoi(studIDStr);

            Reservation r(reservationID, studentID, name, resourceID, date);
            activeReservations.insert(r);
            reservationFrequency[resourceID]++;
            loadedCountByResource[resourceID]++;
            loadedCount++;

            if (reservationID > highestID) highestID = reservationID;
        } catch (const std::exception&) {
            std::cout << "WARNING: Skipping line " << lineNumber
                      << " with invalid numeric field.\n";
        }
    }

    file.close();
    nextReservationID = highestID + 1;
    std::cout << "Loaded " << loadedCount << " reservations from " << filename << ".\n";

    // Reconciliation pass: force Unavailable for any resource with an
    // active reservation, and flag resources that were loaded with more
    // than one simultaneous active reservation (a data conflict that
    // the raw resources.txt/reservations.txt files cannot represent
    // correctly, since a resource only has a single Available/Unavailable
    // flag).
    for (const auto& entry : loadedCountByResource) {
        const std::string& resourceID = entry.first;
        int count = entry.second;

        Resource* resource = findResource(resourceID);
        if (resource != nullptr && resource->isAvailable()) {
            resource->setAvailable(false);
        }

        if (count > 1) {
            std::cout << "WARNING: Resource '" << resourceID << "' has " << count
                      << " active reservations loaded against it at the same time. "
                      << "A resource can only be held by one reservation; please "
                      << "check " << filename << " for duplicate/conflicting bookings.\n";
        }
    }

    return true;
}

// Validates a date string is in MM/DD/YYYY format and represents a real
// calendar date (correct days-per-month, including leap years).
bool ReservationManager::isValidDate(const std::string& date) {
    // Expected shape: MM/DD/YYYY -> exactly 10 characters.
    if (date.size() != 10 || date[2] != '/' || date[5] != '/') {
        return false;
    }

    std::string monthStr = date.substr(0, 2);
    std::string dayStr = date.substr(3, 2);
    std::string yearStr = date.substr(6, 4);

    for (char c : monthStr) if (!isdigit(static_cast<unsigned char>(c))) return false;
    for (char c : dayStr)   if (!isdigit(static_cast<unsigned char>(c))) return false;
    for (char c : yearStr)  if (!isdigit(static_cast<unsigned char>(c))) return false;

    int month = std::stoi(monthStr);
    int day = std::stoi(dayStr);
    int year = std::stoi(yearStr);

    if (month < 1 || month > 12) return false;
    if (year < 1900 || year > 2999) return false;

    static const int daysInMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    int maxDay = daysInMonth[month - 1];

    bool isLeapYear = (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
    if (month == 2 && isLeapYear) maxDay = 29;

    if (day < 1 || day > maxDay) return false;

    return true;
}

// Checks whether 'studentID' already has an active reservation for
// 'resourceID', to prevent the same student double-booking the same
// resource. O(n) traversal of the active-reservations linked list.
bool ReservationManager::hasDuplicateReservation(int studentID, const std::string& resourceID) const {
    Reservation dummy;
    return activeReservations.find(
        [studentID, resourceID](const Reservation& r) {
            return r.getStudentID() == studentID && r.getResourceID() == resourceID;
        },
        dummy);
}

CreateResult ReservationManager::createReservation(int studentID, const std::string& studentName,
                                                     const std::string& resourceID,
                                                     const std::string& date) {
    Resource* resource = findResource(resourceID);
    if (resource == nullptr) {
        return CreateResult::INVALID_RESOURCE;
    }

    if (!isValidDate(date)) {
        return CreateResult::INVALID_DATE;
    }

    if (hasDuplicateReservation(studentID, resourceID)) {
        return CreateResult::DUPLICATE_REQUEST;
    }

    if (!resource->isAvailable()) {
        // Resource is busy: place the student on its FIFO waiting list.
        // FIX: the requested date is now passed along and stored instead
        // of being silently dropped.
        waitingList.addRequest(resourceID, studentID, studentName, date);
        return CreateResult::ADDED_TO_WAITLIST;
    }

    // Resource is free: create the reservation and mark it unavailable.
    int newID = nextReservationID++;
    Reservation r(newID, studentID, studentName, resourceID, date);
    activeReservations.insert(r);
    resource->setAvailable(false);
    reservationFrequency[resourceID]++;

    std::cout << "Reservation Created Successfully. (Reservation ID: " << newID << ")\n";
    return CreateResult::SUCCESS;
}

bool ReservationManager::cancelReservation(int reservationID) {
    Reservation removed;
    bool found = activeReservations.remove(
        [reservationID](const Reservation& r) { return r.getReservationID() == reservationID; },
        removed);

    if (!found) {
        return false;
    }

    // Record on the cancellation history stack (undo support).
    cancellationHistory.recordCancellation(removed);

    // Free up the resource.
    Resource* resource = findResource(removed.getResourceID());
    if (resource != nullptr) {
        resource->setAvailable(true);
    }

    std::cout << "Reservation Cancelled.\nAdded to cancellation history.\n";

    // Automatically assign the now-free resource to the next waiting
    // student, if any (FIFO waiting list processing).
    // FIX: the new reservation now uses the WAITING STUDENT's own
    // requested date (next.getRequestedDate()) instead of reusing the
    // cancelled reservation's date, which belonged to a different student.
    if (resource != nullptr && !waitingList.isEmpty(removed.getResourceID())) {
        WaitingRequest next;
        if (waitingList.removeNext(removed.getResourceID(), next)) {
            int newID = nextReservationID++;
            Reservation autoReservation(newID, next.getStudentID(), next.getStudentName(),
                                         next.getResourceID(), next.getRequestedDate());
            activeReservations.insert(autoReservation);
            resource->setAvailable(false);
            reservationFrequency[next.getResourceID()]++;

            std::cout << "Resource " << removed.getResourceID()
                      << " automatically assigned to next student in waiting list: "
                      << next.getStudentName() << " (Student #" << next.getStudentID()
                      << "), Reservation ID: " << newID << "\n";
        }
    }

    return true;
}

bool ReservationManager::reservationExists(int reservationID) const {
    Reservation dummy;
    return activeReservations.find(
        [reservationID](const Reservation& r) { return r.getReservationID() == reservationID; },
        dummy);
}

void ReservationManager::displayActiveReservations() const {
    std::cout << "Active Reservations (" << activeReservations.size() << " total):\n";
    activeReservations.display();
}

// Linear search through the active-reservations linked list by ID.
void ReservationManager::searchReservationByID(int reservationID) const {
    Reservation result;
    bool found = activeReservations.find(
        [reservationID](const Reservation& r) { return r.getReservationID() == reservationID; },
        result);
    if (!found) {
        std::cout << "No active reservation found with ID " << reservationID << ".\n";
        return;
    }
    result.display();
}

// Linear search through active reservations for every one belonging to
// the given student.
void ReservationManager::searchReservationsByStudent(int studentID) const {
    bool foundAny = false;
    activeReservations.forEach([&](const Reservation& r) {
        if (r.getStudentID() == studentID) {
            r.display();
            foundAny = true;
        }
    });
    if (!foundAny) {
        std::cout << "No active reservations found for student " << studentID << ".\n";
    }
}

// ---------------------------------------------------------------
// Waiting List Management
// ---------------------------------------------------------------

void ReservationManager::displayWaitingLists() const {
    // FIX: WaitingList::displayAll() is now const, so the const_cast
    // that used to be here is no longer needed.
    waitingList.displayAll();
}

// ---------------------------------------------------------------
// Cancellation History (Undo)
// ---------------------------------------------------------------

bool ReservationManager::undoCancellation() {
    Reservation restored;
    if (!cancellationHistory.undoLastCancellation(restored)) {
        std::cout << "No cancellations to undo.\n";
        return false;
    }

    // Put the reservation back into the active list.
    activeReservations.insert(restored);
    reservationFrequency[restored.getResourceID()]++;

    // Mark the resource unavailable again, if it exists.
    Resource* resource = findResource(restored.getResourceID());
    if (resource != nullptr) {
        resource->setAvailable(false);
    }

    std::cout << "Reservation Restored Successfully.\n";
    restored.display();
    return true;
}

void ReservationManager::displayCancellationHistory() const {
    std::cout << "Cancellation History (most recent first):\n";
    cancellationHistory.display();
}

// ---------------------------------------------------------------
// Reporting
// ---------------------------------------------------------------

// Generates the four required system reports, each built directly from
// the data this system already maintains (no re-reading files, no
// separate bookkeeping):
//   1. Active Reservations     - from the activeReservations linked list
//   2. Resource Utilization    - every resource in inventory, paired
//                                 with its reservation count from
//                                 reservationFrequency (0 if never
//                                 reserved, so nothing in the inventory
//                                 is left out)
//   3. Most Requested Resources - the same reservationFrequency data,
//                                 ranked descending with merge sort
//   4. Waiting-List Statistics  - every resource's current waiting
//                                 count from the WaitingList (FIFO
//                                 queues), plus a system-wide total
void ReservationManager::generateReport() const {
    std::cout << "\n================ SYSTEM REPORT ================\n";

    // --- 1. Active Reservations ---
    std::cout << "-- Active Reservations --\n";
    std::cout << "There are currently " << activeReservations.size()
              << " active reservation(s).\n";

    // --- 2. Resource Utilization ---
    std::cout << "\n-- Resource Utilization --\n";
    if (resources.empty()) {
        std::cout << "No resources loaded.\n";
    } else {
        std::cout << std::left << std::setw(8) << "ID" << std::setw(22) << "Name"
                  << "Reservations\n";
        std::cout << std::string(45, '-') << "\n";
        for (const auto& r : resources) {
            auto it = reservationFrequency.find(r.getID());
            int count = (it != reservationFrequency.end()) ? it->second : 0;
            std::cout << std::left << std::setw(8) << r.getID()
                      << std::setw(22) << r.getName() << count << "\n";
        }
    }

    // --- 3. Most Requested Resources ---
    std::cout << "\n-- Most Requested Resources --\n";
    if (reservationFrequency.empty()) {
        std::cout << "No reservations have been made yet.\n";
    } else {
        // Copy into a vector so we can rank it with merge sort.
        std::vector<std::pair<std::string, int>> entries(
            reservationFrequency.begin(), reservationFrequency.end());

        mergeSort(entries, [](const std::pair<std::string, int>& a,
                               const std::pair<std::string, int>& b) {
            return a.second > b.second; // descending by count
        });

        for (const auto& entry : entries) {
            std::cout << "  " << std::left << std::setw(8) << entry.first
                      << " - " << entry.second << " reservation(s)\n";
        }
    }

    // --- 4. Waiting-List Statistics ---
    std::cout << "\n-- Waiting-List Statistics --\n";
    if (resources.empty()) {
        std::cout << "No resources loaded.\n";
    } else {
        bool anyWaiting = false;
        int totalWaiting = 0;
        std::cout << std::left << std::setw(8) << "ID" << "Students Waiting\n";
        std::cout << std::string(30, '-') << "\n";
        for (const auto& r : resources) {
            int count = waitingList.waitingCount(r.getID());
            if (count > 0) {
                anyWaiting = true;
                totalWaiting += count;
                std::cout << std::left << std::setw(8) << r.getID() << count << "\n";
            }
        }
        if (!anyWaiting) {
            std::cout << "No students are currently on any waiting list.\n";
        } else {
            std::cout << "Total students waiting (all resources): " << totalWaiting << "\n";
        }
    }

    std::cout << "=================================================\n";
}

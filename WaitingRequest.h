#ifndef WAITINGREQUEST_H
#define WAITINGREQUEST_H

#include <string>
#include <iostream>
#include <iomanip>

// Represents a single student's request to be placed on a resource's
// waiting list when that resource is unavailable.
//
// FIX: this now stores the date the student actually asked for
// (requestedDate). Previously this class had no date field at all, so
// the date the student typed in at the "Create Reservation" prompt was
// silently thrown away, and when the resource later freed up the student
// was auto-assigned a reservation dated with whatever date the PREVIOUS
// (cancelled) reservation happened to have - i.e. a date that belonged
// to a different student entirely. Storing it here fixes that.
class WaitingRequest {
private:
    int studentID;
    std::string studentName;
    std::string resourceID;
    std::string requestedDate;

public:
    WaitingRequest()
        : studentID(0), studentName(""), resourceID(""), requestedDate("") {}

    WaitingRequest(int studentID, const std::string& studentName,
                   const std::string& resourceID, const std::string& requestedDate)
        : studentID(studentID), studentName(studentName),
          resourceID(resourceID), requestedDate(requestedDate) {}

    int getStudentID() const { return studentID; }
    std::string getStudentName() const { return studentName; }
    std::string getResourceID() const { return resourceID; }
    std::string getRequestedDate() const { return requestedDate; }

    void display() const {
        std::cout << std::left
                  << "Student#" << std::setw(8) << studentID
                  << std::setw(20) << studentName
                  << "waiting for Resource:" << std::setw(8) << resourceID
                  << "Requested Date:" << requestedDate
                  << "\n";
    }
};

#endif // WAITINGREQUEST_H

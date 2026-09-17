#ifndef WAITINGREQUEST_H
#define WAITINGREQUEST_H

#include <string>
#include <iostream>
#include <iomanip>

// Represents a single student's request to be placed on a resource's
// waiting list when that resource is unavailable.
class WaitingRequest {
private:
    int studentID;
    std::string studentName;
    std::string resourceID;

public:
    WaitingRequest() : studentID(0), studentName(""), resourceID("") {}

    WaitingRequest(int studentID, const std::string& studentName, const std::string& resourceID)
        : studentID(studentID), studentName(studentName), resourceID(resourceID) {}

    int getStudentID() const { return studentID; }
    std::string getStudentName() const { return studentName; }
    std::string getResourceID() const { return resourceID; }

    void display() const {
        std::cout << std::left
                  << "Student#" << std::setw(8) << studentID
                  << std::setw(20) << studentName
                  << "waiting for Resource:" << resourceID
                  << "\n";
    }
};

#endif // WAITINGREQUEST_H

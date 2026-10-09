#ifndef RESERVATION_H
#define RESERVATION_H
#include <string>
#include "Student.h"

// Represents a single active (or historical) reservation record.
class Reservation {
private:
    int reservationID;
    int studentID;
    std::string studentName;
    std::string resourceID;
    std::string reservationDate;

public:
    Reservation();
    Reservation(int reservationID, int studentID, const std::string& studentName,
                const std::string& resourceID, const std::string& date);

    // Getters
    int getReservationID() const;
    int getStudentID() const;
    std::string getStudentName() const;
    std::string getResourceID() const;
    std::string getDate() const;

    // Returns the reserving student as a Student object.
    Student getStudent() const;

    // Returns the date as YYYYMMDD so dates compare correctly as strings
    // (MM/DD/YYYY would sort by month first, which is wrong across years).
    std::string getSortableDate() const;

    // Prints a single formatted line describing this reservation.
    void display() const;
};

#endif // RESERVATION_H

#ifndef RESERVATION_H
#define RESERVATION_H

#include <string>

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

    // Prints a single formatted line describing this reservation.
    void display() const;
};

#endif // RESERVATION_H

#include "Reservation.h"
#include <iostream>
#include <iomanip>

Reservation::Reservation()
    : reservationID(0), studentID(0), studentName(""), resourceID(""), reservationDate("") {}

Reservation::Reservation(int reservationID, int studentID, const std::string& studentName,
                          const std::string& resourceID, const std::string& date)
    : reservationID(reservationID), studentID(studentID), studentName(studentName),
      resourceID(resourceID), reservationDate(date) {}

int Reservation::getReservationID() const { return reservationID; }
int Reservation::getStudentID() const { return studentID; }
std::string Reservation::getStudentName() const { return studentName; }
std::string Reservation::getResourceID() const { return resourceID; }
std::string Reservation::getDate() const { return reservationDate; }

Student Reservation::getStudent() const { return Student(studentID, studentName); }

std::string Reservation::getSortableDate() const {
    // Expects MM/DD/YYYY (validated before a reservation is created).
    if (reservationDate.size() != 10) return reservationDate;   // fallback for odd data
    return reservationDate.substr(6, 4) + reservationDate.substr(0, 2) + reservationDate.substr(3, 2);
}

void Reservation::display() const {
    std::cout << std::left
              << "Res#" << std::setw(6) << reservationID
              << "Student#" << std::setw(8) << studentID
              << std::setw(20) << studentName
              << "Resource:" << std::setw(8) << resourceID
              << "Date:" << reservationDate
              << "\n";
}

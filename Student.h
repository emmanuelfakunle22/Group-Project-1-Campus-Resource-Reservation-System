#ifndef STUDENT_H
#define STUDENT_H

#include <string>

// Represents a student who makes reservations. Groups the student's ID and
// name into one object so they travel together through the system
// (menu input -> manager -> Reservation) instead of as loose parameters.
class Student {
private:
    int studentID;            // numeric campus ID
    std::string studentName;  // full name

public:
    Student();                                          // empty student (ID 0)
    Student(int id, const std::string& name);           // full constructor

    int getID() const;                // returns the student ID
    std::string getName() const;      // returns the student name

    // A student is valid if the ID is positive and the name is non-empty.
    bool isValid() const;

    // Prints a one-line description of the student.
    void display() const;
};

#endif // STUDENT_H

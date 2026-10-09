#include "Student.h"
#include <iostream>

Student::Student() : studentID(0), studentName("") {}   // default: invalid student

Student::Student(int id, const std::string& name) : studentID(id), studentName(name) {}

int Student::getID() const { return studentID; }
std::string Student::getName() const { return studentName; }

bool Student::isValid() const {
    return studentID > 0 && !studentName.empty();   // both fields must be usable
}

void Student::display() const {
    std::cout << "Student#" << studentID << " " << studentName << "\n";
}

#ifndef REPORTGENERATOR_H
#define REPORTGENERATOR_H

#include "ReservationManager.h"

// Builds the system reports. It is deliberately separate from
// ReservationManager: the manager owns and changes the data, while this
// class only READS it (through the manager's const accessors) and prints
// formatted, merge-sorted reports. Keeps each class to one job.
class ReportGenerator {
private:
    const ReservationManager& manager;   // read-only view of the system data

    void printActiveReservations() const;   // report 1
    void printResourceUtilization() const;  // report 2
    void printMostRequested() const;        // report 3
    void printWaitingStatistics() const;    // report 4

public:
    explicit ReportGenerator(const ReservationManager& mgr);

    // Prints all four reports in order.
    void generateFullReport() const;
};

#endif // REPORTGENERATOR_H

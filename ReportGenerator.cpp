#include "ReportGenerator.h"
#include "Algorithms.h"
#include <iostream>
#include <iomanip>
#include <utility>

ReportGenerator::ReportGenerator(const ReservationManager& mgr) : manager(mgr) {}

// Report 1: every active reservation, merge-sorted by resource ID
// (ties broken by date) so the listing is grouped and predictable.
void ReportGenerator::printActiveReservations() const {
    std::vector<Reservation> list = manager.getActiveReservationsSnapshot();
    std::cout << "-- Active Reservations --\n";
    std::cout << "There are currently " << list.size() << " active reservation(s).\n";
    if (list.empty()) return;

    mergeSort(list, [](const Reservation& a, const Reservation& b) {
        if (a.getResourceID() != b.getResourceID()) return a.getResourceID() < b.getResourceID();
        return a.getSortableDate() < b.getSortableDate();
    });
    for (const auto& r : list) r.display();
}

// Report 2: reservation count for every resource, merge-sorted by count
// (highest first, ties by resource ID). Resources never reserved show 0.
void ReportGenerator::printResourceUtilization() const {
    std::cout << "\n-- Resource Utilization --\n";
    const auto& resources = manager.getResources();
    if (resources.empty()) {
        std::cout << "No resources loaded.\n";
        return;
    }
    const auto& freq = manager.getReservationFrequency();

    // Pair each resource with its count so the pair can be sorted.
    std::vector<std::pair<Resource, int>> rows;
    for (const auto& r : resources) {
        auto it = freq.find(r.getID());
        rows.push_back(std::make_pair(r, it != freq.end() ? it->second : 0));
    }
    mergeSort(rows, [](const std::pair<Resource, int>& a, const std::pair<Resource, int>& b) {
        if (a.second != b.second) return a.second > b.second;
        return a.first.getID() < b.first.getID();
    });

    std::cout << std::left << std::setw(8) << "ID" << std::setw(22) << "Name" << "Reservations\n";
    std::cout << std::string(45, '-') << "\n";
    for (const auto& row : rows) {
        std::cout << std::left << std::setw(8) << row.first.getID()
                  << std::setw(22) << row.first.getName() << row.second << "\n";
    }
}

// Report 3: only resources that were actually reserved, ranked by count.
void ReportGenerator::printMostRequested() const {
    std::cout << "\n-- Most Requested Resources --\n";
    const auto& freq = manager.getReservationFrequency();
    if (freq.empty()) {
        std::cout << "No reservations have been made yet.\n";
        return;
    }
    std::vector<std::pair<std::string, int>> entries(freq.begin(), freq.end());
    mergeSort(entries, [](const std::pair<std::string, int>& a, const std::pair<std::string, int>& b) {
        return a.second > b.second;   // descending by count (stable for ties)
    });
    int rank = 1;
    for (const auto& e : entries) {
        std::cout << "  " << std::right << std::setw(2) << rank++ << ". " << std::left
                  << std::setw(8) << e.first << " - " << e.second << " reservation(s)\n";
    }
}

// Report 4: students waiting per resource (longest lines first) + total.
void ReportGenerator::printWaitingStatistics() const {
    std::cout << "\n-- Waiting-List Statistics --\n";
    const auto& resources = manager.getResources();
    if (resources.empty()) {
        std::cout << "No resources loaded.\n";
        return;
    }
    std::vector<std::pair<std::string, int>> rows;
    int total = 0;
    for (const auto& r : resources) {
        int count = manager.getWaitingList().waitingCount(r.getID());
        if (count > 0) {
            rows.push_back(std::make_pair(r.getID(), count));
            total += count;
        }
    }
    if (rows.empty()) {
        std::cout << "No students are currently on any waiting list.\n";
        return;
    }
    mergeSort(rows, [](const std::pair<std::string, int>& a, const std::pair<std::string, int>& b) {
        return a.second > b.second;
    });
    std::cout << std::left << std::setw(8) << "ID" << "Students Waiting\n";
    std::cout << std::string(30, '-') << "\n";
    for (const auto& row : rows) {
        std::cout << std::left << std::setw(8) << row.first << row.second << "\n";
    }
    std::cout << "Total students waiting (all resources): " << total << "\n";
}

void ReportGenerator::generateFullReport() const {
    std::cout << "\n================ SYSTEM REPORT ================\n";
    printActiveReservations();
    printResourceUtilization();
    printMostRequested();
    printWaitingStatistics();
    std::cout << "=================================================\n";
}

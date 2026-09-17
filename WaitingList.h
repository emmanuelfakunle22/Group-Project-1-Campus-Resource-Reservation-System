#ifndef WAITINGLIST_H
#define WAITINGLIST_H

#include <map>
#include <string>
#include "DataStructures.h"
#include "WaitingRequest.h"

// Manages one waiting Queue<WaitingRequest> per resource ID.
// Requests are always served First Come, First Served (FIFO).
class WaitingList {
private:
    std::map<std::string, Queue<WaitingRequest>> queues;

public:
    // Add a student to the back of the queue for a given resource. O(1).
    void addRequest(const std::string& resourceID, int studentID, const std::string& studentName) {
        WaitingRequest request(studentID, studentName, resourceID);
        queues[resourceID].enqueue(request);
    }

    // Remove and return the next student in line for a resource (FIFO).
    // Returns true if a request was returned, false if the queue was empty
    // or did not exist. O(1).
    bool removeNext(const std::string& resourceID, WaitingRequest& result) {
        auto it = queues.find(resourceID);
        if (it == queues.end()) return false;
        return it->second.dequeue(result);
    }

    bool isEmpty(const std::string& resourceID) {
        auto it = queues.find(resourceID);
        if (it == queues.end()) return true;
        return it->second.isEmpty();
    }

    int waitingCount(const std::string& resourceID) {
        auto it = queues.find(resourceID);
        if (it == queues.end()) return 0;
        return it->second.size();
    }

    // Display the waiting list for a single resource.
    void display(const std::string& resourceID) {
        auto it = queues.find(resourceID);
        if (it == queues.end() || it->second.isEmpty()) {
            std::cout << "  No students waiting for resource " << resourceID << ".\n";
            return;
        }
        it->second.display();
    }

    // Display every non-empty waiting list.
    void displayAll() {
        bool any = false;
        for (auto& entry : queues) {
            if (!entry.second.isEmpty()) {
                any = true;
                std::cout << "Waiting list for resource " << entry.first << ":\n";
                entry.second.display();
            }
        }
        if (!any) {
            std::cout << "  No students are currently on any waiting list.\n";
        }
    }
};

#endif // WAITINGLIST_H

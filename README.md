
# Group-Project-1-Campus-Resource-Reservation-System
This project is from class CSCE 2110 Foundations of Data Structures. 
The team members are Emmanuel Fakunle (Team Leader), Andrew Camargo, and Jacob Beard.

Menu-driven C++17 program for reserving campus resources (study rooms, laptops, tutoring, lab gear).

## Features implemented in this milestone

- **Resource management**: load resources from `data/resources.txt`,
  display all resources, display availability. - EMMANUEL FAKUNLE
- **Reservation management**: create reservations, cancel reservations,
  display active reservations, validate requests (unknown resource ID,
  empty fields, non-numeric student ID). - EMMANUEL FAKUNLE
- **Linked list**: active reservations are stored in a custom
  `LinkedList<Reservation>` (insert, remove-by-predicate, traverse,
  display). - ANDREW CAMARGO
- **Waiting lists**: a custom `Queue<WaitingRequest>` per resource,
  managed by the `WaitingList` class (FIFO add/remove/display). When a
  reservation is cancelled, the freed resource is automatically handed
  to the next student in that resource's waiting queue, if any. - ANDREW CAMARGO
- **Cancellation history / undo**: a custom `Stack<Reservation>`,
  managed by the `CancellationHistory` class. Only the most recently
  cancelled reservation can be restored. - JACOB BEARD
- **Searching Algorithm - ANDREW CAMARGO
- **Sorting Algorithm - JACOB BEARD
- **Reporting System - EMMANUEL FAKUNLE

## Running

Windows (MinGW g++, e.g. via MSYS2)

cd Project1
g++ -std=c++17 -Iinclude -o campus_reservation_system.exe src/main.cpp src/Resource.cpp src/Reservation.cpp src/ReservationManager.cpp
campus_reservation_system.exe

Windows (Visual Studio Developer Command Prompt)

cd Project1
cl /EHsc /std:c++17 /Iinclude src\main.cpp src\Resource.cpp src\Reservation.cpp src\ReservationManager.cpp /Fe:campus_reservation_system.exe
campus_reservation_system.exe

macOS / Linux

cd Project1
g++ -std=c++17 -Iinclude -o campus_reservation_system src/main.cpp src/Resource.cpp src/Reservation.cpp src/ReservationManager.cpp
./campus_reservation_system

Run the program **from the `Project1/` directory** so that it can find
`data/resources.txt` and `data/reservations.txt` (the program loads these relative paths on startup):

```bash
./campus_reservation_system
```

You'll see a menu:

```
===== Campus Resource Reservation System =====
1. View Resources
2. View Resource Availability
3. Create Reservation
4. Cancel Reservation
5. View Active Reservations
6. View Waiting Lists
7. Undo Last Cancellation
8. View Cancellation History
9. Search Resources
10. Search Reservations
11. Sort Resources
12. Sort Reservations
13. Generate Report
14. Exit
Enter Choice:
```

## Data file formats
`data/resources.txt` (pipe-delimited):
```
ResourceID|ResourceName|ResourceType|Available|Unavailable
```
`data/reservations.txt` (pipe-delimited, preloaded as sample/historical
active reservations):
```
ReservationID|StudentID|StudentName|ResourceID|Date
```
## Classes
| Class | Role |
|---|---|
| Resource, Reservation, Student | Data objects |
| ReservationManager | Owns all data, performs reserve / cancel / undo / search / sort |
| WaitingList, WaitingRequest | One FIFO queue of waiting students per resource |
| CancellationHistory | Stack of cancelled reservations (undo) |
| ReportGenerator | Read-only; builds the four system reports |

## Data structures (hand-written, DataStructures.h)
LinkedList (active reservations), Queue (waiting lists), Stack (cancellation history), std::vector (resource inventory).

## Algorithms (Algorithms.h)
- Linear search: resources by ID, reservations by reservation ID or student ID.
- Merge sort (stable, O(n log n)): resources by name/type; reservations by ID/student name/resource ID/date; all report tables.

## Reports (menu 13)
Active reservations, resource utilization, most requested resources, waiting-list statistics.
## Error handling checklist

- [x] Invalid/out-of-range menu selections
- [x] File-open failures (missing/misnamed data files)
- [x] Invalid resource IDs on reservation creation
- [x] Invalid reservation IDs on cancellation
- [x] Empty cancellation-history stack (undo with nothing to undo)
- [x] Empty waiting-list queues (display, or removing from an empty list)
- [x] Searching for a resource, reservation, or student that does not exist
- [x] Non-numeric menu input, missing data files, unknown resource IDs, invalid dates, duplicate bookings by the same student, empty queue/stack (nothing to undo), and undo blocked when the resource was re-booked.

See `COMPLEXITY_ANALYSIS.md` for the Big-O. - JACOB BEARD

[README.MD](https://github.com/user-attachments/files/32312244/README.MD)
# Group-Project-1-Campus-Resource-Reservation-System
This project is from class CSCE 2110 Foundations of Data Structures. 
The team members are Emmanuel Fakunle (Team Leader), Andrew Camargo, and Jacob Beard.

# Campus Resource Reservation System — Milestone 1

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

## Building

From the `Project1/` directory:

```bash
g++ -std=c++17 -Wall -Iinclude src/main.cpp src/Resource.cpp src/Reservation.cpp src/ReservationManager.cpp -o campus_reservation_system
```

## Running

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
9. Exit
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
## Error handling checklist

- [x] Invalid/out-of-range menu selections
- [x] File-open failures (missing/misnamed data files)
- [x] Invalid resource IDs on reservation creation
- [x] Invalid reservation IDs on cancellation
- [x] Empty cancellation-history stack (undo with nothing to undo)
- [x] Empty waiting-list queues (display, or removing from an empty list)

See `COMPLEXITY_ANALYSIS.md` for the Big-O. - JACOB BEARD

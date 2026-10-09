# Demo Script (about 5 minutes)

Start from the Project1 folder with a fresh run: `./campus_reservation_system` (Windows: `campus_reservation_system.exe`).
Each line is what to type. Lines in *italics* are what to say. Do the steps in order; later steps depend on earlier ones.

## 0. Intro (Emmanuel)
*"Menu-driven reservation system in C++. Linked list for active reservations, queue for waiting lists, stack for cancellation history, vector for resources. Search and sort are our own algorithms."*
- Point out: loads 20 resources and 20 reservations with no warnings.

## 1. Empty-stack handling (Jacob)
- `7` then `8` -> "No cancellations to undo" and an empty history. *"Empty stack is handled, no crash."*

## 2. Searching (Andrew) - linear search
- `9`, `R105` -> shows Laptop 01.
- `10`, `2`, `1003` -> finds Sara Lee's reservation (by student ID).
- `10`, `1`, `305` -> finds reservation 305 (by reservation ID).

## 3. Cancel and undo (Jacob) - stack
- `4`, `301` -> cancelled, added to history.
- `8` -> shows reservation 301 on the stack.
- `7` -> restored. *"Last in, first out."*

## 4. Waiting list (Andrew) - queue
- `3`, `2001`, `Zed`, `R101`, `10/01/2026` -> R101 is taken, Zed goes on the waiting list.
- `3`, `2002`, `Yan`, `R101`, `10/02/2026` -> Yan goes on the list behind Zed.
- `6` -> shows Zed first, Yan second. *"First come, first served."*
- `4`, `301` -> R101 is automatically given to Zed (new reservation 321), using Zed's own requested date.
- `7` -> "Cannot undo: resource R101 has been re-booked." *"We stop the undo from double-booking."*

## 5. Error handling (Emmanuel)
- `3`, `2003`, `Al`, `R999`, `10/01/2026` -> invalid resource ID.
- `3`, `2003`, `Al`, `R102`, `13/40/2026` -> invalid date.
- `3`, `2001`, `Zed`, `R101`, `10/05/2026` -> duplicate reservation rejected.
- Type `abc` at the menu -> "Invalid input", asks again.

## 6. Sorting (Jacob) - merge sort
- `11`, `1` -> resources sorted by name.
- `12`, `2` -> reservations sorted by student name.
- `12`, `4` -> reservations sorted by date.
- *"Stable merge sort, O(n log n). The list itself is untouched; we sort a copy."*

## 7. Reports (Emmanuel)
- `13` -> scroll through the four sections:
  1. Active reservations (sorted by resource)
  2. Resource utilization (reservation count per resource)
  3. Most requested resources (ranked)
  4. Waiting-list statistics (R101: 1 waiting, Yan)

## 8. Wrap-up
- `14` -> Goodbye.
- *"ReservationManager holds the data, ReportGenerator only reads it, Student and Reservation are the data objects. Everyone's work is in the GitHub repo."*

## If something goes wrong
- "Could not open file": you ran it from the wrong folder. `cd` into Project1 first.
- To reset the demo, quit with `14` and start again; nothing is saved between runs.

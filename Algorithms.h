#ifndef ALGORITHMS_H
#define ALGORITHMS_H

#include <vector>

// =====================================================================
// Algorithms.h
//
// Standalone, reusable algorithm implementations satisfying the two
// required algorithm categories:
//   1. Searching -> linearSearchIndex()   (used for resource/reservation
//                                          lookups by ID / student ID)
//   2. Sorting   -> mergeSort()           (used for resource/reservation
//                                          reports)
// Both are templated so they work on Resource, Reservation, or any
// other type, given the right predicate/comparator.
// =====================================================================

// ---------------------------------------------------------------------
// LINEAR SEARCH
// Scans a vector from the front until predicate(items[i]) is true.
// O(n) worst case. Returns the index of the first match, or -1.
// ---------------------------------------------------------------------
template <typename T, typename Predicate>
int linearSearchIndex(const std::vector<T>& items, Predicate predicate) {
    for (int i = 0; i < static_cast<int>(items.size()); i++) {
        if (predicate(items[i])) {
            return i;
        }
    }
    return -1;
}

// ---------------------------------------------------------------------
// MERGE SORT
// Classic divide-and-conquer sort: O(n log n) worst case, stable.
// "compare(a, b)" returns true if a should come before b.
// ---------------------------------------------------------------------
template <typename T, typename Compare>
void mergeHelper(std::vector<T>& items, int left, int mid, int right,
                  Compare compare) {
    std::vector<T> leftHalf(items.begin() + left, items.begin() + mid + 1);
    std::vector<T> rightHalf(items.begin() + mid + 1, items.begin() + right + 1);

    int i = 0, j = 0, k = left;
    while (i < static_cast<int>(leftHalf.size()) &&
           j < static_cast<int>(rightHalf.size())) {
        if (compare(leftHalf[i], rightHalf[j])) {
            items[k++] = leftHalf[i++];
        } else {
            items[k++] = rightHalf[j++];
        }
    }
    while (i < static_cast<int>(leftHalf.size())) items[k++] = leftHalf[i++];
    while (j < static_cast<int>(rightHalf.size())) items[k++] = rightHalf[j++];
}

template <typename T, typename Compare>
void mergeSortHelper(std::vector<T>& items, int left, int right,
                      Compare compare) {
    if (left >= right) return;
    int mid = left + (right - left) / 2;
    mergeSortHelper(items, left, mid, compare);
    mergeSortHelper(items, mid + 1, right, compare);
    mergeHelper(items, left, mid, right, compare);
}

// Public entry point: sorts 'items' in place according to 'compare'.
template <typename T, typename Compare>
void mergeSort(std::vector<T>& items, Compare compare) {
    if (items.size() < 2) return;
    mergeSortHelper(items, 0, static_cast<int>(items.size()) - 1, compare);
}

#endif // ALGORITHMS_H

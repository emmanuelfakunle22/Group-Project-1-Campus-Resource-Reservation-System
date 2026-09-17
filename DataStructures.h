#ifndef DATASTRUCTURES_H
#define DATASTRUCTURES_H

#include <functional>
#include <iostream>

// =====================================================================
// DataStructures.h
//
// Hand-written (non-STL-container) implementations of the three core
// data structures required by the project:
//   - LinkedList<T>  : singly linked list  -> active reservations
//   - Queue<T>        : FIFO queue          -> waiting lists
//   - Stack<T>         : LIFO stack          -> cancellation history
//
// Each type T stored in these containers is expected to provide a
// "void display() const" member function, which is used by the
// display()/print routines below.
//
// See COMPLEXITY_ANALYSIS.md for a discussion of the time complexity
// of each operation.
// =====================================================================

// ---------------------------------------------------------------
// LinkedList<T> - singly linked list with head/tail pointers
// ---------------------------------------------------------------
template <typename T>
class LinkedList {
private:
    struct Node {
        T data;
        Node* next;
        explicit Node(const T& value) : data(value), next(nullptr) {}
    };

    Node* head;
    Node* tail;
    int count;

public:
    LinkedList() : head(nullptr), tail(nullptr), count(0) {}

    ~LinkedList() {
        clear();
    }

    // Insert a new item at the tail of the list. O(1) because we keep
    // a tail pointer (no need to traverse the whole list).
    void insert(const T& item) {
        Node* newNode = new Node(item);
        if (head == nullptr) {
            head = tail = newNode;
        } else {
            tail->next = newNode;
            tail = newNode;
        }
        count++;
    }

    // Remove the first node for which predicate(data) is true.
    // Returns true and copies the removed item into 'removedItem' if found.
    // O(n) - must traverse looking for the matching node.
    bool remove(std::function<bool(const T&)> predicate, T& removedItem) {
        Node* current = head;
        Node* previous = nullptr;

        while (current != nullptr) {
            if (predicate(current->data)) {
                removedItem = current->data;

                if (previous == nullptr) {
                    head = current->next;
                } else {
                    previous->next = current->next;
                }
                if (current == tail) {
                    tail = previous;
                }

                delete current;
                count--;
                return true;
            }
            previous = current;
            current = current->next;
        }
        return false;
    }

    // Find the first item matching predicate. Returns pointer to a copy
    // via 'result' and true on success. O(n).
    bool find(std::function<bool(const T&)> predicate, T& result) const {
        Node* current = head;
        while (current != nullptr) {
            if (predicate(current->data)) {
                result = current->data;
                return true;
            }
            current = current->next;
        }
        return false;
    }

    // Traverse every element, invoking visitor(data) on each. O(n).
    void forEach(std::function<void(const T&)> visitor) const {
        Node* current = head;
        while (current != nullptr) {
            visitor(current->data);
            current = current->next;
        }
    }

    // Display every item using its own display() method. O(n).
    void display() const {
        if (isEmpty()) {
            std::cout << "  (no records to display)\n";
            return;
        }
        Node* current = head;
        while (current != nullptr) {
            current->data.display();
            current = current->next;
        }
    }

    bool isEmpty() const { return count == 0; }
    int size() const { return count; }

    void clear() {
        Node* current = head;
        while (current != nullptr) {
            Node* next = current->next;
            delete current;
            current = next;
        }
        head = tail = nullptr;
        count = 0;
    }
};

// ---------------------------------------------------------------
// Queue<T> - FIFO queue implemented with a singly linked list.
// Used for waiting lists so requests are served in the order
// they arrived (First Come, First Served).
// ---------------------------------------------------------------
template <typename T>
class Queue {
private:
    struct Node {
        T data;
        Node* next;
        explicit Node(const T& value) : data(value), next(nullptr) {}
    };

    Node* front_;
    Node* rear_;
    int count;

public:
    Queue() : front_(nullptr), rear_(nullptr), count(0) {}
    ~Queue() { clear(); }

    // Add to the back of the queue. O(1).
    void enqueue(const T& item) {
        Node* newNode = new Node(item);
        if (rear_ == nullptr) {
            front_ = rear_ = newNode;
        } else {
            rear_->next = newNode;
            rear_ = newNode;
        }
        count++;
    }

    // Remove from the front of the queue. O(1).
    bool dequeue(T& removedItem) {
        if (front_ == nullptr) return false;
        Node* temp = front_;
        removedItem = temp->data;
        front_ = front_->next;
        if (front_ == nullptr) rear_ = nullptr;
        delete temp;
        count--;
        return true;
    }

    // Peek at the front item without removing it. O(1).
    bool peekFront(T& item) const {
        if (front_ == nullptr) return false;
        item = front_->data;
        return true;
    }

    // Display items from front to rear. O(n).
    void display() const {
        if (isEmpty()) {
            std::cout << "  (waiting list is empty)\n";
            return;
        }
        Node* current = front_;
        int position = 1;
        while (current != nullptr) {
            std::cout << "  [" << position << "] ";
            current->data.display();
            current = current->next;
            position++;
        }
    }

    bool isEmpty() const { return count == 0; }
    int size() const { return count; }

    void clear() {
        T temp;
        while (dequeue(temp)) {}
    }
};

// ---------------------------------------------------------------
// Stack<T> - LIFO stack implemented with a singly linked list.
// Used for cancellation history so the most recently cancelled
// reservation is the first one that can be restored (undo).
// ---------------------------------------------------------------
template <typename T>
class Stack {
private:
    struct Node {
        T data;
        Node* next;
        explicit Node(const T& value) : data(value), next(nullptr) {}
    };

    Node* top_;
    int count;

public:
    Stack() : top_(nullptr), count(0) {}
    ~Stack() { clear(); }

    // Push an item onto the top of the stack. O(1).
    void push(const T& item) {
        Node* newNode = new Node(item);
        newNode->next = top_;
        top_ = newNode;
        count++;
    }

    // Pop the top item off the stack. O(1).
    bool pop(T& poppedItem) {
        if (top_ == nullptr) return false;
        Node* temp = top_;
        poppedItem = temp->data;
        top_ = top_->next;
        delete temp;
        count--;
        return true;
    }

    // Peek at the top item without removing it. O(1).
    bool peek(T& item) const {
        if (top_ == nullptr) return false;
        item = top_->data;
        return true;
    }

    // Display items from top (most recent) to bottom. O(n).
    void display() const {
        if (isEmpty()) {
            std::cout << "  (cancellation history is empty)\n";
            return;
        }
        Node* current = top_;
        int position = 1;
        while (current != nullptr) {
            std::cout << "  [" << position << "] ";
            current->data.display();
            current = current->next;
            position++;
        }
    }

    bool isEmpty() const { return count == 0; }
    int size() const { return count; }

    void clear() {
        T temp;
        while (pop(temp)) {}
    }
};

#endif // DATASTRUCTURES_H

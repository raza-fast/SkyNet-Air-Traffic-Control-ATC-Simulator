#ifndef MINHEAP_H
#define MINHEAP_H

#include "Aircraft.h"

// Min-Heap for landing priority (lower priority number = higher urgency)
class MinHeap {
private:
    Aircraft** heap;  // Array of aircraft pointers
    int* heapIndices;  // Maps aircraft to heap position (for DecreaseKey)
    int capacity;
    int size;
    
    // Helper functions
    void heapifyUp(int index);
    void heapifyDown(int index);
    int parent(int index) { return (index - 1) / 2; }
    int leftChild(int index) { return 2 * index + 1; }
    int rightChild(int index) { return 2 * index + 2; }
    void swap(int i, int j);
    
public:
    MinHeap(int cap = 100);
    ~MinHeap();
    
    // Core operations
    bool insert(Aircraft* aircraft);
    Aircraft* extractMin();  // Remove and return highest priority aircraft
    bool isEmpty() const { return size == 0; }
    int getSize() const { return size; }
    
    // Priority update
    bool decreaseKey(const char* flightID, Priority newPriority);
    bool updatePriority(const char* flightID, Priority newPriority);
    
    // Access
    Aircraft* peek() const;  // View top without removing
    bool contains(const char* flightID) const;
    int findIndex(const char* flightID) const;
    
    // Utility
    void printHeap() const;
    void clear();
};

#endif // MINHEAP_H


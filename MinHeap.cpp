#include "MinHeap.h"
#include <iostream>
#include <cstring>
using namespace std;

MinHeap::MinHeap(int cap) : capacity(cap), size(0) {
    heap = new Aircraft*[capacity];
    heapIndices = new int[capacity];
    for (int i = 0; i < capacity; i++) {
        heap[i] = nullptr;
        heapIndices[i] = -1;
    }
}

MinHeap::~MinHeap() {
    // Note: We don't delete Aircraft objects here as they're managed elsewhere
    delete[] heap;
    delete[] heapIndices;
}

void MinHeap::swap(int i, int j) {
    Aircraft* temp = heap[i];
    heap[i] = heap[j];
    heap[j] = temp;
    
    // Update indices if we're tracking them
    // For simplicity, we'll rebuild indices after operations
}

void MinHeap::heapifyUp(int index) {
    while (index > 0) {
        int parentIdx = parent(index);
        
        // Compare priorities (lower number = higher priority)
        int currentPriority = int(heap[index]->getPriority());
        int parentPriority = int(heap[parentIdx]->getPriority());
        
        if (currentPriority < parentPriority) {
            swap(index, parentIdx);
            index = parentIdx;
        } else {
            break;
        }
    }
}

void MinHeap::heapifyDown(int index) {
    while (true) {
        int smallest = index;
        int left = leftChild(index);
        int right = rightChild(index);
        
        int indexPriority = int(heap[index]->getPriority());
        
        if (left < size) {
            int leftPriority = int(heap[left]->getPriority());
            if (leftPriority < indexPriority) {
                smallest = left;
            }
        }
        
        if (right < size) {
            int rightPriority = int(heap[right]->getPriority());
            int smallestPriority = int(heap[smallest]->getPriority());
            if (rightPriority < smallestPriority) {
                smallest = right;
            }
        }
        
        if (smallest != index) {
            // Manual in-place swap without using built-in swap function
            Aircraft* temp = heap[index];
            heap[index] = heap[smallest];
            heap[smallest] = temp;
            index = smallest;
        } else {
            break;
        }
    }
}

bool MinHeap::insert(Aircraft* aircraft) {
    if (size >= capacity || aircraft == nullptr) {
        return false;
    }
    
    heap[size] = aircraft;
    size++;
    heapifyUp(size - 1);
    
    return true;
}

Aircraft* MinHeap::extractMin() {
    if (size == 0) {
        return nullptr;
    }
    
    Aircraft* min = heap[0];
    heap[0] = heap[size - 1];
    heap[size - 1] = nullptr;
    size--;
    
    if (size > 0) {
        heapifyDown(0);
    }
    
    return min;
}

Aircraft* MinHeap::peek() const {
    if (size == 0) {
        return nullptr;
    }
    return heap[0];
}

bool MinHeap::contains(const char* flightID) const {
    for (int i = 0; i < size; i++) {
        if (heap[i] && strcmp(heap[i]->getFlightID(), flightID) == 0) {
            return true;
        }
    }
    return false;
}

int MinHeap::findIndex(const char* flightID) const {
    for (int i = 0; i < size; i++) {
        if (heap[i] && strcmp(heap[i]->getFlightID(), flightID) == 0) {
            return i;
        }
    }
    return -1;
}

bool MinHeap::decreaseKey(const char* flightID, Priority newPriority) {
    int index = findIndex(flightID);
    if (index == -1) {
        return false;
    }
    
    Priority oldPriority = heap[index]->getPriority();
    if (int(newPriority) >= int(oldPriority)) {
        return false;  // Not a decrease
    }
    
    heap[index]->setPriority(newPriority);
    heapifyUp(index);
    
    return true;
}

bool MinHeap::updatePriority(const char* flightID, Priority newPriority) {
    int index = findIndex(flightID);
    if (index == -1) {
        return false;
    }
    
    Priority oldPriority = heap[index]->getPriority();
    heap[index]->setPriority(newPriority);
    
    if (int(newPriority) < int(oldPriority)) {
        heapifyUp(index);
    } else {
        heapifyDown(index);
    }
    
    return true;
}

void MinHeap::printHeap() const {
cout << "\n=== Landing Queue (Min-Heap) ===\n";
    if (size == 0) {
cout << "Queue is empty.\n";
        return;
    }
    
cout << "Next to land: " << heap[0]->getFlightID() 
              << " (Priority: " << heap[0]->getPriorityString() << ")\n";
cout << "\nAll aircraft in queue:\n";
    for (int i = 0; i < size; i++) {
cout << (i + 1) << ". " << heap[i]->getFlightID() 
                  << " - Priority: " << heap[i]->getPriorityString()
                  << ", Fuel: " << heap[i]->getFuelLevel() << "%\n";
    }
}

void MinHeap::clear() {
    size = 0;
    for (int i = 0; i < capacity; i++) {
        heap[i] = nullptr;
    }
}


#include "HashTable.h"
#include <iostream>
#include <cstring>
using namespace std;

HashTable::HashTable(int size) : tableSize(size), count(0) {
    table = new HashNode*[tableSize];
    for (int i = 0; i < tableSize; i++) {
        table[i] = nullptr;
    }
}

HashTable::~HashTable() {
    clear();
    delete[] table;
}

int HashTable::hashFunction(const char* key) const {
    int sum = 0;
    for (int i = 0; key[i] != '\0'; i++) {
        sum += int(key[i]);
    }
    return sum % tableSize;
}

bool HashTable::insert(const char* key, Aircraft* value) {
    if (key == nullptr || value == nullptr) {
        return false;
    }
    
    // Check if key already exists
    if (search(key) != nullptr) {
        return false;  // Key already exists
    }
    
    int index = hashFunction(key);
    HashNode* newNode = new HashNode(key, value);
    newNode->next = table[index];
    table[index] = newNode;
    count++;
    
    return true;
}

Aircraft* HashTable::search(const char* key) const {
    if (key == nullptr) {
        return nullptr;
    }
    
    int index = hashFunction(key);
    HashNode* current = table[index];
    
    while (current) {
        if (strcmp(current->key, key) == 0) {
            return current->value;
        }
        current = current->next;
    }
    
    return nullptr;
}

bool HashTable::remove(const char* key) {
    if (key == nullptr) {
        return false;
    }
    
    int index = hashFunction(key);
    HashNode* current = table[index];
    HashNode* prev = nullptr;
    
    while (current) {
        if (strcmp(current->key, key) == 0) {
            if (prev) {
                prev->next = current->next;
            } else {
                table[index] = current->next;
            }
            delete current;
            count--;
            return true;
        }
        prev = current;
        current = current->next;
    }
    
    return false;
}

bool HashTable::update(const char* key, Aircraft* value) {
    Aircraft* existing = search(key);
    if (existing == nullptr) {
        return false;
    }
    
    // Update the value (Aircraft object is managed elsewhere)
    // We just need to update the pointer if needed
    int index = hashFunction(key);
    HashNode* current = table[index];
    
    while (current) {
        if (strcmp(current->key, key) == 0) {
            current->value = value;
            return true;
        }
        current = current->next;
    }
    
    return false;
}

void HashTable::printTable() const {
cout << "\n=== Aircraft Registry (Hash Table) ===\n";
cout << "Total aircraft: " << count << "\n\n";
    
    bool hasAircraft = false;
    for (int i = 0; i < tableSize; i++) {
        HashNode* current = table[i];
        while (current) {
            hasAircraft = true;
            Aircraft* ac = current->value;
cout << "Flight ID: " << current->key << "\n";
cout << "  Model: " << ac->getModel() << "\n";
cout << "  Origin: " << ac->getOrigin() << "\n";
cout << "  Destination: " << ac->getDestination() << "\n";
cout << "  Fuel: " << ac->getFuelLevel() << "%\n";
cout << "  Priority: " << ac->getPriorityString() << "\n";
cout << "  Type: " << ac->getTypeString() << "\n";
cout << "  Position: [" << ac->getCurrentX() << "," << ac->getCurrentY() << "]\n";
cout << "\n";
            current = current->next;
        }
    }
    
    if (!hasAircraft) {
cout << "No aircraft in registry.\n";
    }
}

void HashTable::clear() {
    for (int i = 0; i < tableSize; i++) {
        HashNode* current = table[i];
        while (current) {
            HashNode* next = current->next;
            delete current;
            current = next;
        }
        table[i] = nullptr;
    }
    count = 0;
}

Aircraft** HashTable::getAllAircraft(int& aircraftCount) const {
    aircraftCount = count;
    if (count == 0) {
        return nullptr;
    }
    
    Aircraft** aircraftArray = new Aircraft*[count];
    int index = 0;
    
    for (int i = 0; i < tableSize; i++) {
        HashNode* current = table[i];
        while (current) {
            aircraftArray[index++] = current->value;
            current = current->next;
        }
    }
    
    return aircraftArray;
}


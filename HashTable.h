#ifndef HASHTABLE_H
#define HASHTABLE_H

#include "Aircraft.h"
#include <cstring>

// Hash table node for chaining
struct HashNode {
    char* key;  // Flight ID
    Aircraft* value;
    HashNode* next;
    
    HashNode(const char* k, Aircraft* v) : value(v), next(nullptr) {
        key = new char[strlen(k) + 1];
        strcpy(key, k);
    }
    
    ~HashNode() {
        delete[] key;
        // Note: We don't delete Aircraft* as it's managed elsewhere
    }
};

// Hash Table with chaining for O(1) lookup
class HashTable {
private:
    HashNode** table;
    int tableSize;
    int count;
    
    // Hash function: Sum of ASCII values modulo table size
    int hashFunction(const char* key) const;
    
public:
    HashTable(int size = 101);  // Prime number for better distribution
    ~HashTable();
    
    // Core operations
    bool insert(const char* key, Aircraft* value);
    Aircraft* search(const char* key) const;
    bool remove(const char* key);
    bool update(const char* key, Aircraft* value);
    
    // Utility
    int getCount() const { return count; }
    int getTableSize() const { return tableSize; }
    void printTable() const;
    void clear();
    
    // For save/load
    Aircraft** getAllAircraft(int& count) const;
};

#endif // HASHTABLE_H


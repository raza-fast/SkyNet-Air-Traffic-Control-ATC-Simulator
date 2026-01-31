#ifndef AVLTREE_H
#define AVLTREE_H

#include "Aircraft.h"
#include <fstream>
using namespace std;

// AVL Tree node for flight logs
struct AVLNode {
    Aircraft* aircraft;  // Aircraft that landed/crashed
    long long timestamp;  // Arrival timestamp (key for sorting)
    AVLNode* left;
    AVLNode* right;
    int height;
    
    AVLNode(Aircraft* ac, long long ts) 
        : aircraft(ac), timestamp(ts), left(nullptr), right(nullptr), height(1) {}
};

// AVL Tree for maintaining balanced flight log history
class AVLTree {
private:
    AVLNode* root;
    
    // Helper functions
    int getHeight(AVLNode* node) const;
    int getBalanceFactor(AVLNode* node) const;
    int max(int a, int b) const { return (a > b) ? a : b; }
    
    // Rotations
    AVLNode* rotateRight(AVLNode* y);
    AVLNode* rotateLeft(AVLNode* x);
    
    // Insert helper
    AVLNode* insertHelper(AVLNode* node, Aircraft* aircraft, long long timestamp);
    
    // Traversal helpers
    void inOrderHelper(AVLNode* node) const;
    void clearHelper(AVLNode* node);
    
    // Save/Load helpers (removed - using fstream instead)
    
public:
    AVLTree();
    ~AVLTree();
    
    // Core operations
    bool insert(Aircraft* aircraft, long long timestamp);
    void printInOrder() const;  // Chronological report
    
    // Utility
    bool isEmpty() const { return root == nullptr; }
    void clear();
    
    // Save/Load
    bool saveToFile(const char* filename) const;
    bool loadFromFile(const char* filename);
    
    // Get all logs for save/load
    int getLogCount() const;
    void getAllLogs(Aircraft**& aircraftArray, long long*& timestampArray, int& count) const;
};

#endif // AVLTREE_H


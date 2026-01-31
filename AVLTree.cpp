#include "AVLTree.h"
#include <iostream>
#include <fstream>
#include <cstring>
using namespace std;

AVLTree::AVLTree() : root(nullptr) {
}

AVLTree::~AVLTree() {
    clear();
}

int AVLTree::getHeight(AVLNode* node) const {
    if (node == nullptr) {
        return 0;
    }
    return node->height;
}

int AVLTree::getBalanceFactor(AVLNode* node) const {
    if (node == nullptr) {
        return 0;
    }
    return getHeight(node->left) - getHeight(node->right);
}

AVLNode* AVLTree::rotateRight(AVLNode* y) {
    AVLNode* x = y->left;
    AVLNode* T2 = x->right;
    
    // Perform rotation
    x->right = y;
    y->left = T2;
    
    // Update heights
    y->height = max(getHeight(y->left), getHeight(y->right)) + 1;
    x->height = max(getHeight(x->left), getHeight(x->right)) + 1;
    
    return x;
}

AVLNode* AVLTree::rotateLeft(AVLNode* x) {
    AVLNode* y = x->right;
    AVLNode* T2 = y->left;
    
    // Perform rotation
    y->left = x;
    x->right = T2;
    
    // Update heights
    x->height = max(getHeight(x->left), getHeight(x->right)) + 1;
    y->height = max(getHeight(y->left), getHeight(y->right)) + 1;
    
    return y;
}

AVLNode* AVLTree::insertHelper(AVLNode* node, Aircraft* aircraft, long long timestamp) {
    // Standard BST insert
    if (node == nullptr) {
        return new AVLNode(aircraft, timestamp);
    }
    
    if (timestamp < node->timestamp) {
        node->left = insertHelper(node->left, aircraft, timestamp);
    } else if (timestamp > node->timestamp) {
        node->right = insertHelper(node->right, aircraft, timestamp);
    } else {
        // Duplicate timestamp - insert to right (or handle as needed)
        node->right = insertHelper(node->right, aircraft, timestamp);
    }
    
    // Update height
    node->height = 1 + max(getHeight(node->left), getHeight(node->right));
    
    // Get balance factor
    int balance = getBalanceFactor(node);
    
    // Left Left Case
    if (balance > 1 && timestamp < node->left->timestamp) {
        return rotateRight(node);
    }
    
    // Right Right Case
    if (balance < -1 && timestamp > node->right->timestamp) {
        return rotateLeft(node);
    }
    
    // Left Right Case
    if (balance > 1 && timestamp > node->left->timestamp) {
        node->left = rotateLeft(node->left);
        return rotateRight(node);
    }
    
    // Right Left Case
    if (balance < -1 && timestamp < node->right->timestamp) {
        node->right = rotateRight(node->right);
        return rotateLeft(node);
    }
    
    return node;
}

bool AVLTree::insert(Aircraft* aircraft, long long timestamp) {
    if (aircraft == nullptr) {
        return false;
    }
    
    root = insertHelper(root, aircraft, timestamp);
    return true;
}

void AVLTree::inOrderHelper(AVLNode* node) const {
    if (node == nullptr) {
        return;
    }
    
    inOrderHelper(node->left);
    
    // Print flight log entry
    Aircraft* ac = node->aircraft;
    cout << "Timestamp: " << node->timestamp << "\n";
    cout << "  Flight ID: " << ac->getFlightID() << "\n";
    cout << "  Model: " << ac->getModel() << "\n";
    cout << "  Origin: " << ac->getOrigin() << "\n";
    cout << "  Destination: " << ac->getDestination() << "\n";
    cout << "  Status: " << (ac->getIsCrashed() ? "CRASHED" : "LANDED") << "\n";
    cout << "  Final Fuel: " << ac->getFuelLevel() << "%\n";
    cout << "\n";
    
    inOrderHelper(node->right);
}

void AVLTree::printInOrder() const {
    cout << "\n=== Flight Log History (Chronological) ===\n";
    if (root == nullptr) {
        cout << "No flight logs available.\n";
        return;
    }
    
    inOrderHelper(root);
}

void AVLTree::clearHelper(AVLNode* node) {
    if (node == nullptr) {
        return;
    }
    
    clearHelper(node->left);
    clearHelper(node->right);
    // Note: We don't delete Aircraft* as it's managed elsewhere
    delete node;
}

void AVLTree::clear() {
    clearHelper(root);
    root = nullptr;
}

int AVLTree::getLogCount() const {
    // Helper function to count nodes
    struct Counter {
        static int count(AVLNode* node) {
            if (node == nullptr) return 0;
            return 1 + count(node->left) + count(node->right);
        }
    };
    return Counter::count(root);
}

void AVLTree::getAllLogs(Aircraft**& aircraftArray, long long*& timestampArray, int& count) const {
    count = getLogCount();
    if (count == 0) {
        aircraftArray = nullptr;
        timestampArray = nullptr;
        return;
    }
    
    aircraftArray = new Aircraft*[count];
    timestampArray = new long long[count];
    int index = 0;
    
    // In-order traversal to get chronological order
    struct Collector {
        static void collect(AVLNode* node, Aircraft**& acArray, long long*& tsArray, int& idx) {
            if (node == nullptr) return;
            collect(node->left, acArray, tsArray, idx);
            acArray[idx] = node->aircraft;
            tsArray[idx] = node->timestamp;
            idx++;
            collect(node->right, acArray, tsArray, idx);
        }
    };
    
    Collector::collect(root, aircraftArray, timestampArray, index);
}

bool AVLTree::saveToFile(const char* filename) const {
    ofstream file(filename);
    if (!file.is_open()) {
        return false;
    }
    
    int count = getLogCount();
    file << count << "\n";
    
    Aircraft** aircraftArray;
    long long* timestampArray;
    int actualCount;
    getAllLogs(aircraftArray, timestampArray, actualCount);
    
    for (int i = 0; i < actualCount; i++) {
        Aircraft* ac = aircraftArray[i];
        file << ac->getFlightID() << "|"
             << ac->getModel() << "|"
             << ac->getOrigin() << "|"
             << ac->getDestination() << "|"
             << ac->getFuelLevel() << "|"
             << int(ac->getPriority()) << "|"
             << int(ac->getType()) << "|"
             << timestampArray[i] << "|"
             << (ac->getIsLanded() ? 1 : 0) << "|"
             << (ac->getIsCrashed() ? 1 : 0) << "\n";
    }
    
    delete[] aircraftArray;
    delete[] timestampArray;
    file.close();
    return true;
}

bool AVLTree::loadFromFile(const char* filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        return false;
    }
    
    clear();
    
    int count;
    file >> count;
    file.ignore();  // Skip newline
    
    char flightID[100], model[100], origin[100], destination[100];
    double fuel;
    int priority, type, landed, crashed;
    long long timestamp;
    char delimiter;
    
    for (int i = 0; i < count; i++) {
        file.getline(flightID, 100, '|');
        file.getline(model, 100, '|');
        file.getline(origin, 100, '|');
        file.getline(destination, 100, '|');
        file >> fuel >> delimiter;
        file >> priority >> delimiter;
        file >> type >> delimiter;
        file >> timestamp >> delimiter;
        file >> landed >> delimiter;
        file >> crashed;
        file.ignore();  // Skip newline
        
        Aircraft* ac = new Aircraft(flightID, model, origin, destination, fuel,
                                    (Priority)priority, (AircraftType)type);
        ac->setLanded(landed == 1);
        ac->setCrashed(crashed == 1);
        ac->setArrivalTimestamp(timestamp);
        
        insert(ac, timestamp);
    }
    
    file.close();
    return true;
}


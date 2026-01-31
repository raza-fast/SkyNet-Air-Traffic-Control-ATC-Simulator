#ifndef SKYNET_H
#define SKYNET_H

#include "Graph.h"
#include "MinHeap.h"
#include "HashTable.h"
#include "AVLTree.h"
#include "Radar.h"
#include "Aircraft.h"

// Main SkyNet ATC System
class SkyNet {
private:
    Graph* airspace;
    MinHeap* landingQueue;
    HashTable* aircraftRegistry;
    AVLTree* flightLogs;
    Radar* radar;
    
    int nextFlightNumber;
    
    // Helper functions
    void initializeAirspace();
    Aircraft* createAircraft(const char* flightID, const char* model,
                            const char* origin, const char* dest,
                            double fuel, Priority priority, AircraftType type);
    
public:
    SkyNet();
    ~SkyNet();
    
    // Menu options
    void displayRadar();
    void addFlight();
    void declareEmergency();
    void landFlight();
    void searchFlight();
    void printLog();
    void findSafeRoute();
    void moveAircraft();  // Move aircraft with collision check
    void saveState();
    void loadState();
    
    // Main menu
    void run();
    
    // Utility
    Graph* getAirspace() { return airspace; }
    MinHeap* getLandingQueue() { return landingQueue; }
    HashTable* getRegistry() { return aircraftRegistry; }
    AVLTree* getLogs() { return flightLogs; }
};

#endif // SKYNET_H


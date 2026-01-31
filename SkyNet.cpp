#include "SkyNet.h"
#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <cstdlib>
using namespace std;

SkyNet::SkyNet() : nextFlightNumber(1) {
    airspace = new Graph(100);
    landingQueue = new MinHeap(100);
    aircraftRegistry = new HashTable(101);
    flightLogs = new AVLTree();
    
    initializeAirspace();
    
    radar = new Radar(airspace);
}

SkyNet::~SkyNet() {
    delete radar;
    delete flightLogs;
    delete aircraftRegistry;
    delete landingQueue;
    delete airspace;
}

void SkyNet::initializeAirspace() {
    // Create airports
    int jfk = airspace->addNode("JFK", true, 2, 2);
    int lhr = airspace->addNode("LHR", true, 17, 17);
    int dxb = airspace->addNode("DXB", true, 10, 3);
    int nrt = airspace->addNode("NRT", true, 18, 5);
    int cdg = airspace->addNode("CDG", true, 5, 15);
    
    // Create waypoints
    int wp1 = airspace->addNode("WP1", false, 5, 5);
    int wp2 = airspace->addNode("WP2", false, 10, 10);
    int wp3 = airspace->addNode("WP3", false, 15, 8);
    int wp4 = airspace->addNode("WP4", false, 8, 15);
    int wp5 = airspace->addNode("WP5", false, 12, 12);
    
    // Create flight corridors (edges)
    airspace->addEdge(jfk, wp1, 50.0);
    airspace->addEdge(wp1, wp2, 75.0);
    airspace->addEdge(wp2, lhr, 60.0);
    
    airspace->addEdge(jfk, dxb, 120.0);
    airspace->addEdge(dxb, wp3, 40.0);
    airspace->addEdge(wp3, nrt, 80.0);
    
    airspace->addEdge(cdg, wp4, 45.0);
    airspace->addEdge(wp4, wp5, 55.0);
    airspace->addEdge(wp5, lhr, 35.0);
    
    airspace->addEdge(wp1, wp4, 90.0);
    airspace->addEdge(wp2, wp5, 70.0);
    
    // Bidirectional connections
    airspace->addEdge(wp1, jfk, 50.0);
    airspace->addEdge(wp2, wp1, 75.0);
    airspace->addEdge(lhr, wp2, 60.0);
    airspace->addEdge(dxb, jfk, 120.0);
    airspace->addEdge(wp3, dxb, 40.0);
    airspace->addEdge(nrt, wp3, 80.0);
    airspace->addEdge(wp4, cdg, 45.0);
    airspace->addEdge(wp5, wp4, 55.0);
    airspace->addEdge(lhr, wp5, 35.0);
    airspace->addEdge(wp4, wp1, 90.0);
    airspace->addEdge(wp5, wp2, 70.0);
}

Aircraft* SkyNet::createAircraft(const char* flightID, const char* model,
                                const char* origin, const char* dest,
                                double fuel, Priority priority, AircraftType type) {
    return new Aircraft(flightID, model, origin, dest, fuel, priority, type);
}

void SkyNet::displayRadar() {
    radar->refresh();
    
    // Show landing queue head
    Aircraft* next = landingQueue->peek();
    if (next) {
cout << "Next to land: " << next->getFlightID() 
                  << " (Priority: " << next->getPriorityString() << ")\n";
    } else {
cout << "No aircraft in landing queue.\n";
    }
    
cout << "\nPress Enter to continue...";
cin.ignore();
cin.get();
}

void SkyNet::addFlight() {
    char flightID[100];
    char model[100];
    char origin[100];
    char dest[100];
    double fuel;
    int typeChoice;
    
cout << "\n=== Add New Flight ===\n";
cout << "Enter Flight ID (e.g., PK-786): ";
cin >> flightID;
    
    // Check if flight already exists
    if (aircraftRegistry->search(flightID) != nullptr) {
cout << "Error: Flight ID already exists!\n";
        return;
    }
    
cout << "Enter Aircraft Model: ";
cin >> model;
cout << "Enter Origin: ";
cin >> origin;
cout << "Enter Destination: ";
cin >> dest;
cout << "Enter Fuel Level (0-100%): ";
cin >> fuel;
    
cout << "Select Aircraft Type:\n";
cout << "1. Commercial\n";
cout << "2. Cargo\n";
cout << "3. Private\n";
cout << "4. Emergency\n";
cout << "Choice: ";
cin >> typeChoice;
    
    AircraftType type = AircraftType::COMMERCIAL;
    Priority priority = Priority::MEDIUM;
    
    switch (typeChoice) {
        case 1:
            type = AircraftType::COMMERCIAL;
            priority = Priority::MEDIUM;
            break;
        case 2:
            type = AircraftType::CARGO;
            priority = Priority::LOW;
            break;
        case 3:
            type = AircraftType::PRIVATE;
            priority = Priority::LOW;
            break;
        case 4:
            type = AircraftType::EMERGENCY;
            priority = Priority::CRITICAL;
            break;
        default:
cout << "Invalid choice, defaulting to Commercial.\n";
    }
    
    // Auto-adjust priority based on fuel
    if (fuel < 10.0) {
        priority = Priority::HIGH;
    }
    
    Aircraft* aircraft = createAircraft(flightID, model, origin, dest, fuel, priority, type);
    
    // Find an entry node (waypoint or airport)
    int entryNode = -1;
    for (int i = 0; i < airspace->getNodeCount(); i++) {
        GraphNode* node = airspace->getNode(i);
        if (node && !airspace->isNodeOccupied(i)) {
            entryNode = i;
            break;
        }
    }
    
    if (entryNode == -1) {
cout << "Error: No available entry points in airspace!\n";
        delete aircraft;
        return;
    }
    
    // Place aircraft in airspace
    if (airspace->placeAircraft(entryNode, aircraft)) {
        // Add to registry
        aircraftRegistry->insert(flightID, aircraft);
        
        // Add to landing queue
        landingQueue->insert(aircraft);
        
cout << "Flight " << flightID << " added successfully!\n";
cout << "Entry point: " << airspace->getNode(entryNode)->name << "\n";
    } else {
cout << "Error: Could not place aircraft in airspace!\n";
        delete aircraft;
    }
}

void SkyNet::declareEmergency() {
    char flightID[100];
cout << "\n=== Declare Emergency ===\n";
cout << "Enter Flight ID: ";
cin >> flightID;
    
    Aircraft* aircraft = aircraftRegistry->search(flightID);
    if (aircraft == nullptr) {
cout << "Error: Flight not found!\n";
        return;
    }
    
    aircraft->declareEmergency();
    landingQueue->updatePriority(flightID, Priority::CRITICAL);
    
cout << "Emergency declared for " << flightID << "!\n";
cout << "Priority updated to CRITICAL.\n";
}

void SkyNet::landFlight() {
cout << "\n=== Land Flight ===\n";
    
    if (landingQueue->isEmpty()) {
cout << "No aircraft in landing queue.\n";
        return;
    }
    
    Aircraft* aircraft = landingQueue->extractMin();
    if (aircraft == nullptr) {
cout << "Error: Could not process landing.\n";
        return;
    }
    
    // Remove from airspace
    int nodeID = aircraft->getCurrentNodeID();
    airspace->removeAircraft(nodeID);
    
    // Mark as landed (timestamp will be set automatically in setLanded)
    aircraft->setLanded(true);
    
    // Add to flight logs
    flightLogs->insert(aircraft, aircraft->getArrivalTimestamp());
    
cout << "Flight " << aircraft->getFlightID() << " has landed successfully!\n";
cout << "Status: " << (aircraft->getIsCrashed() ? "CRASHED" : "SAFE") << "\n";
}

void SkyNet::searchFlight() {
    char flightID[100];
cout << "\n=== Search Flight ===\n";
cout << "Enter Flight ID: ";
cin >> flightID;
    
    Aircraft* aircraft = aircraftRegistry->search(flightID);
    if (aircraft == nullptr) {
cout << "Flight not found!\n";
        return;
    }
    
cout << "\n=== Flight Information ===\n";
cout << "Flight ID: " << aircraft->getFlightID() << "\n";
cout << "Model: " << aircraft->getModel() << "\n";
cout << "Origin: " << aircraft->getOrigin() << "\n";
cout << "Destination: " << aircraft->getDestination() << "\n";
cout << "Fuel Level: " << aircraft->getFuelLevel() << "%\n";
cout << "Priority: " << aircraft->getPriorityString() << "\n";
cout << "Type: " << aircraft->getTypeString() << "\n";
cout << "Position: [" << aircraft->getCurrentX() << "," << aircraft->getCurrentY() << "]\n";
    
    GraphNode* node = airspace->getNode(aircraft->getCurrentNodeID());
    if (node) {
cout << "Current Location: " << node->name << "\n";
    }
    
cout << "Status: " << (aircraft->getIsLanded() ? "LANDED" : "IN FLIGHT") << "\n";
}

void SkyNet::printLog() {
    flightLogs->printInOrder();
cout << "\nPress Enter to continue...";
cin.ignore();
cin.get();
}

void SkyNet::findSafeRoute() {
    char flightID[100];
cout << "\n=== Find Safe Route ===\n";
cout << "Enter Flight ID: ";
cin >> flightID;
    
    Aircraft* aircraft = aircraftRegistry->search(flightID);
    if (aircraft == nullptr) {
cout << "Error: Flight not found!\n";
        return;
    }
    
    int currentNode = aircraft->getCurrentNodeID();
    if (currentNode == -1) {
cout << "Error: Aircraft not in airspace!\n";
        return;
    }
    
    Graph::PathResult* result = airspace->findShortestPathToNearestAirport(currentNode);
    
    if (result == nullptr || result->pathLength == 0) {
cout << "Error: No route to airport found!\n";
        delete result;
        return;
    }
    
cout << "\n=== Safe Route to Nearest Airport ===\n";
cout << "Total Distance: " << result->totalDistance << " km\n";
cout << "Path: ";
    
    for (int i = 0; i < result->pathLength; i++) {
        GraphNode* node = airspace->getNode(result->path[i]);
        if (node) {
cout << node->name;
            if (i < result->pathLength - 1) {
cout << " -> ";
            }
        }
    }
cout << "\n";
    
    delete result;
}

void SkyNet::moveAircraft() {
    char flightID[100];
    int targetNode;
    
cout << "\n=== Move Aircraft ===\n";
cout << "Enter Flight ID: ";
cin >> flightID;
    
    Aircraft* aircraft = aircraftRegistry->search(flightID);
    if (aircraft == nullptr) {
cout << "Error: Flight not found!\n";
        return;
    }
    
    if (aircraft->getIsLanded()) {
cout << "Error: Aircraft has already landed!\n";
        return;
    }
    
    int currentNode = aircraft->getCurrentNodeID();
    if (currentNode == -1) {
cout << "Error: Aircraft not in airspace!\n";
        return;
    }
    
cout << "Current location: ";
    GraphNode* current = airspace->getNode(currentNode);
    if (current) {
cout << current->name << "\n";
    }
    
cout << "Enter target node ID: ";
cin >> targetNode;
    
    // Collision avoidance check
    if (airspace->isNodeOccupied(targetNode)) {
        Aircraft* blockingAircraft = airspace->getAircraftAtNode(targetNode);
cout << "\n*** COLLISION ALERT! ***\n";
cout << "Target node is occupied by: " << blockingAircraft->getFlightID() << "\n";
cout << "Movement blocked. Aircraft held at current position.\n";
        return;
    }
    
    // Check if there's a path
    Graph::PathResult* path = airspace->findShortestPath(currentNode, targetNode);
    if (path == nullptr || path->pathLength == 0) {
cout << "Error: No valid path to target node!\n";
        delete path;
        return;
    }
    
    // Move aircraft
    airspace->removeAircraft(currentNode);
    if (airspace->placeAircraft(targetNode, aircraft)) {
cout << "Aircraft " << flightID << " moved successfully to node " << targetNode << "\n";
        
        // Consume some fuel
        aircraft->updateFuel(-2.0);  // Consume 2% fuel per move
        
        if (aircraft->getFuelLevel() < 10.0 && aircraft->getPriority() != Priority::CRITICAL) {
            aircraft->setPriority(Priority::HIGH);
            landingQueue->updatePriority(flightID, Priority::HIGH);
cout << "Warning: Low fuel! Priority upgraded to HIGH.\n";
        }
    } else {
cout << "Error: Could not move aircraft!\n";
        airspace->placeAircraft(currentNode, aircraft);  // Restore to original position
    }
    
    delete path;
}

void SkyNet::saveState() {
    cout << "\n=== Save State ===\n";
    
    ofstream file("skynet_save.txt");
    if (!file.is_open()) {
        cout << "Error: Could not create save file!\n";
        return;
    }
    
    // Save aircraft registry
    int aircraftCount;
    Aircraft** allAircraft = aircraftRegistry->getAllAircraft(aircraftCount);
    file << "REGISTRY\n" << aircraftCount << "\n";
    
    for (int i = 0; i < aircraftCount; i++) {
        Aircraft* ac = allAircraft[i];
        file << ac->getFlightID() << "|"
             << ac->getModel() << "|"
             << ac->getOrigin() << "|"
             << ac->getDestination() << "|"
             << ac->getFuelLevel() << "|"
             << int(ac->getPriority()) << "|"
             << int(ac->getType()) << "|"
             << ac->getCurrentX() << "|"
             << ac->getCurrentY() << "|"
             << ac->getCurrentNodeID() << "|"
             << (ac->getIsLanded() ? 1 : 0) << "|"
             << ac->getArrivalTimestamp() << "\n";
    }
    
    delete[] allAircraft;
    
    // Save flight logs
    file << "LOGS\n";
    file.close();
    flightLogs->saveToFile("skynet_logs.txt");
    
    cout << "State saved successfully!\n";
}

void SkyNet::loadState() {
    cout << "\n=== Load State ===\n";
    
    ifstream file("skynet_save.txt");
    if (!file.is_open()) {
        cout << "No save file found. Starting fresh.\n";
        return;
    }
    
    string section;
    file >> section;
    file.ignore();  // Skip newline
    
    if (section == "REGISTRY") {
        int count;
        file >> count;
        file.ignore();  // Skip newline
        
        char flightID[100], model[100], origin[100], destination[100];
        double fuel;
        int priority, type, x, y, nodeID, landed;
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
            file >> x >> delimiter;
            file >> y >> delimiter;
            file >> nodeID >> delimiter;
            file >> landed >> delimiter;
            file >> timestamp;
            file.ignore();  // Skip newline
            
            Aircraft* ac = createAircraft(flightID, model, origin, destination, fuel,
                                         (Priority)priority, (AircraftType)type);
            ac->setPosition(x, y);
            ac->setCurrentNodeID(nodeID);
            ac->setLanded(landed == 1);
            ac->setArrivalTimestamp(timestamp);
            
            aircraftRegistry->insert(flightID, ac);
            if (!ac->getIsLanded()) {
                airspace->placeAircraft(nodeID, ac);
                landingQueue->insert(ac);
            }
        }
    }
    
    file.close();
    flightLogs->loadFromFile("skynet_logs.txt");
    
    cout << "State loaded successfully!\n";
}

void SkyNet::run() {
    loadState();
    
    int choice;
    bool running = true;
    
    while (running) {
        system("cls");
        
cout << "\n";
cout << "╔══════════════════════════════════════════════════════════╗\n";
cout << "║              SKYNET AIR TRAFFIC CONTROL                 ║\n";
cout << "║                    SIMULATOR                             ║\n";
cout << "╚══════════════════════════════════════════════════════════╝\n";
cout << "\n";
        
cout << "1. Airspace Visualizer (Radar)\n";
cout << "2. Flight Control\n";
cout << "3. Search & Reporting\n";
cout << "4. System\n";
cout << "5. Exit\n";
cout << "\nChoice: ";
cin >> choice;
        
        switch (choice) {
            case 1: {
                int subChoice;
cout << "\n1. Display Radar\n";
cout << "2. Show Landing Queue\n";
cout << "Choice: ";
cin >> subChoice;
                
                if (subChoice == 1) {
                    displayRadar();
                } else if (subChoice == 2) {
                    landingQueue->printHeap();
cout << "\nPress Enter to continue...";
cin.ignore();
cin.get();
                }
                break;
            }
            case 2: {
                int subChoice;
cout << "\n1. Add Flight\n";
cout << "2. Declare Emergency\n";
cout << "3. Land Flight\n";
cout << "4. Move Aircraft\n";
cout << "Choice: ";
cin >> subChoice;
                
                if (subChoice == 1) {
                    addFlight();
                } else if (subChoice == 2) {
                    declareEmergency();
                } else if (subChoice == 3) {
                    landFlight();
                } else if (subChoice == 4) {
                    moveAircraft();
                }
                
cout << "\nPress Enter to continue...";
cin.ignore();
cin.get();
                break;
            }
            case 3: {
                int subChoice;
cout << "\n1. Search Flight\n";
cout << "2. Print Flight Log\n";
cout << "3. Find Safe Route\n";
cout << "Choice: ";
cin >> subChoice;
                
                if (subChoice == 1) {
                    searchFlight();
                } else if (subChoice == 2) {
                    printLog();
                } else if (subChoice == 3) {
                    findSafeRoute();
                }
                
cout << "\nPress Enter to continue...";
cin.ignore();
cin.get();
                break;
            }
            case 4: {
                int subChoice;
cout << "\n1. Save State\n";
cout << "2. Load State\n";
cout << "Choice: ";
cin >> subChoice;
                
                if (subChoice == 1) {
                    saveState();
                } else if (subChoice == 2) {
                    loadState();
                }
                
cout << "\nPress Enter to continue...";
cin.ignore();
cin.get();
                break;
            }
            case 5:
                saveState();
                running = false;
cout << "\nThank you for using SkyNet ATC!\n";
                break;
            default:
cout << "Invalid choice!\n";
cin.ignore();
cin.get();
        }
    }
}


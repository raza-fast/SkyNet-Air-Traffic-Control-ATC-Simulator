#include "Aircraft.h"
#include <cstdlib>
using namespace std;

Aircraft::Aircraft() : flightID(nullptr), model(nullptr), origin(nullptr), 
                       destination(nullptr), fuelLevel(100.0), priority(Priority::MEDIUM),
                       type(AircraftType::COMMERCIAL), currentX(0), currentY(0),
                       currentNodeID(-1), isLanded(false), isCrashed(false),
                       arrivalTimestamp(0) {
}

Aircraft::Aircraft(const char* id, const char* mdl, const char* orig, const char* dest,
                   double fuel, Priority prio, AircraftType tp)
    : fuelLevel(fuel), priority(prio), type(tp), currentX(0), currentY(0),
      currentNodeID(-1), isLanded(false), isCrashed(false), arrivalTimestamp(0) {
    
    flightID = new char[strlen(id) + 1];
    strcpy(flightID, id);
    
    model = new char[strlen(mdl) + 1];
    strcpy(model, mdl);
    
    origin = new char[strlen(orig) + 1];
    strcpy(origin, orig);
    
    destination = new char[strlen(dest) + 1];
    strcpy(destination, dest);
}

Aircraft::Aircraft(const Aircraft& other) {
    flightID = new char[strlen(other.flightID) + 1];
    strcpy(flightID, other.flightID);
    
    model = new char[strlen(other.model) + 1];
    strcpy(model, other.model);
    
    origin = new char[strlen(other.origin) + 1];
    strcpy(origin, other.origin);
    
    destination = new char[strlen(other.destination) + 1];
    strcpy(destination, other.destination);
    
    fuelLevel = other.fuelLevel;
    priority = other.priority;
    type = other.type;
    currentX = other.currentX;
    currentY = other.currentY;
    currentNodeID = other.currentNodeID;
    isLanded = other.isLanded;
    isCrashed = other.isCrashed;
    arrivalTimestamp = other.arrivalTimestamp;
}

Aircraft::~Aircraft() {
    delete[] flightID;
    delete[] model;
    delete[] origin;
    delete[] destination;
}

Aircraft& Aircraft::operator=(const Aircraft& other) {
    if (this != &other) {
        delete[] flightID;
        delete[] model;
        delete[] origin;
        delete[] destination;
        
        flightID = new char[strlen(other.flightID) + 1];
        strcpy(flightID, other.flightID);
        
        model = new char[strlen(other.model) + 1];
        strcpy(model, other.model);
        
        origin = new char[strlen(other.origin) + 1];
        strcpy(origin, other.origin);
        
        destination = new char[strlen(other.destination) + 1];
        strcpy(destination, other.destination);
        
        fuelLevel = other.fuelLevel;
        priority = other.priority;
        type = other.type;
        currentX = other.currentX;
        currentY = other.currentY;
        currentNodeID = other.currentNodeID;
        isLanded = other.isLanded;
        isCrashed = other.isCrashed;
        arrivalTimestamp = other.arrivalTimestamp;
    }
    return *this;
}

void Aircraft::setFlightID(const char* id) {
    delete[] flightID;
    flightID = new char[strlen(id) + 1];
    strcpy(flightID, id);
}

void Aircraft::setModel(const char* mdl) {
    delete[] model;
    model = new char[strlen(mdl) + 1];
    strcpy(model, mdl);
}

void Aircraft::setOrigin(const char* orig) {
    delete[] origin;
    origin = new char[strlen(orig) + 1];
    strcpy(origin, orig);
}

void Aircraft::setDestination(const char* dest) {
    delete[] destination;
    destination = new char[strlen(dest) + 1];
    strcpy(destination, dest);
}

void Aircraft::setFuelLevel(double fuel) {
    fuelLevel = fuel;
    if (fuelLevel < 0) fuelLevel = 0;
    if (fuelLevel > 100) fuelLevel = 100;
    
    // Auto-update priority if fuel is low
    if (fuelLevel < 10.0 && priority != Priority::CRITICAL) {
        priority = Priority::HIGH;
    }
}

void Aircraft::setPriority(Priority prio) {
    priority = prio;
}

void Aircraft::setType(AircraftType tp) {
    type = tp;
}

void Aircraft::setPosition(int x, int y) {
    currentX = x;
    currentY = y;
}

void Aircraft::setCurrentNodeID(int nodeID) {
    currentNodeID = nodeID;
}

void Aircraft::setLanded(bool landed) {
    isLanded = landed;
    if (landed) {
        // Use a simple counter instead of time
        static long long timestampCounter = 1;
        arrivalTimestamp = timestampCounter++;
    }
}

void Aircraft::setCrashed(bool crashed) {
    isCrashed = crashed;
}

void Aircraft::setArrivalTimestamp(long long timestamp) {
    arrivalTimestamp = timestamp;
}

void Aircraft::updateFuel(double delta) {
    fuelLevel += delta;
    if (fuelLevel < 0) fuelLevel = 0;
    if (fuelLevel > 100) fuelLevel = 100;
    
    // Auto-update priority if fuel is low
    if (fuelLevel < 10.0 && priority != Priority::CRITICAL) {
        priority = Priority::HIGH;
    }
}

void Aircraft::declareEmergency() {
    priority = Priority::CRITICAL;
}

bool Aircraft::needsEmergencyLanding() const {
    return priority == Priority::CRITICAL || fuelLevel < 10.0;
}

const char* Aircraft::getPriorityString() const {
    switch (priority) {
        case Priority::CRITICAL: return "CRITICAL";
        case Priority::HIGH: return "HIGH";
        case Priority::MEDIUM: return "MEDIUM";
        case Priority::LOW: return "LOW";
        default: return "UNKNOWN";
    }
}

const char* Aircraft::getTypeString() const {
    switch (type) {
        case AircraftType::COMMERCIAL: return "Commercial";
        case AircraftType::CARGO: return "Cargo";
        case AircraftType::PRIVATE: return "Private";
        case AircraftType::EMERGENCY: return "Emergency";
        default: return "Unknown";
    }
}


#ifndef AIRCRAFT_H
#define AIRCRAFT_H

#include <cstring>

// Aircraft status/priority levels
enum class Priority {
    CRITICAL = 1,  // Medical Emergency, Engine Failure, Hijack
    HIGH = 2,      // Low Fuel (< 10%)
    MEDIUM = 3,    // Commercial Passenger Flight
    LOW = 4        // Cargo / Private Jets
};

// Aircraft types
enum class AircraftType {
    COMMERCIAL,
    CARGO,
    PRIVATE,
    EMERGENCY
};

class Aircraft {
private:
    char* flightID;
    char* model;
    char* origin;
    char* destination;
    double fuelLevel;      // Percentage (0-100)
    Priority priority;
    AircraftType type;
    int currentX, currentY;  // Position on radar grid
    int currentNodeID;       // Current node in graph
    bool isLanded;
    bool isCrashed;
    long long arrivalTimestamp;  // For AVL tree sorting

public:
    // Constructors
    Aircraft();
    Aircraft(const char* id, const char* mdl, const char* orig, const char* dest, 
             double fuel, Priority prio, AircraftType tp);
    Aircraft(const Aircraft& other);
    ~Aircraft();

    // Assignment operator
    Aircraft& operator=(const Aircraft& other);

    // Getters
    const char* getFlightID() const { return flightID; }
    const char* getModel() const { return model; }
    const char* getOrigin() const { return origin; }
    const char* getDestination() const { return destination; }
    double getFuelLevel() const { return fuelLevel; }
    Priority getPriority() const { return priority; }
    AircraftType getType() const { return type; }
    int getCurrentX() const { return currentX; }
    int getCurrentY() const { return currentY; }
    int getCurrentNodeID() const { return currentNodeID; }
    bool getIsLanded() const { return isLanded; }
    bool getIsCrashed() const { return isCrashed; }
    long long getArrivalTimestamp() const { return arrivalTimestamp; }

    // Setters
    void setFlightID(const char* id);
    void setModel(const char* mdl);
    void setOrigin(const char* orig);
    void setDestination(const char* dest);
    void setFuelLevel(double fuel);
    void setPriority(Priority prio);
    void setType(AircraftType tp);
    void setPosition(int x, int y);
    void setCurrentNodeID(int nodeID);
    void setLanded(bool landed);
    void setCrashed(bool crashed);
    void setArrivalTimestamp(long long timestamp);

    // Utility
    void updateFuel(double delta);
    void declareEmergency();
    bool needsEmergencyLanding() const;
    const char* getPriorityString() const;
    const char* getTypeString() const;
};

#endif // AIRCRAFT_H


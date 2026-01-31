#ifndef GRAPH_H
#define GRAPH_H

#include <cstring>

// Forward declaration
class Aircraft;

// Edge structure for adjacency list
struct Edge {
    int destination;
    double weight;  // Distance in km or fuel cost
    Edge* next;
    
    Edge(int dest, double w) : destination(dest), weight(w), next(nullptr) {}
};

// Node structure for graph
struct GraphNode {
    int nodeID;
    char* name;
    bool isAirport;  // true for airports, false for waypoints
    int gridX, gridY;  // Position on radar grid
    Aircraft* aircraft;  // Aircraft currently at this node (nullptr if empty)
    Edge* edges;  // Linked list of edges
    
    GraphNode(int id, const char* n, bool airport, int x, int y)
        : nodeID(id), isAirport(airport), gridX(x), gridY(y), aircraft(nullptr), edges(nullptr) {
        name = new char[strlen(n) + 1];
        strcpy(name, n);
    }
    
    ~GraphNode() {
        delete[] name;
        Edge* current = edges;
        while (current) {
            Edge* next = current->next;
            delete current;
            current = next;
        }
    }
};

// Graph class (Adjacency List implementation)
class Graph {
private:
    GraphNode** nodes;  // Array of node pointers
    int maxNodes;
    int nodeCount;
    
public:
    Graph(int maxSize = 100);
    ~Graph();
    
    // Node operations
    int addNode(const char* name, bool isAirport, int gridX, int gridY);
    GraphNode* getNode(int nodeID);
    bool nodeExists(int nodeID);
    
    // Edge operations
    void addEdge(int from, int to, double weight);
    void removeEdge(int from, int to);
    
    // Aircraft operations
    bool placeAircraft(int nodeID, Aircraft* aircraft);
    bool removeAircraft(int nodeID);
    Aircraft* getAircraftAtNode(int nodeID);
    bool isNodeOccupied(int nodeID);
    
    // Dijkstra's algorithm for shortest path
    struct PathResult {
        int* path;
        int pathLength;
        double totalDistance;
        
        PathResult() : path(nullptr), pathLength(0), totalDistance(0.0) {}
        ~PathResult() { delete[] path; }
    };
    
    PathResult* findShortestPath(int start, int end);
    PathResult* findShortestPathToNearestAirport(int start);
    
    // Utility
    int getNodeCount() const { return nodeCount; }
    int getMaxNodes() const { return maxNodes; }
    void printGraph();
};

#endif // GRAPH_H


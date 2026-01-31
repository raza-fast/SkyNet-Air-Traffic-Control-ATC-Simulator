#include "Graph.h"
#include "Aircraft.h"
#include <cstring>
#include <iostream>
using namespace std;

Graph::Graph(int maxSize) : maxNodes(maxSize), nodeCount(0) {
    nodes = new GraphNode*[maxNodes];
    for (int i = 0; i < maxNodes; i++) {
        nodes[i] = nullptr;
    }
}

Graph::~Graph() {
    for (int i = 0; i < maxNodes; i++) {
        delete nodes[i];
    }
    delete[] nodes;
}

int Graph::addNode(const char* name, bool isAirport, int gridX, int gridY) {
    if (nodeCount >= maxNodes) {
        return -1;  // Graph is full
    }
    
    int newNodeID = nodeCount;
    nodes[nodeCount] = new GraphNode(newNodeID, name, isAirport, gridX, gridY);
    nodeCount++;
    return newNodeID;
}

GraphNode* Graph::getNode(int nodeID) {
    if (nodeID < 0 || nodeID >= nodeCount) {
        return nullptr;
    }
    return nodes[nodeID];
}

bool Graph::nodeExists(int nodeID) {
    return nodeID >= 0 && nodeID < nodeCount && nodes[nodeID] != nullptr;
}

void Graph::addEdge(int from, int to, double weight) {
    if (!nodeExists(from) || !nodeExists(to)) {
        return;
    }
    
    GraphNode* fromNode = nodes[from];
    Edge* newEdge = new Edge(to, weight);
    newEdge->next = fromNode->edges;
    fromNode->edges = newEdge;
}

void Graph::removeEdge(int from, int to) {
    if (!nodeExists(from)) {
        return;
    }
    
    GraphNode* fromNode = nodes[from];
    Edge* current = fromNode->edges;
    Edge* prev = nullptr;
    
    while (current) {
        if (current->destination == to) {
            if (prev) {
                prev->next = current->next;
            } else {
                fromNode->edges = current->next;
            }
            delete current;
            return;
        }
        prev = current;
        current = current->next;
    }
}

bool Graph::placeAircraft(int nodeID, Aircraft* aircraft) {
    if (!nodeExists(nodeID) || isNodeOccupied(nodeID)) {
        return false;
    }
    
    nodes[nodeID]->aircraft = aircraft;
    if (aircraft) {
        aircraft->setCurrentNodeID(nodeID);
        aircraft->setPosition(nodes[nodeID]->gridX, nodes[nodeID]->gridY);
    }
    return true;
}

bool Graph::removeAircraft(int nodeID) {
    if (!nodeExists(nodeID)) {
        return false;
    }
    
    nodes[nodeID]->aircraft = nullptr;
    return true;
}

Aircraft* Graph::getAircraftAtNode(int nodeID) {
    if (!nodeExists(nodeID)) {
        return nullptr;
    }
    return nodes[nodeID]->aircraft;
}

bool Graph::isNodeOccupied(int nodeID) {
    if (!nodeExists(nodeID)) {
        return false;
    }
    return nodes[nodeID]->aircraft != nullptr;
}

Graph::PathResult* Graph::findShortestPath(int start, int end) {
    if (!nodeExists(start) || !nodeExists(end)) {
        return nullptr;
    }
    
    // Dijkstra's algorithm
    const double INF = 1e9;  // Large value instead of INT_MAX
    double* distances = new double[nodeCount];
    int* previous = new int[nodeCount];
    bool* visited = new bool[nodeCount];
    
    for (int i = 0; i < nodeCount; i++) {
        distances[i] = INF;
        previous[i] = -1;
        visited[i] = false;
    }
    
    distances[start] = 0.0;
    
    for (int count = 0; count < nodeCount; count++) {
        // Find unvisited node with minimum distance
        int u = -1;
        double minDist = INF;
        for (int i = 0; i < nodeCount; i++) {
            if (!visited[i] && distances[i] < minDist) {
                minDist = distances[i];
                u = i;
            }
        }
        
        if (u == -1 || u == end) break;
        visited[u] = true;
        
        // Update distances to neighbors
        Edge* edge = nodes[u]->edges;
        while (edge) {
            int v = edge->destination;
            if (!visited[v] && distances[u] != INF) {
                double alt = distances[u] + edge->weight;
                if (alt < distances[v]) {
                    distances[v] = alt;
                    previous[v] = u;
                }
            }
            edge = edge->next;
        }
    }
    
    // Reconstruct path
    PathResult* result = new PathResult();
    if (distances[end] == INF) {
        delete[] distances;
        delete[] previous;
        delete[] visited;
        return result;  // No path found
    }
    
    result->totalDistance = distances[end];
    
    // Count path length
    int pathLen = 0;
    int current = end;
    while (current != -1) {
        pathLen++;
        current = previous[current];
    }
    
    result->pathLength = pathLen;
    result->path = new int[pathLen];
    
    // Build path array
    current = end;
    for (int i = pathLen - 1; i >= 0; i--) {
        result->path[i] = current;
        current = previous[current];
    }
    
    delete[] distances;
    delete[] previous;
    delete[] visited;
    
    return result;
}

Graph::PathResult* Graph::findShortestPathToNearestAirport(int start) {
    if (!nodeExists(start)) {
        return nullptr;
    }
    
    // Find all airports
    int* airports = new int[nodeCount];
    int airportCount = 0;
    
    for (int i = 0; i < nodeCount; i++) {
        if (nodes[i] && nodes[i]->isAirport) {
            airports[airportCount++] = i;
        }
    }
    
    if (airportCount == 0) {
        delete[] airports;
        return nullptr;
    }
    
    // Find shortest path to any airport
    const double INF = 1e9;
    PathResult* bestResult = nullptr;
    double bestDistance = INF;
    
    for (int i = 0; i < airportCount; i++) {
        PathResult* result = findShortestPath(start, airports[i]);
        if (result && result->pathLength > 0 && result->totalDistance < bestDistance) {
            if (bestResult) delete bestResult;
            bestResult = result;
            bestDistance = result->totalDistance;
        } else if (result) {
            delete result;
        }
    }
    
    delete[] airports;
    return bestResult;
}

void Graph::printGraph() {
    cout << "\n=== Graph Structure ===\n";
    for (int i = 0; i < nodeCount; i++) {
        if (nodes[i]) {
            cout << "Node " << i << ": " << nodes[i]->name 
                      << " (" << (nodes[i]->isAirport ? "Airport" : "Waypoint") << ")"
                      << " at [" << nodes[i]->gridX << "," << nodes[i]->gridY << "]";
            if (nodes[i]->aircraft) {
                cout << " - Aircraft: " << nodes[i]->aircraft->getFlightID();
            }
            cout << "\n";
            
            Edge* edge = nodes[i]->edges;
            while (edge) {
                cout << "  -> Node " << edge->destination 
                          << " (weight: " << edge->weight << ")\n";
                edge = edge->next;
            }
        }
    }
}


#include "Radar.h"
#include <iostream>
#include <cstdlib>
using namespace std;

Radar::Radar(Graph* graph) : airspace(graph) {
    grid = new char*[GRID_SIZE];
    for (int i = 0; i < GRID_SIZE; i++) {
        grid[i] = new char[GRID_SIZE];
        for (int j = 0; j < GRID_SIZE; j++) {
            grid[i][j] = '.';
        }
    }
    updateGrid();
}

Radar::~Radar() {
    for (int i = 0; i < GRID_SIZE; i++) {
        delete[] grid[i];
    }
    delete[] grid;
}

void Radar::clearGrid() {
    for (int i = 0; i < GRID_SIZE; i++) {
        for (int j = 0; j < GRID_SIZE; j++) {
            grid[i][j] = '.';
        }
    }
}

void Radar::updateGrid() {
    clearGrid();
    
    if (airspace == nullptr) {
        return;
    }
    
    // Mark airports and waypoints
    for (int i = 0; i < airspace->getNodeCount(); i++) {
        GraphNode* node = airspace->getNode(i);
        if (node) {
            int x = node->gridX;
            int y = node->gridY;
            
            // Ensure coordinates are within grid bounds
            if (x >= 0 && x < GRID_SIZE && y >= 0 && y < GRID_SIZE) {
                if (node->isAirport) {
                    grid[y][x] = 'A';
                } else {
                    // Waypoints shown as empty, but we can mark them if needed
                    if (grid[y][x] == '.') {
                        grid[y][x] = ' ';  // Waypoint (invisible on radar)
                    }
                }
                
                // Mark aircraft
                if (node->aircraft != nullptr) {
                    grid[y][x] = 'P';
                }
            }
        }
    }
}

void Radar::display() {
    updateGrid();
    
cout << "\n";
cout << "╔══════════════════════════════════════════════════════════╗\n";
cout << "║                    SKYNET RADAR VIEW                     ║\n";
cout << "╚══════════════════════════════════════════════════════════╝\n";
cout << "\n";
cout << "Legend: A = Airport, P = Plane, . = Empty Sky\n";
cout << "\n";
    
    // Print column numbers
cout << "   ";
    for (int j = 0; j < GRID_SIZE; j++) {
cout << (j % 10);
    }
cout << "\n";
    
    // Print grid with row numbers
    for (int i = 0; i < GRID_SIZE; i++) {
        printf("%2d ", i);
        for (int j = 0; j < GRID_SIZE; j++) {
cout << grid[i][j];
        }
cout << "\n";
    }
cout << "\n";
}

void Radar::refresh() {
    system("cls");
    display();
}


#ifndef RADAR_H
#define RADAR_H

#include "Graph.h"

// Radar visualization system (2D grid)
class Radar {
private:
    static const int GRID_SIZE = 20;
    char** grid;
    Graph* airspace;
    
    void clearGrid();
    void updateGrid();
    
public:
    Radar(Graph* graph);
    ~Radar();
    
    void display();
    void refresh();  // Clear screen and redraw
    int getGridSize() const { return GRID_SIZE; }
};

#endif // RADAR_H


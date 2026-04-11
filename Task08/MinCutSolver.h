#ifndef MINCUTSOLVER_H
#define MINCUTSOLVER_H

#include <vector>
#include "Graph.h"
using namespace std;

class MinCutSolver {
private:
    Graph& graph;  // Reference to a Graph object

public:
    // Constructor
    MinCutSolver(Graph& g);

    // Calculate cut value for a partition
    int calculateCut(const vector<int>& partition);

    int bruteForceMinCut();

    int iterativeImprovement();

    // Test function
    void testCut();
};

#endif
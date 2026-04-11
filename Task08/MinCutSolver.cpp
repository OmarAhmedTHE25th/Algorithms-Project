#include "MinCutSolver.h"
#include <iostream>
#include "Graph.h"

MinCutSolver::MinCutSolver(Graph& g) : graph(g) {}

int MinCutSolver::calculateCut(const vector<int>& partition) {
    int n = graph.getVertices();
    int cutWeight = 0;

    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            if (partition[i] != partition[j]) {
                cutWeight += graph.getWeight(i, j);
            }
        }
    }

    return cutWeight;
}

void MinCutSolver::testCut() {
    int n = graph.getVertices();
    vector<int> partition(n);

    // Alternate vertices between 0 and 1
    for (int i = 0; i < n; i++)
        partition[i] = (i < n / 2) ? 0 : 1;

    std::cout << "\n[Test Partition]\nPartition: ";
    for (int set : partition) std::cout << set << " ";
    std::cout << "\nCut Value = " << calculateCut(partition) << std::endl;
}
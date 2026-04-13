#include "MinCutSolver.h"
#include <iostream>
#include "Graph.h"
#include <climits>

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

int MinCutSolver::bruteForceMinCut() {
    int n = graph.getVertices();
    int minCut = INT_MAX;

    // Generate all possible non-empty partitions
    for (int mask = 1; mask < (1 << n) - 1; mask++) {
        vector<int> partition(n);

        // Convert bitmask to partition
        for (int i = 0; i < n; i++) {
            partition[i] = (mask >> i) & 1;
        }

        int currentCut = calculateCut(partition);

        if (currentCut < minCut) {
            minCut = currentCut;
        }
    }

    return minCut;
}

int MinCutSolver::iterativeImprovement() {
    int n = graph.getVertices();

    // Initial partition: first half 0, second half 1
    vector<int> partition(n);
    for (int i = 0; i < n; i++) {
        partition[i] = (i < n / 2) ? 0 : 1;
    }

    int bestCut = calculateCut(partition);
    bool improved = true;

    while (improved) {
        improved = false;

        for (int i = 0; i < n; i++) {
            // Count current sets
            int count0 = 0, count1 = 0;
            for (int x : partition) {
                if (x == 0) count0++;
                else count1++;
            }

            // Prevent empty set after move
            if ((partition[i] == 0 && count0 == 1) ||
                (partition[i] == 1 && count1 == 1)) {
                continue;
            }

            // Try flipping
            partition[i] = 1 - partition[i];
            int newCut = calculateCut(partition);

            if (newCut < bestCut) {
                bestCut = newCut;
                improved = true;
            }
            else {
                // Undo bad move
                partition[i] = 1 - partition[i];
            }
        }
    }

    return bestCut;
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
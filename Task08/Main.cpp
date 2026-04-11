
#include <iostream>
#include "Graph.h"
#include "MinCutSolver.h"

int main() {
    int n;
    std::cout << "Enter number of vertices: ";
    std::cin >> n;

    // Create Graph object
    Graph g(n);
    g.inputGraph();
    g.displayGraph();

    // Create Solver object
    MinCutSolver solver(g);

    cout << "  Brute Force Result" << endl;
    cout << "Minimum Cut = " << solver.bruteForceMinCut() << endl;

    cout << " Iterative Improvement Result" << endl;
    cout << "Approximate Minimum Cut = "
        << solver.iterativeImprovement() << endl;

    // Test a sample cut
    solver.testCut();

    return 0;
}

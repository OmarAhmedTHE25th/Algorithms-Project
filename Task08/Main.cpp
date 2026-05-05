
#include <iostream>
#include "Graph.h"
#include "MinCutSolver.h"

using namespace std;

int main() {
    int n, choice;

    cout << "==================================" << endl;
    cout << "   CSE245 Task 8 - Min Cut UI" << endl;
    cout << "==================================" << endl;
    cout << "Enter number of vertices: ";
    cin >> n;

    Graph g(n);
    g.inputGraph();

    MinCutSolver solver(g);

    do {
        cout << "\n========= MENU =========" << endl;
        cout << "1. Display Graph" << endl;
        cout << "2. Brute Force Minimum Cut" << endl;
        cout << "3. Iterative Improvement" << endl;
        cout << "4. Greedy " << endl;
        cout << "5. Exit " << endl;
        cout << "Choose: ";
        cin >> choice;

        switch (choice) {
        case 1:
            g.displayGraph();
            break;

        case 2:
            cout << "\nBrute Force Result" << endl;
            cout << "Minimum Cut = "
                << solver.bruteForceMinCut() << endl;
            break;

        case 3:
            cout << "\nIterative Improvement Result" << endl;
            cout << "Approximate Minimum Cut = "
                << solver.iterativeImprovement() << endl;
            break;

        case 4:
            cout << "\nGreedy Algorithm Result" << endl;
            cout << "Greedy Cut = "
                << solver.greedyMinCut() << endl;
            break;

        case 5:
            cout << "Exiting program..." << endl;
            break;

        default:
            cout << "Invalid choice!" << endl;
        }

    } while (choice != 5);

    return 0;
}

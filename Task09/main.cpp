#include <iostream>
#include <iomanip>
#include <vector>
#include "point.hpp"
#include "clustering_common.hpp"
#include "brute_force.hpp"
#include "divide_and_conquer.hpp"
#include "iterative_improvement.hpp"

using namespace std;

void printResult(const string& title, const ClusterResult& result, int k) {
    cout << "\n=== " << title << " ===\n";
    cout << fixed << setprecision(4);
    cout << "Cost (WCSS) : " << result.cost << "\n";
    for (int c = 0; c < k; c++) {
        cout << "  Cluster " << c << ": points [ ";
        for (int i = 0; i < (int)result.assignment.size(); i++)
            if (result.assignment[i] == c) cout << i << " ";
        cout << "]\n";
    }
}

int main() {
    int n, k;
    cout << "========================================\n";
    cout << "  Task 9 - K-Clustering (CSE245 S2026)\n";
    cout << "========================================\n";
    cout << "Enter number of points: ";
    cin >> n;
    cout << "Enter number of clusters k: ";
    cin >> k;

    vector<Point> points(n);
    cout << "Enter " << n << " points as  x y  (one per line):\n";
    for (int i = 0; i < n; i++) {
        cout << "  Point " << i << ": ";
        cin >> points[i].x >> points[i].y;
    }

    ClusterResult brRes = solveBruteForce(points, k);
    ClusterResult dcRes = solveDivideAndConquer(points, k);
    ClusterResult iiRes = solveIterativeImprovement(points, k);

    printResult("Brute Force",           brRes, k);
    printResult("Divide and Conquer",    dcRes, k);
    printResult("Iterative Improvement", iiRes, k);

    cout << "\n--- Cost Comparison ---\n";
    cout << fixed << setprecision(4);
    cout << "  Brute Force           : " << brRes.cost << "  (optimal)\n";
    cout << "  Divide and Conquer    : " << dcRes.cost << "\n";
    cout << "  Iterative Improvement : " << iiRes.cost << "\n";
    return 0;
}

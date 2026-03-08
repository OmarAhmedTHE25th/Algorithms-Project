#include <iostream>
#include <vector>
#include "point.h"
#include "clustering_common.h"
#include "brute_force.h"
#include "divide_and_conquer.h"
#include "iterative_improvement.h"

using namespace std;

double distanceSquared(const Point& a, const Point& b) {
    double dx = a.x - b.x;
    double dy = a.y - b.y;
    return dx * dx + dy * dy;
}

void printResult(const string& title, const ClusterResult& result) {
    cout << "\n=== " << title << " ===\n";
    cout << "Cost: " << result.cost << "\n";
    cout << "Assignments: ";
    for (int clusterId : result.assignment) {
        cout << clusterId << " ";
    }
    cout << "\n";
}

int main() {
    int n, k;
    cout << "Enter number of points: ";
    cin >> n;

    cout << "Enter number of clusters k: ";
    cin >> k;

    vector<Point> points(n);
    cout << "Enter points as x y:\n";
    for (int i = 0; i < n; i++) {
        cin >> points[i].x >> points[i].y;
    }

    ClusterResult bruteForceResult = solveBruteForce(points, k);
    ClusterResult divideConquerResult = solveDivideAndConquer(points, k);
    ClusterResult iterativeResult = solveIterativeImprovement(points, k);

    printResult("Brute Force", bruteForceResult);
    printResult("Divide and Conquer", divideConquerResult);
    printResult("Iterative Improvement", iterativeResult);

    return 0;
}

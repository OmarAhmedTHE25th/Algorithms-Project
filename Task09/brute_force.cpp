#include "brute_force.h"

ClusterResult solveBruteForce(const std::vector<Point>& points, int k) {
    ClusterResult result;
    result.assignment = std::vector<int>(points.size(), -1);
    result.cost = -1.0;

    // TODO:
    // 1. Generate all possible assignments of points into k clusters
    // 2. Compute clustering cost for each assignment
    // 3. Return the assignment with minimum cost

    return result;
}

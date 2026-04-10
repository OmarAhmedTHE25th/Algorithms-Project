#ifndef DIVIDE_AND_CONQUER_H
#define DIVIDE_AND_CONQUER_H

#include <vector>
#include "point.hpp"
#include "clustering_common.hpp"

// Divide-and-conquer k-clustering.
// Recursively splits the point set by median x-coordinate, solves each half,
// then merges by re-assigning every point to its nearest global centroid.
// Time complexity: O(n log n * k * iterations).
ClusterResult solveDivideAndConquer(const std::vector<Point>& points, int k);

#endif

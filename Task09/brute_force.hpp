#ifndef BRUTE_FORCE_H
#define BRUTE_FORCE_H

#include <vector>
#include "point.hpp"
#include "clustering_common.hpp"

// Brute-force k-clustering: tries every possible assignment of n points to k clusters.
// Guaranteed optimal but exponential time: O(k^n).
ClusterResult solveBruteForce(const std::vector<Point>& points, int k);

#endif

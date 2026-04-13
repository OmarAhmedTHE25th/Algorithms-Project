#ifndef BRUTE_FORCE_H
#define BRUTE_FORCE_H

#include <vector>
#include "point.hpp"
#include "clustering_common.hpp"

<<<<<<< HEAD
=======
// Brute-force k-clustering: tries every possible assignment of n points to k clusters.
// Guaranteed optimal but exponential time: O(k^n).
>>>>>>> db2e2416a07011b873dce1f152ed81f8d85529fa
ClusterResult solveBruteForce(const std::vector<Point>& points, int k);

#endif

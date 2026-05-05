#ifndef BRUTE_FORCE_H
#define BRUTE_FORCE_H

#include <vector>
#include "point.hpp"
#include "clustering_common.hpp"

// tries every possible assignment, always gives best answer but very slow (O(k^n))
ClusterResult solveBruteForce(const std::vector<Point>& points, int k);

#endif

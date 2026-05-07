#ifndef BRUTE_FORCE_H
#define BRUTE_FORCE_H

#include <vector>
#include "point.hpp"
#include "clustering_common.hpp"

ClusterResult solveBruteForce(const std::vector<Point>& points, int k);

#endif

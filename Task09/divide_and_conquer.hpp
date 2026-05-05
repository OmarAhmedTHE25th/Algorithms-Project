#ifndef DIVIDE_AND_CONQUER_H
#define DIVIDE_AND_CONQUER_H

#include <vector>
#include "point.hpp"
#include "clustering_common.hpp"

// divide & conquer: split by median x, recurse, then fix boundaries with k-means
ClusterResult solveDivideAndConquer(const std::vector<Point>& points, int k);

#endif

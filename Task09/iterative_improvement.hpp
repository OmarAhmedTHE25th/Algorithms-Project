#ifndef ITERATIVE_IMPROVEMENT_H
#define ITERATIVE_IMPROVEMENT_H

#include <vector>
#include "point.hpp"
#include "clustering_common.hpp"

// k-means with k-means++ seeding and multiple restarts
ClusterResult solveIterativeImprovement(const std::vector<Point>& points, int k);

#endif

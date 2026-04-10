#ifndef CLUSTERING_COMMON_H
#define CLUSTERING_COMMON_H

#include <vector>
#include "point.hpp"

struct ClusterResult {
    std::vector<int> assignment;
    double cost;
};

// Returns squared Euclidean distance between two points
double distanceSquared(const Point& a, const Point& b);

#endif

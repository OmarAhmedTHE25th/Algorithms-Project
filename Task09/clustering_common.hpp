#ifndef CLUSTERING_COMMON_H
#define CLUSTERING_COMMON_H

#include <vector>
#include "point.hpp"

// stores the result of a clustering run
struct ClusterResult {
    std::vector<int> assignment; // which cluster each point belongs to
    double cost;                 // total WCSS
};

double distanceSquared(const Point& a, const Point& b);

#endif

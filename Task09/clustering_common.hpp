#ifndef CLUSTERING_COMMON_H
#define CLUSTERING_COMMON_H

#include <vector>
#include "point.h"

struct ClusterResult {
    std::vector<int> assignment;
    double cost;
};

double distanceSquared(const Point& a, const Point& b);

#endif

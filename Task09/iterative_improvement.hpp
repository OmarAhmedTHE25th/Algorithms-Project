#ifndef ITERATIVE_IMPROVEMENT_H
#define ITERATIVE_IMPROVEMENT_H

#include <vector>
#include "point.h"
#include "clustering_common.h"

ClusterResult solveIterativeImprovement(const std::vector<Point>& points, int k);

#endif

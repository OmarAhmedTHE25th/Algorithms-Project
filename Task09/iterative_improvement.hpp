#ifndef ITERATIVE_IMPROVEMENT_H
#define ITERATIVE_IMPROVEMENT_H

#include <vector>
#include "point.hpp"
#include "clustering_common.hpp"

// Iterative-improvement k-clustering (Lloyd's k-means algorithm).
// Initialises k centres with k-means++ seeding for better convergence,
// then alternates between assignment and centroid-update steps until
// no point changes cluster.  Runs multiple restarts and keeps best result.
// Average time complexity: O(n * k * iterations * restarts).
ClusterResult solveIterativeImprovement(const std::vector<Point>& points, int k);

#endif

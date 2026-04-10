#ifndef BRUTE_FORCE_H
#define BRUTE_FORCE_H

#include <vector>
#include "point.hpp"

<<<<<<< HEAD
using namespace std;
=======
ClusterResult solveBruteForce(const std::vector<Point>& points, int k);
>>>>>>> 0f9d90a (Task09: Move completed clustering files into Task09/ folder)

struct ClusterResult {
    vector<int> assignment;
    double cost;
};

// brute force clustering
ClusterResult solveBruteForce(const vector<Point>& points, int k);

#endif
#include "iterative_improvement.hpp"
#include <algorithm>
#include <limits>
#include <cmath>
#include <cstdlib>
#include <ctime>
using namespace std;

// ── helpers ───────────────────────────────────────────────────────────────────

static double dist2(const Point& a, const Point& b) {
    double dx = a.x - b.x, dy = a.y - b.y;
    return dx * dx + dy * dy;
}

// Recompute centroids; returns false if any cluster is empty (degenerate)
static bool computeCentroids(const vector<Point>& pts,
                              const vector<int>&   asgn,
                              int k,
                              vector<Point>&       centers) {
    vector<double> sx(k, 0), sy(k, 0);
    vector<int>    cnt(k, 0);
    for (int i = 0; i < (int)pts.size(); i++) {
        sx[asgn[i]] += pts[i].x;
        sy[asgn[i]] += pts[i].y;
        cnt[asgn[i]]++;
    }
    for (int c = 0; c < k; c++) {
        if (cnt[c] == 0) return false;
        centers[c] = {sx[c] / cnt[c], sy[c] / cnt[c]};
    }
    return true;
}

// Assign every point to nearest centroid; returns true if any assignment changed
static bool assignNearest(const vector<Point>& pts,
                           const vector<Point>& centers,
                           vector<int>&         asgn) {
    int k = (int)centers.size();
    bool changed = false;
    for (int i = 0; i < (int)pts.size(); i++) {
        double best = numeric_limits<double>::max();
        int    bestC = 0;
        for (int c = 0; c < k; c++) {
            double d = dist2(pts[i], centers[c]);
            if (d < best) { best = d; bestC = c; }
        }
        if (asgn[i] != bestC) { asgn[i] = bestC; changed = true; }
    }
    return changed;
}

// Total WCSS
static double totalCost(const vector<Point>& pts,
                         const vector<int>&   asgn,
                         const vector<Point>& centers) {
    double cost = 0;
    for (int i = 0; i < (int)pts.size(); i++)
        cost += dist2(pts[i], centers[asgn[i]]);
    return cost;
}

// ── k-means++ seeding ─────────────────────────────────────────────────────────
// Picks k initial centres with probability proportional to D^2 distance.
// This dramatically reduces the chance of bad local minima compared to
// pure random initialisation.
static vector<Point> kMeansPlusPlusInit(const vector<Point>& pts, int k) {
    int n = (int)pts.size();
    vector<Point> centers;
    centers.reserve(k);

    // Pick first centre uniformly at random
    centers.push_back(pts[rand() % n]);

    vector<double> d2(n);
    for (int step = 1; step < k; step++) {
        // Compute D^2 for each point to nearest chosen centre
        double sum = 0;
        for (int i = 0; i < n; i++) {
            double best = numeric_limits<double>::max();
            for (const Point& c : centers) {
                double d = dist2(pts[i], c);
                if (d < best) best = d;
            }
            d2[i] = best;
            sum += best;
        }
        // Sample next centre proportional to D^2
        double threshold = ((double)rand() / RAND_MAX) * sum;
        double cumul = 0;
        int chosen = n - 1;
        for (int i = 0; i < n; i++) {
            cumul += d2[i];
            if (cumul >= threshold) { chosen = i; break; }
        }
        centers.push_back(pts[chosen]);
    }
    return centers;
}

// ── single k-means run ────────────────────────────────────────────────────────
static ClusterResult kMeansRun(const vector<Point>& pts, int k) {
    int n = (int)pts.size();
    ClusterResult res;
    res.assignment.assign(n, 0);
    res.cost = numeric_limits<double>::max();

    // Seed centres with k-means++
    vector<Point> centers = kMeansPlusPlusInit(pts, k);

    // Initial assignment
    assignNearest(pts, centers, res.assignment);

    const int MAX_ITER = 300;
    for (int iter = 0; iter < MAX_ITER; iter++) {
        // Recompute centroids
        if (!computeCentroids(pts, res.assignment, k, centers)) break; // degenerate

        // Reassign; stop if converged
        if (!assignNearest(pts, centers, res.assignment)) break;
    }

    // Final cost
    computeCentroids(pts, res.assignment, k, centers);
    res.cost = totalCost(pts, res.assignment, centers);
    return res;
}

// ── public API ────────────────────────────────────────────────────────────────
ClusterResult solveIterativeImprovement(const vector<Point>& points, int k) {
    ClusterResult best;
    best.assignment = vector<int>(points.size(), 0);
    best.cost = numeric_limits<double>::max();

    if (k <= 0 || k > (int)points.size()) return best;

    // Seed random number generator once
    srand(42);  // fixed seed for reproducibility

    // Multiple restarts to escape local minima
    const int RESTARTS = 10;
    for (int r = 0; r < RESTARTS; r++) {
        ClusterResult res = kMeansRun(points, k);
        if (res.cost < best.cost) best = res;
    }

    return best;
}

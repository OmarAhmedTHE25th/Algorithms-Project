#include "divide_and_conquer.hpp"
#include <algorithm>
#include <limits>
#include <cmath>
using namespace std;

// ── helpers ───────────────────────────────────────────────────────────────────

static double dist2(const Point& a, const Point& b) {
    double dx = a.x - b.x, dy = a.y - b.y;
    return dx * dx + dy * dy;
}

// Recompute centroid for each cluster
static vector<Point> computeCentroids(const vector<Point>& pts,
                                       const vector<int>& asgn, int k) {
    vector<double> sx(k, 0), sy(k, 0);
    vector<int> cnt(k, 0);
    for (int i = 0; i < (int)pts.size(); i++) {
        sx[asgn[i]] += pts[i].x;
        sy[asgn[i]] += pts[i].y;
        cnt[asgn[i]]++;
    }
    vector<Point> centers(k, {0, 0});
    for (int c = 0; c < k; c++)
        if (cnt[c] > 0) centers[c] = {sx[c] / cnt[c], sy[c] / cnt[c]};
    return centers;
}

// Assign every point to its nearest centroid
static vector<int> assignToNearest(const vector<Point>& pts,
                                    const vector<Point>& centers) {
    int k = (int)centers.size();
    vector<int> asgn(pts.size());
    for (int i = 0; i < (int)pts.size(); i++) {
        double best = numeric_limits<double>::max();
        int bestC = 0;
        for (int c = 0; c < k; c++) {
            double d = dist2(pts[i], centers[c]);
            if (d < best) { best = d; bestC = c; }
        }
        asgn[i] = bestC;
    }
    return asgn;
}

// Total WCSS cost
static double totalCost(const vector<Point>& pts,
                         const vector<int>& asgn,
                         const vector<Point>& centers) {
    double cost = 0;
    for (int i = 0; i < (int)pts.size(); i++)
        cost += dist2(pts[i], centers[asgn[i]]);
    return cost;
}

// ── divide-and-conquer clustering ─────────────────────────────────────────────
//
// Strategy:
//   Base case  – 1 cluster or 1 point: assign everything to cluster 0.
//   Recursive  – Sort by x (or y), split at median, recurse on each half
//                with ceil(k/2) and floor(k/2) clusters respectively,
//                then re-label cluster ids so they don't overlap, collect
//                all centroids, and run a few k-means refinement passes
//                on the full point set to fix any cross-boundary mismatches.

// Recursive helper – works on a subset (given by index list)
// Returns assignments indexed into 'subset', cluster ids start at 'offset'
static void dcHelper(const vector<Point>& pts,
                     vector<int> indices,   // indices into pts
                     int k,                 // clusters to form
                     int offset,            // cluster-id offset
                     vector<int>& globalAsgn) {

    int n = (int)indices.size();

    // Base case: only one cluster (or single point)
    if (k == 1 || n <= k) {
        for (int i = 0; i < n; i++)
            globalAsgn[indices[i]] = offset + (n > k ? 0 : i % k);
        // If n <= k give each point its own cluster starting at offset
        if (n <= k)
            for (int i = 0; i < n; i++)
                globalAsgn[indices[i]] = offset + i;
        else
            for (int idx : indices)
                globalAsgn[idx] = offset;
        return;
    }

    // Sort indices by x-coordinate (use y to break ties)
    sort(indices.begin(), indices.end(), [&](int a, int b) {
        return pts[a].x < pts[b].x ||
               (pts[a].x == pts[b].x && pts[a].y < pts[b].y);
    });

    int mid   = n / 2;
    int kLeft  = k / 2;
    int kRight = k - kLeft;

    vector<int> left(indices.begin(), indices.begin() + mid);
    vector<int> right(indices.begin() + mid, indices.end());

    dcHelper(pts, left,  kLeft,  offset,          globalAsgn);
    dcHelper(pts, right, kRight, offset + kLeft,  globalAsgn);
}

// ── public API ────────────────────────────────────────────────────────────────
ClusterResult solveDivideAndConquer(const vector<Point>& points, int k) {
    ClusterResult res;
    int n = (int)points.size();
    res.assignment = vector<int>(n, 0);
    res.cost = numeric_limits<double>::max();

    if (k <= 0 || k > n) return res;

    // 1. Build initial assignment via divide-and-conquer
    vector<int> indices(n);
    for (int i = 0; i < n; i++) indices[i] = i;
    dcHelper(points, indices, k, 0, res.assignment);

    // 2. Refine with k-means iterations (fixes cross-boundary errors)
    const int MAX_ITER = 100;
    for (int iter = 0; iter < MAX_ITER; iter++) {
        vector<Point> centers = computeCentroids(points, res.assignment, k);
        vector<int> newAsgn   = assignToNearest(points, centers);

        // Check convergence
        bool changed = false;
        for (int i = 0; i < n; i++)
            if (newAsgn[i] != res.assignment[i]) { changed = true; break; }

        res.assignment = newAsgn;
        if (!changed) break;
    }

    // 3. Compute final cost
    vector<Point> centers = computeCentroids(points, res.assignment, k);
    res.cost = totalCost(points, res.assignment, centers);

    return res;
}

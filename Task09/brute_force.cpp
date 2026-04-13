#include "brute_force.hpp"
#include <limits>
#include <vector>
using namespace std;

<<<<<<< HEAD

=======
// ── helpers ──────────────────────────────────────────────────────────────────

// Squared Euclidean distance (used by main.cpp as well via clustering_common)
>>>>>>> db2e2416a07011b873dce1f152ed81f8d85529fa
double distanceSquared(const Point& a, const Point& b) {
    double dx = a.x - b.x, dy = a.y - b.y;
    return dx * dx + dy * dy;
}

<<<<<<< HEAD
=======
// Centroid of cluster c given current assignments
>>>>>>> db2e2416a07011b873dce1f152ed81f8d85529fa
static Point centroid(const vector<Point>& pts, const vector<int>& asgn, int c) {
    double sx = 0, sy = 0;
    int cnt = 0;
    for (int i = 0; i < (int)pts.size(); i++) {
        if (asgn[i] == c) { sx += pts[i].x; sy += pts[i].y; cnt++; }
    }
    if (cnt == 0) return {0, 0};
    return {sx / cnt, sy / cnt};
}

<<<<<<< HEAD
=======
// Check that every cluster 0..k-1 has at least one point
>>>>>>> db2e2416a07011b873dce1f152ed81f8d85529fa
static bool allNonEmpty(const vector<int>& asgn, int k) {
    vector<int> cnt(k, 0);
    for (int x : asgn) cnt[x]++;
    for (int i = 0; i < k; i++) if (cnt[i] == 0) return false;
    return true;
}

<<<<<<< HEAD
=======
// Total within-cluster sum of squared distances to centroid
>>>>>>> db2e2416a07011b873dce1f152ed81f8d85529fa
static double totalCost(const vector<Point>& pts, const vector<int>& asgn, int k) {
    if (!allNonEmpty(asgn, k)) return 1e18;
    vector<Point> centers(k);
    for (int c = 0; c < k; c++) centers[c] = centroid(pts, asgn, c);
    double cost = 0;
    for (int i = 0; i < (int)pts.size(); i++)
        cost += distanceSquared(pts[i], centers[asgn[i]]);
    return cost;
}

<<<<<<< HEAD
=======
// ── recursive DFS over all k^n assignments ───────────────────────────────────
>>>>>>> db2e2416a07011b873dce1f152ed81f8d85529fa
static void dfs(const vector<Point>& pts, int k, int idx,
                vector<int>& cur, ClusterResult& best) {
    if (idx == (int)pts.size()) {
        double c = totalCost(pts, cur, k);
        if (c < best.cost) { best.cost = c; best.assignment = cur; }
        return;
    }
    for (int c = 0; c < k; c++) {
        cur[idx] = c;
        dfs(pts, k, idx + 1, cur, best);
    }
}

// ── public API ────────────────────────────────────────────────────────────────
ClusterResult solveBruteForce(const vector<Point>& points, int k) {
    ClusterResult res;
    res.assignment = vector<int>(points.size(), -1);
    res.cost = numeric_limits<double>::max();
    if (k <= 0 || k > (int)points.size()) return res;

    vector<int> cur(points.size(), 0);
    dfs(points, k, 0, cur, res);
    return res;
}

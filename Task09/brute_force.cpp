#include "brute_force.hpp"
#include <vector>
using namespace std;

static const double INF = 1e18;

double distanceSquared(const Point& a, const Point& b) {
    double dx = a.x - b.x, dy = a.y - b.y;
    return dx * dx + dy * dy;
}

static Point centroid(const vector<Point>& pts, const vector<int>& asgn, int c) {
    double sx = 0, sy = 0;
    int cnt = 0;
    for (int i = 0; i < (int)pts.size(); i++) {
        if (asgn[i] == c) { sx += pts[i].x; sy += pts[i].y; cnt++; }
    }
    if (cnt == 0) return {0, 0};
    return {sx / cnt, sy / cnt};
}

static bool allNonEmpty(const vector<int>& asgn, int k) {
    vector<int> cnt(k, 0);
    for (int x : asgn) cnt[x]++;
    for (int i = 0; i < k; i++) if (cnt[i] == 0) return false;
    return true;
}

static double totalCost(const vector<Point>& pts, const vector<int>& asgn, int k) {
    if (!allNonEmpty(asgn, k)) return INF;
    vector<Point> centers(k);
    for (int c = 0; c < k; c++) centers[c] = centroid(pts, asgn, c);
    double cost = 0;
    for (int i = 0; i < (int)pts.size(); i++)
        cost += distanceSquared(pts[i], centers[asgn[i]]);
    return cost;
}

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

ClusterResult solveBruteForce(const vector<Point>& points, int k) {
    ClusterResult res;
    res.assignment = vector<int>(points.size(), -1);
    res.cost = INF;
    if (k <= 0 || k > (int)points.size()) return res;

    vector<int> cur(points.size(), 0);
    dfs(points, k, 0, cur, res);
    return res;
}

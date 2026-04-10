#include "brute_force.hpp"
#include <limits>
using namespace std;

<<<<<<< HEAD
// distance squared between two points
double dist2(const Point& a, const Point& b) {
    double dx = a.x - b.x;
    double dy = a.y - b.y;
    return dx * dx + dy * dy;
}

// compute centroid of cluster c
Point getCenter(const vector<Point>& points, const vector<int>& assign, int c) {
=======

double distanceSquared(const Point& a, const Point& b) {
    double dx = a.x - b.x, dy = a.y - b.y;
    return dx * dx + dy * dy;
}

static Point centroid(const vector<Point>& pts, const vector<int>& asgn, int c) {
>>>>>>> 0f9d90a (Task09: Move completed clustering files into Task09/ folder)
    double sx = 0, sy = 0;
    int cnt = 0;

    for (int i = 0; i < (int)points.size(); i++) {
        if (assign[i] == c) {
            sx += points[i].x;
            sy += points[i].y;
            cnt++;
        }
    }

    Point center{0, 0};
    if (cnt > 0) {
        center.x = sx / cnt;
        center.y = sy / cnt;
    }
    return center;
}

<<<<<<< HEAD
// check no empty clusters
bool valid(const vector<int>& assign, int k) {
=======
static bool allNonEmpty(const vector<int>& asgn, int k) {
>>>>>>> 0f9d90a (Task09: Move completed clustering files into Task09/ folder)
    vector<int> cnt(k, 0);
    for (int x : assign) cnt[x]++;
    for (int i = 0; i < k; i++)
        if (cnt[i] == 0) return false;
    return true;
}

<<<<<<< HEAD
// calculate cost
double getCost(const vector<Point>& points, const vector<int>& assign, int k) {
    if (!valid(assign, k)) return 1e18;

=======
static double totalCost(const vector<Point>& pts, const vector<int>& asgn, int k) {
    if (!allNonEmpty(asgn, k)) return 1e18;
>>>>>>> 0f9d90a (Task09: Move completed clustering files into Task09/ folder)
    vector<Point> centers(k);
    for (int c = 0; c < k; c++)
        centers[c] = getCenter(points, assign, c);

    double cost = 0;
    for (int i = 0; i < (int)points.size(); i++) {
        cost += dist2(points[i], centers[assign[i]]);
    }

    return cost;
}

<<<<<<< HEAD
// recursive brute force
void dfs(const vector<Point>& points, int k, int i,
         vector<int>& cur, ClusterResult& best) {

    if (i == (int)points.size()) {
        double cost = getCost(points, cur, k);
        if (cost < best.cost) {
            best.cost = cost;
            best.assignment = cur;
        }
=======
static void dfs(const vector<Point>& pts, int k, int idx,
                vector<int>& cur, ClusterResult& best) {
    if (idx == (int)pts.size()) {
        double c = totalCost(pts, cur, k);
        if (c < best.cost) { best.cost = c; best.assignment = cur; }
>>>>>>> 0f9d90a (Task09: Move completed clustering files into Task09/ folder)
        return;
    }

    for (int c = 0; c < k; c++) {
        cur[i] = c;
        dfs(points, k, i + 1, cur, best);
    }
}

ClusterResult solveBruteForce(const vector<Point>& points, int k) {
    ClusterResult res;
    res.assignment = vector<int>(points.size(), -1);
    res.cost = numeric_limits<double>::max();

    if (k > (int)points.size()) return res;

    vector<int> cur(points.size(), 0);
    dfs(points, k, 0, cur, res);

    return res;
}
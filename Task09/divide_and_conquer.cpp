#include "divide_and_conquer.hpp"
using namespace std;

static const double INF = 1e18;

static double dist2(const Point& a, const Point& b) {
    double dx = a.x - b.x, dy = a.y - b.y;
    return dx * dx + dy * dy;
}

static bool pointLess(const Point& a, const Point& b) {
    if (a.x != b.x) return a.x < b.x;
    return a.y < b.y;
}

static void mergeSortIndices(vector<int>& idx, int lo, int hi, const vector<Point>& pts) {
    if (hi - lo <= 1) return;
    int mid = (lo + hi) / 2;
    mergeSortIndices(idx, lo, mid, pts);
    mergeSortIndices(idx, mid, hi, pts);

    vector<int> tmp;
    tmp.reserve(hi - lo);
    int i = lo, j = mid;
    while (i < mid && j < hi) {
        if (pointLess(pts[idx[i]], pts[idx[j]])) tmp.push_back(idx[i++]);
        else                                     tmp.push_back(idx[j++]);
    }
    while (i < mid) tmp.push_back(idx[i++]);
    while (j < hi)  tmp.push_back(idx[j++]);

    for (int k = 0; k < (int)tmp.size(); k++) idx[lo + k] = tmp[k];
}

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

static vector<int> assignToNearest(const vector<Point>& pts,
                                    const vector<Point>& centers) {
    int k = (int)centers.size();
    vector<int> asgn(pts.size());
    for (int i = 0; i < (int)pts.size(); i++) {
        double best = INF;
        int bestC = 0;
        for (int c = 0; c < k; c++) {
            double d = dist2(pts[i], centers[c]);
            if (d < best) { best = d; bestC = c; }
        }
        asgn[i] = bestC;
    }
    return asgn;
}

static double totalCost(const vector<Point>& pts,
                         const vector<int>& asgn,
                         const vector<Point>& centers) {
    double cost = 0;
    for (int i = 0; i < (int)pts.size(); i++)
        cost += dist2(pts[i], centers[asgn[i]]);
    return cost;
}

static void dcHelper(const vector<Point>& pts,
                     vector<int> indices,
                     int k,
                     int offset,
                     vector<int>& globalAsgn) {

    int n = (int)indices.size();

    if (k == 1 || n <= k) {
        if (n <= k)
            for (int i = 0; i < n; i++)
                globalAsgn[indices[i]] = offset + i;
        else
            for (int idx : indices)
                globalAsgn[idx] = offset;
        return;
    }

    mergeSortIndices(indices, 0, (int)indices.size(), pts);

    int mid   = n / 2;
    int kLeft  = k / 2;
    int kRight = k - kLeft;

    vector<int> left(indices.begin(), indices.begin() + mid);
    vector<int> right(indices.begin() + mid, indices.end());

    dcHelper(pts, left,  kLeft,  offset,          globalAsgn);
    dcHelper(pts, right, kRight, offset + kLeft,  globalAsgn);
}

ClusterResult solveDivideAndConquer(const vector<Point>& points, int k) {
    ClusterResult res;
    int n = (int)points.size();
    res.assignment = vector<int>(n, 0);
    res.cost = INF;

    if (k <= 0 || k > n) return res;

    vector<int> indices(n);
    for (int i = 0; i < n; i++) indices[i] = i;
    dcHelper(points, indices, k, 0, res.assignment);

    const int MAX_ITER = 100;
    for (int iter = 0; iter < MAX_ITER; iter++) {
        vector<Point> centers = computeCentroids(points, res.assignment, k);
        vector<int> newAsgn   = assignToNearest(points, centers);

        bool changed = false;
        for (int i = 0; i < n; i++)
            if (newAsgn[i] != res.assignment[i]) { changed = true; break; }

        res.assignment = newAsgn;
        if (!changed) break;
    }

    vector<Point> centers = computeCentroids(points, res.assignment, k);
    res.cost = totalCost(points, res.assignment, centers);

    return res;
}

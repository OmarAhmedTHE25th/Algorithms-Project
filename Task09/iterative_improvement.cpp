#include "iterative_improvement.hpp"
using namespace std;

static const double INF = 1e18;

static unsigned long randState = 1;
static const int MY_RAND_MAX = 32767;

static int myRand() {
    randState = randState * 214013UL + 2531011UL;
    return (int)((randState >> 16) & 0x7FFF);
}

static void mySrand(unsigned long s) {
    randState = s;
}

static double dist2(const Point& a, const Point& b) {
    double dx = a.x - b.x, dy = a.y - b.y;
    return dx * dx + dy * dy;
}

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

static bool assignNearest(const vector<Point>& pts,
                           const vector<Point>& centers,
                           vector<int>&         asgn) {
    int k = (int)centers.size();
    bool changed = false;
    for (int i = 0; i < (int)pts.size(); i++) {
        double best = INF;
        int    bestC = 0;
        for (int c = 0; c < k; c++) {
            double d = dist2(pts[i], centers[c]);
            if (d < best) { best = d; bestC = c; }
        }
        if (asgn[i] != bestC) { asgn[i] = bestC; changed = true; }
    }
    return changed;
}

static double totalCost(const vector<Point>& pts,
                         const vector<int>&   asgn,
                         const vector<Point>& centers) {
    double cost = 0;
    for (int i = 0; i < (int)pts.size(); i++)
        cost += dist2(pts[i], centers[asgn[i]]);
    return cost;
}

static vector<Point> kMeansPlusPlusInit(const vector<Point>& pts, int k) {
    int n = (int)pts.size();
    vector<Point> centers;
    centers.reserve(k);

    centers.push_back(pts[myRand() % n]);

    vector<double> d2(n);
    for (int step = 1; step < k; step++) {
        double sum = 0;
        for (int i = 0; i < n; i++) {
            double best = INF;
            for (const Point& c : centers) {
                double d = dist2(pts[i], c);
                if (d < best) best = d;
            }
            d2[i] = best;
            sum += best;
        }
        double threshold = ((double)myRand() / MY_RAND_MAX) * sum;
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

static ClusterResult kMeansRun(const vector<Point>& pts, int k) {
    int n = (int)pts.size();
    ClusterResult res;
    res.assignment.assign(n, 0);
    res.cost = INF;

    vector<Point> centers = kMeansPlusPlusInit(pts, k);
    assignNearest(pts, centers, res.assignment);

    const int MAX_ITER = 300;
    for (int iter = 0; iter < MAX_ITER; iter++) {
        if (!computeCentroids(pts, res.assignment, k, centers)) break;
        if (!assignNearest(pts, centers, res.assignment)) break;
    }

    computeCentroids(pts, res.assignment, k, centers);
    res.cost = totalCost(pts, res.assignment, centers);
    return res;
}

ClusterResult solveIterativeImprovement(const vector<Point>& points, int k) {
    ClusterResult best;
    best.assignment = vector<int>(points.size(), 0);
    best.cost = INF;

    if (k <= 0 || k > (int)points.size()) return best;

    mySrand(42);

    const int RESTARTS = 10;
    for (int r = 0; r < RESTARTS; r++) {
        ClusterResult res = kMeansRun(points, k);
        if (res.cost < best.cost) best = res;
    }

    return best;
}

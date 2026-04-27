#include <vector>
#include <set>
#include <iostream>
#include <cstddef>

/*
 * Task 5 - Shooting an unseen moving target.
 *
 * n hiding spots are arranged on a line and indexed 1..n. A target hides
 * in one of them. After every shot that misses, the target moves to an
 * adjacent spot (1 -> 2, n -> n-1, otherwise +-1). The shooter has no
 * feedback. Design a fixed sequence of shots that always hits the target.
 *
 * Divide and Conquer:
 *   Every move flips the target's parity, so its INITIAL parity is fixed
 *   for the whole game. We split the problem into two disjoint sub-instances
 *   by initial parity:
 *     A) Target started on an even-indexed spot
 *           -> forward sweep over [2, n-1] catches it.
 *     B) Target started on an odd-indexed spot
 *           -> backward sweep over [n-1, 2] catches it.
 *   Each sweep itself is built by recursive halving of the index range
 *   (true binary divide and conquer rather than decrease-and-conquer).
 *   Combining the two sweeps gives a strategy that is correct on every
 *   possible target trajectory.
 */

using std::vector;

/**
 * Recursively build a contiguous sweep of indices [lo, hi] in the chosen
 * direction by halving the range and concatenating the two halves.
 *
 * @param lo  Left endpoint of the index range (inclusive).
 * @param hi  Right endpoint of the index range (inclusive).
 * @param dir +1 for ascending, -1 for descending.
 * @param out Output vector that receives the sweep, in order.
 */
static void buildSweep(const int lo, const int hi, const int dir, vector<int>& out)
{
    if (lo > hi) return;
    if (lo == hi) {
        out.push_back(lo);
        return;
    }
    const int mid = lo + (hi - lo) / 2;
    if (dir > 0) {
        buildSweep(lo,      mid, dir, out);
        buildSweep(mid + 1, hi,  dir, out);
    } else {
        buildSweep(mid + 1, hi,  dir, out);
        buildSweep(lo,      mid, dir, out);
    }
}

/**
 * Generate the full shot sequence for n hiding spots using D&C.
 */
vector<int> generateShots(const int n)
{
    vector<int> shots;
    if (n == 2) {
        // Base case: two spots, target alternates -> shoot the same spot twice.
        shots.push_back(2);
        shots.push_back(2);
        return shots;
    }
    buildSweep(2, n - 1, +1, shots); // sub-instance A: even-start targets
    buildSweep(2, n - 1, -1, shots); // sub-instance B: odd-start targets
    return shots;
}

/**
 * Adversarial verifier. Tracks every position the target could currently
 * occupy (a set), shoots, then propagates each survivor to its neighbours.
 * Returns true iff the live set is emptied by the end of the sequence.
 */
bool verifyStrategy(const int n, const vector<int>& shots, const bool trace)
{
    std::set<int> live;
    for (int p = 1; p <= n; ++p) live.insert(p);

    for (std::size_t r = 0; r < shots.size(); ++r) {
        const int s = shots[r];
        live.erase(s);

        if (trace) {
            std::cout << "  round " << (r + 1) << "  shoot " << s << "  survivors {";
            bool first = true;
            for (const int p : live) {
                if (!first) std::cout << ",";
                std::cout << p;
                first = false;
            }
            std::cout << "}\n";
        }

        if (live.empty()) return true;

        std::set<int> next;
        for (const int p : live) {
            if (p > 1) next.insert(p - 1);
            if (p < n) next.insert(p + 1);
        }
        live.swap(next);
    }
    return live.empty();
}

int main()
{
    int n;
    std::cout << "Enter number of hiding spots (n > 1): ";
    if (!(std::cin >> n) || n < 2) {
        std::cout << "Invalid n.\n";
        return 1;
    }

    const vector<int> shots = generateShots(n);

    std::cout << "\nShots needed: " << shots.size() << "\n";
    std::cout << "Sequence    : ";
    for (std::size_t i = 0; i < shots.size(); ++i) {
        std::cout << shots[i];
        if (i + 1 < shots.size()) std::cout << " ";
    }
    std::cout << "\n\nAdversarial trace:\n";

    const bool ok = verifyStrategy(n, shots, true);
    std::cout << "\nStrategy " << (ok ? "guarantees a hit." : "FAILS to guarantee a hit.") << "\n";

    std::cout << "\nSelf-check across small n:\n";
    for (int m = 2; m <= 12; ++m) {
        const vector<int> s = generateShots(m);
        const bool good = verifyStrategy(m, s, false);
        std::cout << "  n=" << m << "  shots=" << s.size()
                  << "  " << (good ? "OK" : "FAIL") << "\n";
    }

    return 0;
}

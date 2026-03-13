#include <vector>
#include <algorithm>
#include <iostream>

#include "Job.hpp"
/**
 * Problem: Job scheduling on a single machine to minimize total penalty.
 * Each job i has processing time t[i], deadline d[i], and penalty p[i].
 * 
 * Divide and Conquer (with Pruning) approach:
 * The problem is solved by deciding for each job (sorted by deadline) whether 
 * to complete it on time or late. 
 * 'savedPenalty' is the sum of penalties of jobs completed on time.
 * Total penalty = (Sum of all penalties) - (Sum of penalties of on-time jobs).
 * Minimizing total penalty is equivalent to maximizing saved penalty.
 */

using std::vector;
using std::sort;

// Structure to store the optimization results
struct Result {
    vector<Job> bestOrder; // Resulting sequence of jobs
    int totalPenalty = 0;  // Minimum total penalty achieved
};

// Internal state to track the best solution found during recursion
struct DCState {
    int bestSavedPenalty = -1; // Maximum penalty sum of jobs completed on time
    vector<char> best_on_time;      // 1 if job i is on time, 0 otherwise
};

/**
 * Recursive helper function implementing the Divide and Conquer approach with pruning.
 * @param jobs Vector of jobs, sorted by deadline.
 * @param i Index of the current job being considered.
 * @param currentTime Cumulative processing time of jobs selected to be on time.
 * @param savedPenalty Sum of penalties of jobs selected to be on time.
 * @param on_time Current decisions for jobs (1 for on time, 0 for late).
 * @param remainingPenaltySum Sum of penalties of jobs from index i to the end.
 * @param state Shared state to store the best results.
 */
static void DCHelper(vector<Job>& jobs,
    const int i,
    const int currentTime,
    const int savedPenalty,
    vector<char>& on_time,
    const int remainingPenaltySum,
    DCState& state) {
    
    // Pruning: if even by taking all remaining jobs on time we can't beat the best found so far, stop.
    if (savedPenalty + remainingPenaltySum <= state.bestSavedPenalty) return;

    // Base case: all jobs have been processed
    if (i == static_cast<int>(jobs.size())) {
        if (savedPenalty > state.bestSavedPenalty) {
            state.bestSavedPenalty = savedPenalty;
            state.best_on_time = on_time;
        }
        return;
    }

    const int penalty_sum = remainingPenaltySum - jobs[i].penalty;

    // Option 1: Mark job i as late (skip completing it by its deadline)
    on_time[i] = 0;
    DCHelper(jobs, i + 1, currentTime, savedPenalty, on_time, penalty_sum, state);

    // Option 2: Try to complete job i on time if the deadline allows
    if (currentTime + jobs[i].time <= jobs[i].deadline) {
        on_time[i] = 1;
        DCHelper(jobs,
            i + 1,
            currentTime + jobs[i].time,
            savedPenalty + jobs[i].penalty,
            on_time,
            penalty_sum,
            state);
        
        // Backtrack
        on_time[i] = 0;
    }
}

/**
 * The main optimization function using Divide and Conquer.
 */
Result optimizeJobScheduling(vector<Job> jobs) {
    // Sort jobs by deadline (EDF - Earliest Deadline First heuristic is useful here)
    sort(jobs.begin(), jobs.end());

    int totalPenaltyAll = 0;
    for (const auto& j : jobs) totalPenaltyAll += j.penalty;

    DCState state;
    state.best_on_time.assign(jobs.size(), 0);
    vector<char> currentTake(jobs.size(), 0);

    // Start recursion
    DCHelper(jobs, 0, 0, 0, currentTake, totalPenaltyAll, state);

    // Separate jobs into on-time and late based on the best solution found
    vector<Job> onTime, late;
    onTime.reserve(jobs.size());
    late.reserve(jobs.size());

    for (size_t i = 0; i < jobs.size(); ++i) {
        if (state.best_on_time[i]) onTime.push_back(jobs[i]);
        else late.push_back(jobs[i]);
    }

    // Prepare a final result: on-time jobs followed by late jobs
    Result res;
    res.bestOrder = onTime;
    res.bestOrder.insert(res.bestOrder.end(), late.begin(), late.end());
    res.totalPenalty = totalPenaltyAll - state.bestSavedPenalty;
    return res;
}

int main() {
    const vector<Job> jobs = {
        {1,22,6,32},
        {2,7,8,20},
        {3,17,9,48},
        {4,10,12,75}
    };
    Result r = optimizeJobScheduling(jobs);

    for (const auto& job : r.bestOrder) {
        std::cout << "Job " << job.id << " (Penalty: " << job.penalty << ")\n";
    }
    std::cout << "Total penalty: " << r.totalPenalty << "\n";
    return 0;
}

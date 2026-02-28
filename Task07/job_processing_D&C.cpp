#include <vector>
#include <algorithm>
#include <iostream>

#include "Job.hpp"
using  std::vector;
using std::sort;
struct Result {
    vector<Job> bestOrder;
    int totalPenalty = 0;
};
struct DCState {
    int bestSavedPenalty = -1;
    vector<char> bestTake;   // 0/1 decisions for each job (sorted order)
};
static void DCHelper( vector<Job>& jobs,
    const int i,
    const int currentTime,
    const int savedPenalty,
    vector<char>& currentTake,
    const int remainingPenaltySum,
    DCState& state) {
    if (savedPenalty + remainingPenaltySum <= state.bestSavedPenalty)return;
    if (i == static_cast<int>(jobs.size())) {
        if (savedPenalty>state.bestSavedPenalty) {
            state.bestSavedPenalty = savedPenalty;
            state.bestTake = currentTake;
        }
        return;
    }
    const int penalty_sum = remainingPenaltySum-jobs[i].penalty;
    //option 1: skip job i -> late -> penalty
    currentTake[i]=0;
    DCHelper(jobs,i+1,currentTime,savedPenalty,currentTake,penalty_sum,state);
    //option 2: take job i on time if the deadline allows
    if (currentTime+jobs[i].time <= jobs[i].deadline) {
        currentTake[i] = 1;
        DCHelper(jobs,
            i+1,
            currentTime+jobs[i].time,
            savedPenalty+jobs[i].penalty,
            currentTake,
            remainingPenaltySum-jobs[i].penalty,
            state);
        currentTake[i] = 0;
    }
}

Result optimizeJobScheduling(vector<Job> jobs) {
    sort(jobs.begin(), jobs.end());
    int totalPenaltyAll=0;
    for (const auto& j : jobs) totalPenaltyAll += j.penalty;
    DCState state;
    state.bestTake.assign(jobs.size(),0);
    vector<char> currentTake(jobs.size(), 0);
    DCHelper(jobs,0,0,0,currentTake,totalPenaltyAll,state);
    vector<Job> onTime, late;
    onTime.reserve(jobs.size());
    late.reserve(jobs.size());

    for (size_t i = 0; i < jobs.size(); ++i) {
        if (state.bestTake[i]) onTime.push_back(jobs[i]);
        else late.push_back(jobs[i]);
    }

    Result res;
    res.bestOrder = onTime;
    res.bestOrder.insert(res.bestOrder.end(), late.begin(), late.end());
    res.totalPenalty = totalPenaltyAll - state.bestSavedPenalty;
    return res;
}

int main() {
    vector<Job> jobs = {
        {1,4,5,10},
        {2,2,6,20},
        {3,1,6,30},
        {4,3,1,5}
    };
    Result r = optimizeJobScheduling(jobs);

    for (const auto& job : r.bestOrder) {
        std::cout << "Job " << job.id << " (Penalty: " << job.penalty << ")\n";
    }
    std::cout << "Total penalty: " << r.totalPenalty << "\n";
    return 0;
}

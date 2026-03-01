#include <iostream>
#include <limits>
#include <vector>
#include "Job.hpp"
using namespace std;


/*
A company must schedule jobs on one machine.
Each job has processing time t[i] and penalty p[i] if finished after deadline d[i].
Minimize total penalty.
Solve this problem using Dynamic Programming, Brute Force and divide and conquer. 
*/
// BruteForce solution

 bool compareJobs(const Job& a, const Job& b)
{
    return a.id < b.id; 
}
struct Result {
    std::vector<Job> bestOrder;
    int totalPenalty=numeric_limits<int>::max();
};

static int calculatePenalty(const std::vector<Job> &jobs) {
    int time = 0;
    int totalPenalty = 0;
    for(auto& job : jobs)
    {
        time += job.time;
        if (time > job.deadline)totalPenalty+=job.penalty;
    }
    return totalPenalty;
}

void BFHelper(const std::vector<Job>& jobs,
                             std::vector<Job>& current,
                             std::vector<bool>& used,
                             Result& best){
    if (current.size()== jobs.size()) {
        if (const int totalPenalty = calculatePenalty(current); totalPenalty < best.totalPenalty)
        {
            best.totalPenalty = totalPenalty;
            best.bestOrder = current;
        }
        return ;
    }
    for (size_t i = 0; i < jobs.size(); i++) {
        if (used[i])continue;
        used[i] = true;
        current.push_back(jobs[i]);
        BFHelper(jobs,current,used,best);
        current.pop_back();
        used[i] = false;
    }

}
Result optimizeJobScheduling(const vector<Job>& jobs) {
    Result best;
    std::vector<Job> current;
    std::vector used(jobs.size(), false);
    BFHelper(jobs,current,used,best);
    return best;
}


    int main()
    {
        //----------------------------Brute Force--------------------------------------
        vector<Job> jobs = {
            {1,4,5,10},
            {2,2,6,20},
            {3,1,6,30}
        };
        const auto [bestOrder, totalPenalty] = optimizeJobScheduling(jobs);
        for(const auto job : bestOrder)
        {
            cout << "Job " << job.id << " (Penalty: " << job.penalty << ")\n";

        }
        std::cout << "Total penalty: " << totalPenalty << "\n";
    vector<Job> jobs2 = {
        {1,8,2,17},
        {2,11,1,19},
        {3,3,1,80},
        {4,12,3,51}
    };
    const auto [bestOrder2, totalPenalty2] = optimizeJobScheduling(jobs2);
    for(const auto job : bestOrder2)
    {
        cout << "Job " << job.id << " (Penalty: " << job.penalty << ")\n";

    }
    std::cout << "Total penalty: " << totalPenalty2 << "\n";        return 0;
    }

#include <algorithm>
#include <iostream>
#include <vector>
#include "Job.hpp"
using namespace std;
int totalPenalty;
vector<int> onTimeJobs;
vector<int> lateJobs;
void optimizeJobScheduling(vector<Job>& jobs)
{
    if (jobs.empty()) return;
    sort(jobs.begin(), jobs.end());
    vector<Job> current;
    const size_t size = jobs.size();
    int total_time=0;
    for(const auto j: jobs)total_time+=j.time;
    vector<vector<int>> dp{size + 1, vector<int>(total_time + 1, 0)};
    for (int i = 1; i <= size; ++i) {
        const int p_i = jobs[i-1].penalty;
        const int t_i = jobs[i-1].time;
        const int d_i = jobs[i-1].deadline;
        for (int t = 0; t <= total_time; ++t) {
            int skipPenalty = p_i + dp[i-1][t];
            int doPenalty = 2e9;
            if (t >= t_i && t <= d_i) {
                doPenalty = dp[i-1][t - t_i];
            }
            dp[i][t] = min(doPenalty,skipPenalty);
            if (t > 0) dp[i][t] = min(dp[i][t], dp[i][t-1]);
        }
    }
    int curTime = total_time;
    for (int t = 0; t <= total_time; ++t) {
        if (dp[size][t] == totalPenalty) {
            curTime = t;
            break; 
        }
    }
    for (size_t i = size; i > 0; i--) {
        const int jobTime = jobs[i-1].time;
        const int jobDeadline = jobs[i-1].deadline;
        while ( curTime > 0 && dp[i][ curTime] == dp[i][ curTime-1]) {
             curTime--;
        }
        if (curTime >= jobTime && curTime <= jobDeadline && dp[i][curTime] == dp[i-1][curTime - jobTime]) {
            onTimeJobs.push_back(jobs[i-1].id);
            curTime -= jobTime;
        } else {
            lateJobs.push_back(jobs[i-1].id);
        }
    }
    reverse(onTimeJobs.begin(), onTimeJobs.end());
    totalPenalty = dp[size][total_time];

}

int main() {
    vector<Job> jobs = {
        {1,4,5,10},
        {2,2,6,20},
        {3,1,6,30},
        {4,3,1,5}
    };
    optimizeJobScheduling(jobs);
    cout << "--- Scheduling Result ---" << endl;
    cout << "Total Penalty: " << totalPenalty << endl;

    cout << "On-Time Sequence: ";
    for(int id : onTimeJobs) cout << id << " ";
    cout << "\nLate Jobs (Penalty paid): ";
    for(int id : lateJobs) cout << id << " ";
    cout << endl;

    cout << "Full Execution Order: ";
    for(int id : onTimeJobs) cout << id << " ";
    for(int id : lateJobs) cout << id << " ";
    cout << endl;
}

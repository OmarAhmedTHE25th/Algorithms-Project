#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;
/*
A company must schedule jobs on one machine.
Each job has processing time t[i] and penalty p[i] if finished after deadline d[i].
Minimize total penalty.
Solve this problem using Dynamic Programming, Brute Force and divide and conquer. 
*/
int main()
{
    
}
// BruteForce solution
struct Job
{
    int id;
    int deadline;
    int time;
    int penalty;
};

void jobProcessing(vector<Job> jobs)
{
    int minPenalty = INT_MAX;
    vector<Job> bestOrder;
    sort(jobs.begin(), jobs.end());
    vector<Job> result;
    do
    {
          int currentTime = 0;
          int totalPenalty = 0;
        for (auto job : jobs)
        {
             currentTime += job.time;
             if (currentTime > job.deadline) {
            totalPenalty += job.penalty;
        }
            
        }
         if (totalPenalty < minPenalty) {
        minPenalty = totalPenalty;
        bestOrder = jobs;
    }
        
    } while (next_permutation(jobs.begin(),jobs.end()));
    

}


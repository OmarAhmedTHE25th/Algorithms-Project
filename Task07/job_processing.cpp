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
// BruteForce solution
struct Job
{
    int id;
    int time;
    int deadline;
    int penalty;

};
 bool compareJobs(const Job& a, const Job& b)
{
    return a.id < b.id; 
}
struct Result {
    std::vector<Job> bestOrder;
    int totalPenalty;
};
Result jobProcessing(vector<Job> jobs)
{
    int minPenalty = INT_MAX;
    vector<Job> bestOrder;
    sort(jobs.begin(), jobs.end(),compareJobs);
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
        
    } while (next_permutation(jobs.begin(),jobs.end(),compareJobs));
    
return {bestOrder,minPenalty};
}


    int main()
    {
        //----------------------------Brute Force--------------------------------------
        vector<Job> jobs = {
            {1,4,5,10},
            {2,2,6,20},
            {3,1,6,30}
        };
        auto sortedJobSequence = jobProcessing(jobs);
        for(auto job : sortedJobSequence.bestOrder)
        {
            cout << "Job " << job.id << " (Penalty: " << job.penalty << ")\n";
        
        }
        std::cout << "Total penalty: " << sortedJobSequence.totalPenalty << "\n";
    }
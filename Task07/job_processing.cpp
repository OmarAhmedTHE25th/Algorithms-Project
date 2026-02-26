#include <iostream>
#include <vector>
#include <algorithm>
#include "Job.hpp"
#include "globals.cpp"

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
    int totalPenalty;
};
void optimizeJobScheduling(const std::vector<Job>& jobs,
               std::vector<Job>& current,
               std::vector<bool>& used)
               {
                if (current.size()== jobs.size())
                {
                    int time = 0;
                    int totalPenalty = 0;
                    for(auto& job : jobs)
                    {
                        time += job.time;
                        if (time > job.deadline)
                        {
                            totalPenalty+=job.penalty;
                        }  
                    }
                      if (totalPenalty < minPenalty)
                        {
                           minPenalty = totalPenalty;
                           bestOrder = jobs;
                        }
                        return;
                }
                for (size_t i = 0; i < jobs.size(); i++)
                { 
                    if (!used[i])
                    {
                        used[i] = true;
                        current.push_back(jobs[i]);
                        optimizeJobScheduling(bestOrder,current,used);
                        current.pop_back();
                        used[i] = false;
                    }
                    
                }
                
                
               }


    int main()
    {
        //----------------------------Brute Force--------------------------------------
        vector<Job> jobs = {
            {1,4,5,10},
            {2,2,6,20},
            {3,1,6,30}
        };
         std::vector<Job> current;
    std::vector<bool> used(jobs.size(), false);
        optimizeJobScheduling(jobs,current,used);
        for(auto job : bestOrder)
        {
            cout << "Job " << job.id << " (Penalty: " << job.penalty << ")\n";
        
        }
        std::cout << "Total penalty: " << minPenalty << "\n";
    }
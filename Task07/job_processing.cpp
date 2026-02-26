#include <iostream>
#include <vector>
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
Result optimizeJobScheduling(const std::vector<Job>& jobs,
               std::vector<Job>& current,
               std::vector<bool>& used)
               {
                   vector<Job> bestOrder;
                if (current.size()== jobs.size())
                {
                    int minPenalty = INT_MAX;
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
                        return {bestOrder,minPenalty};
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


                   return {};
               }


    int BF_main()
    {
        //----------------------------Brute Force--------------------------------------
        vector<Job> jobs = {
            {1,4,5,10},
            {2,2,6,20},
            {3,1,6,30}
        };
         std::vector<Job> current;
    std::vector<bool> used(jobs.size(), false);
        const auto result = optimizeJobScheduling(jobs,current,used);
        for(const auto job : result.bestOrder)
        {
            cout << "Job " << job.id << " (Penalty: " << job.penalty << ")\n";
        
        }
        std::cout << "Total penalty: " << result.totalPenalty << "\n";
        return 0;
    }

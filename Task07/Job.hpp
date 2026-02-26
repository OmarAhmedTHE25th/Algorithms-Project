#pragma once
struct Job
{
    int id;
    int time;
    int deadline;
    int penalty;
    bool operator<(const Job& other) const {
        if (deadline != other.deadline) {
            return deadline < other.deadline;
        }

        return penalty > other.penalty;
    }

};

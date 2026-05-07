#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <unordered_map>
#include <algorithm>

using namespace std;

// 3x4 Knight Moves Table
int moves[12][4] = {
    {5, 7, -1, -1}, {6, 8, -1, -1}, {3, 7, -1, -1},
    {2, 8, 10, -1}, {9, 11, -1, -1}, {0, 6, 10, -1},
    {1, 5, 11, -1}, {0, 2, -1, -1}, {1, 3, 9, -1},
    {4, 8, -1, -1}, {3, 5, -1, -1}, {4, 6, -1, -1}
};

string target = "WWW......BBB";

// Heuristic: Counts how many knights are NOT in their target position
int getH(string s) {
    int h = 0;
    for (int i = 0; i < 12; i++) {
        if (s[i] != '.' && s[i] != target[i]) h++;
    }
    return h;
}

struct State {
    string board;
    int g, h;
    // Priority Queue: Lowest (g + h) is processed first
    bool operator>(const State& other) const {
        return (g + h) > (other.g + other.h);
    }
};

void print(string s) {
    for (int i = 0; i < 12; i++) {
        cout << s[i] << ((i + 1) % 3 == 0 ? "\n" : " ");
    }
    cout << "-----------\n";
}

int main() {
    string start = "BBB......WWW";
    priority_queue<State, vector<State>, greater<State>> pq;
    unordered_map<string, string> parent;
    unordered_map<string, int> cost;

    pq.push({ start, 0, getH(start) });
    parent[start] = "DONE";
    cost[start] = 0;

    cout << "Iterative Improvement (A* Pathfinding Strategy)\n\n";

    while (!pq.empty()) {
        State cur = pq.top();
        pq.pop();

        if (cur.board == target) {
            vector<string> path;
            for (string p = target; p != "DONE"; p = parent[p]) path.push_back(p);
            reverse(path.begin(), path.end());

            for (int i = 0; i < path.size(); i++) {
                cout << "Step " << i << ":\n";
                print(path[i]);
            }
            cout << "Goal Reached in " << cur.g << " moves!\n";
            return 0;
        }

        for (int i = 0; i < 12; i++) {
            if (cur.board[i] == '.') continue;
            for (int m = 0; m < 4; m++) {
                int to = moves[i][m];
                if (to != -1 && cur.board[to] == '.') {
                    string next = cur.board;
                    swap(next[i], next[to]);

                    int newG = cur.g + 1;
                    if (cost.find(next) == cost.end() || newG < cost[next]) {
                        cost[next] = newG;
                        parent[next] = cur.board;
                        pq.push({ next, newG, getH(next) });
                    }
                }
            }
        }
    }
}
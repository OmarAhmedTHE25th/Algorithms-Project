#include <iostream>
#include <vector>
#include <queue>
#include <unordered_map>
#include <string>
#include <algorithm>

using namespace std;


int moves[12][4] = {
    {5, 7, -1, -1}, {6, 8, -1, -1}, {3, 7, -1, -1},
    {2, 8, 10, -1}, {9, 11, -1, -1}, {0, 6, 10, -1},
    {1, 5, 11, -1}, {0, 2, -1, -1}, {1, 3, 9, -1},
    {4, 8, -1, -1}, {3, 5, -1, -1}, {4, 6, -1, -1}
};

void print(string s) {
    for (int i = 0; i < 12; i++) {
        cout << s[i] << ((i + 1) % 3 == 0 ? "\n" : "");
    }
    cout << "-----------\n";
}

int main() {
    string start = "BBB......WWW", goal = "WWW......BBB";

    queue<string> q;
    unordered_map<string, string> parent;

    q.push(start);
    parent[start] = "DONE";

    while (!q.empty()) {
        string cur = q.front();
        q.pop();

        if (cur == goal) {

            vector<string> path;
            for (string p = goal; p != "DONE"; p = parent[p]) path.push_back(p);
            reverse(path.begin(), path.end());

            for (int i = 0; i < path.size(); i++) {
                cout << "Step " << i << ":\n";
                print(path[i]);
            }
            cout << "Minimum Moves: " << path.size() - 1 << endl;
            return 0;
        }


        for (int i = 0; i < 12; i++) {
            if (cur[i] == '.') continue;
            for (int m = 0; m < 4; m++) {
                int to = moves[i][m];
                if (to != -1 && cur[to] == '.') {
                    string next = cur;
                    swap(next[i], next[to]);

                    if (parent.find(next) == parent.end()) {
                        parent[next] = cur;
                        q.push(next);
                    }
                }
            }
        }
    }
    return 0;
}
#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>

using namespace std;

unordered_map<string, vector<int>> memo;

bool isAllEmpty(const string& s) {
    for (char c : s) if (c != '-') return false;
    return true;
}

void applyMove(string& s, int idx) {
    s[idx] = '-';
    if (idx > 0 && s[idx - 1] != '-')
        s[idx - 1] = (s[idx - 1] == 'H') ? 'T' : 'H';
    if (idx < (int)s.length() - 1 && s[idx + 1] != '-')
        s[idx + 1] = (s[idx + 1] == 'H') ? 'T' : 'H';
}

vector<int> solveDP(string s) {
    if (memo.count(s)) return memo[s];
    if (isAllEmpty(s)) return {};

    for (int i = 0; i < (int)s.length(); ++i) {
        if (s[i] == 'H') {
            string nextState = s;
            applyMove(nextState, i);

            vector<int> subResult = solveDP(nextState);

            if (isAllEmpty(nextState) || !subResult.empty()) {
                vector<int> currentPath;
                currentPath.push_back(i + 1);
                currentPath.insert(currentPath.end(), subResult.begin(), subResult.end());
                return memo[s] = currentPath;
            }
        }
    }
    return memo[s] = {};
}

int main() {
    string puzzle = "THHTHHH";
    string currentState = puzzle;

    cout << "Initial State: " << currentState << endl;
    cout << "----------------------------" << endl;

    memo.clear();
    vector<int> finalMoves = solveDP(puzzle);

    if (!finalMoves.empty()) {
        for (int i = 0; i < (int)finalMoves.size(); ++i) {
            int moveIdx = finalMoves[i] - 1;
            applyMove(currentState, moveIdx);

            cout << "Step " << i + 1 << ": Picked index " << finalMoves[i]
                << " -> " << currentState << endl;
        }

        cout << "----------------------------" << endl;
        cout << "Success All coins removed." << endl;
    }
    else {
        cout << "No solution possible." << endl;
    }

    return 0;
}
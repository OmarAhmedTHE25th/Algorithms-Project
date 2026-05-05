#include <iostream>
#include <vector>
#include <string>

using namespace std;

class CoinSolver {
public:
    void solveAndPrint(string coins) {
        vector<int> moves;
        cout << "Step 0 (Initial): " << coins << endl;
        cout << "---------------------------" << endl;

        if (solve(coins, moves, 0)) {
            cout << "---------------------------" << endl;
            cout << "Success All coins removed." << endl;
            cout << "Final Moves Sequence: ";
            for (int i = 0; i < (int)moves.size(); ++i) {
                cout << moves[i] << (i == (int)moves.size() - 1 ? "" : " -> ");
            }
            cout << endl;
        }
        else {
            cout << "No solution found." << endl;
        }
    }

private:
    bool solve(string& coins, vector<int>& moves, int offset) {
        if (isAllEmpty(coins)) return true;

        for (int i = 0; i < (int)coins.length(); ++i) {
            if (coins[i] == 'H') {
                string nextState = coins;
                applyMove(nextState, i);

                printStep(nextState, i + offset);

                string leftPart = nextState.substr(0, i);
                string rightPart = nextState.substr(i + 1);

                vector<int> currentPath = moves;
                currentPath.push_back(i + offset + 1);

                if (solve(leftPart, currentPath, offset) &&
                    solve(rightPart, currentPath, offset + i + 1)) {
                    moves = currentPath;
                    return true;
                }
            }
        }
        return false;
    }

    void applyMove(string& s, int idx) {
        s[idx] = '-';
        if (idx > 0 && s[idx - 1] != '-')
            s[idx - 1] = (s[idx - 1] == 'H') ? 'T' : 'H';
        if (idx < (int)s.length() - 1 && s[idx + 1] != '-')
            s[idx + 1] = (s[idx + 1] == 'H') ? 'T' : 'H';
    }

    void printStep(const string& s, int moveIdx) {
        static int stepNum = 1;
        cout << "Step " << stepNum++ << " (Picked index " << moveIdx + 1 << "): " << s << endl;
    }

    bool isAllEmpty(const string& s) {
        for (char c : s) if (c != '-') return false;
        return true;
    }
};

int main() {
    CoinSolver solver;
    string puzzle = "THHTHHH";
    solver.solveAndPrint(puzzle);
    return 0;
}
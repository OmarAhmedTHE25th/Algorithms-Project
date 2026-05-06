#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
using namespace std;

int knightMoves[12][4] = {
    {5, 7, -1, -1}, {6, 8, -1, -1}, {3, 7, -1, -1},
    {2, 8, 10, -1}, {9, 11, -1, -1}, {0, 6, 10, -1},
    {1, 5, 11, -1}, {0, 2, -1, -1}, {1, 3, 9, -1},
    {4, 8, -1, -1}, {3, 5, -1, -1}, {4, 6, -1, -1}
};

string target = "WWW......BBB";

int heuristic(string s) {
    int score = 0;
    for (int i = 0; i < 12; i++) {
        if (s[i] == target[i])
            score++;
    }
    return score;
}

void printBoard(string s) {
    for (int i = 0; i < 12; i++) {
        cout << s[i];
        if ((i + 1) % 3 == 0) cout << endl;
    }
    cout << "-----------\n";
}

int main() {
    srand(time(0));

    string current = "BBB......WWW";
    int steps = 0;

    cout << "Iterative Improvement (Hill Climbing)\n";
    printBoard(current);

    while (steps < 1000) {
        if (current == target) {
            cout << "Reached Goal!\n";
            break;
        }

        string best = current;
        int bestScore = heuristic(current);

        for (int i = 0; i < 12; i++) {
            if (current[i] == '.') continue;

            for (int m = 0; m < 4; m++) {
                int nextPos = knightMoves[i][m];
                if (nextPos == -1) break;

                if (current[nextPos] == '.') {
                    string next = current;
                    swap(next[i], next[nextPos]);

                    int h = heuristic(next);

                    if (h > bestScore) {
                        bestScore = h;
                        best = next;
                    }
                }
            }
        }

        if (best == current) {
            cout << "Stuck at local optimum -> Random Restart\n";

            for (int i = 0; i < 12; i++) {
                int j = rand() % 12;
                swap(current[i], current[j]);
            }

            printBoard(current);
            continue;
        }

        current = best;
        steps++;

        cout << "Step " << steps << ":\n";
        printBoard(current);
    }

    if (current != target)
        cout << "Did not reach exact goal.\n";

    return 0;
}
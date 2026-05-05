#include <iostream>
#include <vector>
#include <string>
using namespace std;

void printCoins(const vector<char>& coins) {
    for (char c : coins) {
        cout << c << " ";
    }
    cout << endl;
}

bool isFinished(const vector<char>& coins) {
    for (char c : coins) {
        if (c != '.') return false;
    }
    return true;
}

bool solve(vector<char> coins) {
    if (isFinished(coins)) {
        return true;
    }

    for (int i = 0; i < coins.size(); i++) {
        if (coins[i] == 'H') {
            vector<char> nextState = coins;

            nextState[i] = '.';

            if (i - 1 >= 0 && nextState[i - 1] != '.') {
                nextState[i - 1] = (nextState[i - 1] == 'H') ? 'T' : 'H';
            }

            if (i + 1 < nextState.size() && nextState[i + 1] != '.') {
                nextState[i + 1] = (nextState[i + 1] == 'H') ? 'T' : 'H';
            }

            cout << "Removed H at index " << i << " -> ";
            printCoins(nextState);

            if (solve(nextState)) return true;

        }
    }

    return false;
}

int main() {
    vector<char> coins = { 'T', 'H', 'H', 'T', 'H', 'H', 'H' };

    cout << "Initial State: ";
    printCoins(coins);
    cout << "--------------------------" << endl;

    if (solve(coins)) {
        cout << "--------------------------" << endl;
        cout << "All coins removed successfully" << endl;
    }
    else {
        cout << "No solution found for this configuration" << endl;
    }

    return 0;
}
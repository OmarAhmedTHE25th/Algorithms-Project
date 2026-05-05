#include <iostream>
#include <vector>
using namespace std;

void printCoins(const vector<char>& coins) {
    for (char c : coins) cout << c << " ";
    cout << endl;
}

int countPotentialHeads(vector<char> coins, int i) {
    int hCount = 0;
    if (i - 1 >= 0 && coins[i - 1] != '.')
        coins[i - 1] = (coins[i - 1] == 'H') ? 'T' : 'H';
    if (i + 1 < coins.size() && coins[i + 1] != '.')
        coins[i + 1] = (coins[i + 1] == 'H') ? 'T' : 'H';

    for (char c : coins) if (c == 'H') hCount++;
    return hCount;
}

void solveGreedy(vector<char> coins) {
    while (true) {
        int bestIndex = -1;
        int maxHeads = -1;

        for (int i = 0; i < coins.size(); i++) {
            if (coins[i] == 'H') {
                int score = countPotentialHeads(coins, i);
                if (score > maxHeads) {
                    maxHeads = score;
                    bestIndex = i;
                }
            }
        }

        if (bestIndex == -1) break;

        cout << "Greedy Choice: Remove H at index " << bestIndex << " -> ";
        coins[bestIndex] = '.';
        if (bestIndex - 1 >= 0 && coins[bestIndex - 1] != '.')
            coins[bestIndex - 1] = (coins[bestIndex - 1] == 'H') ? 'T' : 'H';
        if (bestIndex + 1 < coins.size() && coins[bestIndex + 1] != '.')
            coins[bestIndex + 1] = (coins[bestIndex + 1] == 'H') ? 'T' : 'H';

        printCoins(coins);
    }

    bool success = true;
    for (char c : coins) if (c != '.') success = false;

    if (success) cout << "SUCCESS: Greedy approach solved it\n";
    else cout << "FAILED: Greedy got stuck (Local Optimum)\n";
}

int main() {
    vector<char> coins = { 'T', 'H', 'H', 'T', 'H', 'H', 'H' };
    cout << "Initial: "; printCoins(coins);
    solveGreedy(coins);
    return 0;
}
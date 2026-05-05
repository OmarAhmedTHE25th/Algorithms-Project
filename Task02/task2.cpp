#include <iostream>
#include <vector>
#include <iomanip>
using namespace std;

int dx[] = {2,1,-1,-2,-2,-1,1,2};
int dy[] = {1,2,2,1,-1,-2,-2,-1};

bool isValid(int x, int y, int N, vector<vector<int>>& board) {
    return (x >= 0 && y >= 0 && x < N && y < N && board[x][y] == -1);
}

int countMoves(int x, int y, int N, vector<vector<int>>& board) {
    int count = 0;
    for(int i = 0; i < 8; i++) {
        int nx = x + dx[i];
        int ny = y + dy[i];
        if(isValid(nx, ny, N, board))
            count++;
    }
    return count;
}

bool knightTour(int sx, int sy, int N, vector<vector<int>>& board, bool &closed) {
    board.assign(N, vector<int>(N, -1));

    int x = sx, y = sy;
    board[x][y] = 0;

    for(int step = 1; step < N*N; step++) {
        int min_deg = 9;
        int next_x = -1, next_y = -1;

        for(int i = 0; i < 8; i++) {
            int nx = x + dx[i];
            int ny = y + dy[i];

            if(isValid(nx, ny, N, board)) {
                int deg = countMoves(nx, ny, N, board);
                if(deg < min_deg) {
                    min_deg = deg;
                    next_x = nx;
                    next_y = ny;
                }
            }
        }

        if(next_x == -1) return false;

        x = next_x;
        y = next_y;
        board[x][y] = step;
    }

    // check closed tour
    closed = false;
    for(int i = 0; i < 8; i++) {
        if(x + dx[i] == sx && y + dy[i] == sy) {
            closed = true;
            break;
        }
    }

    return true;
}

void printBoard(vector<vector<int>>& board, int N) {
    for(int i = 0; i < N; i++) {
        for(int j = 0; j < N; j++) {
            cout << setw(3) << board[i][j] << " ";
        }
        cout << endl;
    }
}

void runTest(int sx, int sy, int N) {
    vector<vector<int>> board;
    bool closed;

    cout << "=====================================\n";
    cout << "Board Size: " << N << " x " << N << endl;
    cout << "Start Position: (" << sx << ", " << sy << ")\n";

    bool success = knightTour(sx, sy, N, board, closed);

    if(!success) {
        cout << "No Tour Found (Greedy Failed)\n";
        return;
    }

    cout << "Tour Found!\n";
    cout << (closed ? " Closed Tour\n" : "Open Tour\n");

    cout << "\nBoard:\n";
    printBoard(board, N);
}

int main() {

    //  Multiple Test Cases
    runTest(0, 0, 5);
    runTest(3, 3, 8);
    runTest(7, 7, 8);
    runTest(0, 1, 8);   
    runTest(0, 0, 2);
    runTest(0, 0, 6);
    runTest(0, 0, 10);

    return 0;
}

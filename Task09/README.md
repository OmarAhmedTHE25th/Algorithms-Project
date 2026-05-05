# Task 9 — K-Clustering

## How to compile

Make sure all 4 `.cpp` files are compiled together.

### From the terminal (g++ / MinGW)

```
g++ -std=c++17 -O2 main.cpp brute_force.cpp divide_and_conquer.cpp iterative_improvement.cpp -o kclustering.exe
```

### From an IDE (Dev-C++, Code::Blocks, etc.)

Create a new **Project** (not single file) and add all 9 files to it:

- `main.cpp`
- `brute_force.cpp` + `brute_force.hpp`
- `divide_and_conquer.cpp` + `divide_and_conquer.hpp`
- `iterative_improvement.cpp` + `iterative_improvement.hpp`
- `clustering_common.hpp`
- `point.hpp`

Then Build & Run

## How to run

The program reads input from the keyboard:

```
Enter number of points: 6
Enter number of clusters k: 2
Enter 6 points as x y (one per line):
  Point 0: 1 1
  Point 1: 2 1
  ...
```
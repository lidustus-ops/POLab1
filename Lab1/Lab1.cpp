#include <iostream>
#include <vector>
#include <thread> 
#include <chrono>
#include <cstdlib> 
#include <ctime>   
#include <iomanip>

using namespace std;
using namespace std::chrono;

void fillMatrix(vector<vector<int>>& matrix, int N) {
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            matrix[i][j] = rand() % 1000 + 1;
        }
    }
}

void solveSequential(int N, vector<vector<int>>& matrix) {
    for (int j = 0; j < N; ++j) {
        int max_val = matrix[0][j];
        for (int i = 1; i < N; ++i) {
            if (matrix[i][j] > max_val) max_val = matrix[i][j];
        }
        matrix[j][j] = max_val;
    }
}

void partialSolve(int N, vector<vector<int>>* matrix, int start_col, int end_col) {
    for (int j = start_col; j < end_col; ++j) {
        int max_val = (*matrix)[0][j];
        for (int i = 1; i < N; ++i) {
            if ((*matrix)[i][j] > max_val) max_val = (*matrix)[i][j];
        }
        (*matrix)[j][j] = max_val;
    }
}

double solveParallel(int N, vector<vector<int>>& matrix, int num_threads) {
    vector<thread> threads;
    int chunk = N / num_threads;
    auto start = high_resolution_clock::now();

    for (int i = 0; i < num_threads; ++i) {
        int start_col = i * chunk;
        int end_col = (i == num_threads - 1) ? N : (i + 1) * chunk;
        threads.emplace_back(partialSolve, N, &matrix, start_col, end_col);
    }
    for (auto& th : threads) th.join();

    auto end = high_resolution_clock::now();
    return duration<double>(end - start).count();
}

int main() {
    srand(static_cast<unsigned int>(time(0)));
    int N = 5000;
    vector<vector<int>> matrix(N, vector<int>(N));
    fillMatrix(matrix, N);

    cout << "Parallel time (4 threads): " << solveParallel(N, matrix, 4) << "s" << endl;
    return 0;
}
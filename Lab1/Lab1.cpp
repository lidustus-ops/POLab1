#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <functional>

using namespace std;
using namespace std::chrono;

void processColumns(int N, vector<vector<int>>& matrix, int start_col, int end_col) {
    for (int j = start_col; j < end_col; ++j) {
        int max_val = matrix[0][j];
        for (int i = 1; i < N; ++i) {
            if (matrix[i][j] > max_val) {
                max_val = matrix[i][j];
            }
        }
        matrix[j][j] = max_val;
    }
}

void fillMatrix(vector<vector<int>>& matrix, int N) {
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            matrix[i][j] = rand() % 1000 + 1;
        }
    }
}

void solveSequential(int N, vector<vector<int>>& matrix) {
    processColumns(N, matrix, 0, N);
}

double solveParallel(int N, vector<vector<int>>& matrix, int num_threads) {
    if (num_threads <= 0) return 0;

    vector<thread> worker_threads;
    int chunk = N / num_threads;
    auto start_time = high_resolution_clock::now();

    for (int i = 0; i < num_threads; ++i) {
        int start_col = i * chunk;
        int end_col = (i == num_threads - 1) ? N : (i + 1) * chunk;
        worker_threads.emplace_back(processColumns, N, ref(matrix), start_col, end_col);
    }

    for (auto& th : worker_threads) {
        th.join();
    }

    auto end_time = high_resolution_clock::now();
    duration<double> elapsed = end_time - start_time;
    return elapsed.count();
}

void runTest(int N) {
    cout << "\n>>> Matrix: " << N << "x" << N << " <<<" << endl;
    vector<vector<int>> matrix(N, vector<int>(N));

    fillMatrix(matrix, N);
    auto start_seq = high_resolution_clock::now();
    solveSequential(N, matrix);
    auto end_seq = high_resolution_clock::now();
    duration<double> seq_time = end_seq - start_seq;

    cout << "Sequential Execution: " << fixed << setprecision(5) << seq_time.count() << "s" << endl;

    unsigned int logical_cores = thread::hardware_concurrency();
    unsigned int physical_cores = (logical_cores > 1) ? logical_cores / 2 : 1;

    vector<int> thread_counts = {
        (int)physical_cores/2,
        (int)physical_cores,
        (int)logical_cores,
        (int)logical_cores * 2,
        (int)logical_cores * 4,
        (int)logical_cores * 8,
		(int)logical_cores * 16
    };

    cout << left << setw(10) << "Threads" << "Execution Time (s)" << endl;
    cout << "-----------------------------------" << endl;

    for (int tc : thread_counts) {
        if (tc <= 0) continue;
        fillMatrix(matrix, N);
        double p_time = solveParallel(N, matrix, tc);
        cout << left << setw(10) << tc << p_time << "s" << endl;
    }
}

int main() {
    srand(static_cast<unsigned int>(time(0)));

    unsigned int cores = thread::hardware_concurrency();
    cout << "Logical cores: " << cores << endl;

    runTest(2000);
    runTest(5000);
    runTest(10000);

    return 0;
}

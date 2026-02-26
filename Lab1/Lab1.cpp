#include <iostream>
#include <vector>
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
            if (matrix[i][j] > max_val) {
                max_val = matrix[i][j];
            }
        }
        matrix[j][j] = max_val;
    }
}

int main() {
    srand(static_cast<unsigned int>(time(0)));
    int N = 2000;
    vector<vector<int>> matrix(N, vector<int>(N));
    fillMatrix(matrix, N);

    auto start = high_resolution_clock::now();
    solveSequential(N, matrix);
    auto end = high_resolution_clock::now();

    duration<double> elapsed = end - start;
    cout << "Sequential time for 2000x2000: " << elapsed.count() << "s" << endl;

    return 0;
}
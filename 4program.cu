#include <iostream>
#include <cuda.h>
using namespace std;

/*
================ VECTOR ADDITION =================
Each thread adds one element of A and B
*/
__global__ void vectorAdd(int *A, int *B, int *C, int n) {
    int i = blockIdx.x * blockDim.x + threadIdx.x;

    if (i < n)
        C[i] = A[i] + B[i];
}

void runVectorAddition() {
    int n;
    cout << "\n--- VECTOR ADDITION ---\n";
    cout << "Enter size: ";
    cin >> n;

    // Host arrays
    int *h_A = new int[n];
    int *h_B = new int[n];
    int *h_C = new int[n];

    cout << "Enter A:\n";
    for (int i = 0; i < n; i++) cin >> h_A[i];

    cout << "Enter B:\n";
    for (int i = 0; i < n; i++) cin >> h_B[i];

    // Device arrays
    int *d_A, *d_B, *d_C;
    cudaMalloc(&d_A, n * sizeof(int));
    cudaMalloc(&d_B, n * sizeof(int));
    cudaMalloc(&d_C, n * sizeof(int));

    // Copy to GPU
    cudaMemcpy(d_A, h_A, n * sizeof(int), cudaMemcpyHostToDevice);
    cudaMemcpy(d_B, h_B, n * sizeof(int), cudaMemcpyHostToDevice);

    // Kernel launch
    int blockSize = 256;
    int numBlocks = (n + blockSize - 1) / blockSize;

    vectorAdd<<<numBlocks, blockSize>>>(d_A, d_B, d_C, n);

    // Copy result back
    cudaMemcpy(h_C, d_C, n * sizeof(int), cudaMemcpyDeviceToHost);

    cout << "Result:\n";
    for (int i = 0; i < n; i++) cout << h_C[i] << " ";
    cout << endl;

    // Free memory
    delete[] h_A; delete[] h_B; delete[] h_C;
    cudaFree(d_A); cudaFree(d_B); cudaFree(d_C);
}

/*
================ MATRIX MULTIPLICATION =================
Each thread calculates one element of result matrix
*/
__global__ void matrixMul(int *A, int *B, int *C, int N) {
    int row = blockIdx.y * blockDim.y + threadIdx.y;
    int col = blockIdx.x * blockDim.x + threadIdx.x;

    if (row < N && col < N) {
        int sum = 0;

        for (int k = 0; k < N; k++) {
            sum += A[row * N + k] * B[k * N + col];
        }

        C[row * N + col] = sum;
    }
}

void runMatrixMultiplication() {
    int N;
    cout << "\n--- MATRIX MULTIPLICATION ---\n";
    cout << "Enter size (N x N): ";
    cin >> N;

    int *h_A = new int[N * N];
    int *h_B = new int[N * N];
    int *h_C = new int[N * N];

    cout << "Enter Matrix A:\n";
    for (int i = 0; i < N * N; i++) cin >> h_A[i];

    cout << "Enter Matrix B:\n";
    for (int i = 0; i < N * N; i++) cin >> h_B[i];

    int *d_A, *d_B, *d_C;
    cudaMalloc(&d_A, N * N * sizeof(int));
    cudaMalloc(&d_B, N * N * sizeof(int));
    cudaMalloc(&d_C, N * N * sizeof(int));

    cudaMemcpy(d_A, h_A, N * N * sizeof(int), cudaMemcpyHostToDevice);
    cudaMemcpy(d_B, h_B, N * N * sizeof(int), cudaMemcpyHostToDevice);

    // 2D grid
    dim3 threads(16, 16);
    dim3 blocks((N + 15) / 16, (N + 15) / 16);

    matrixMul<<<blocks, threads>>>(d_A, d_B, d_C, N);

    cudaMemcpy(h_C, d_C, N * N * sizeof(int), cudaMemcpyDeviceToHost);

    cout << "Result Matrix:\n";
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++)
            cout << h_C[i * N + j] << " ";
        cout << endl;
    }

    delete[] h_A; delete[] h_B; delete[] h_C;
    cudaFree(d_A); cudaFree(d_B); cudaFree(d_C);
}

/*
================ MAIN =================
Simple menu-driven program
*/
int main() {
    int choice;

    do {
        cout << "\nCUDA MENU\n";
        cout << "1. Vector Addition\n";
        cout << "2. Matrix Multiplication\n";
        cout << "3. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {
            case 1: runVectorAddition(); break;
            case 2: runMatrixMultiplication(); break;
            case 3: cout << "Exit...\n"; break;
            default: cout << "Invalid choice!\n";
        }

    } while (choice != 3);

    return 0;
}

/*
================ HOW TO RUN THIS PROGRAM =================

1. Save file with .cu extension (e.g., program.cu)

2. Compile using CUDA compiler:
   nvcc program.cu -o program

3. Run the executable:
   ./program  

================ REQUIREMENTS =================

- NVIDIA GPU
- CUDA Toolkit installed
- nvcc compiler available

==========================================================

COMMENTS:

1. CUDA uses parallel execution where each thread handles one element.
2. Vector Addition:
   - Each thread adds one element of vectors A and B.
   - Time complexity reduces due to parallel execution.

3. Matrix Multiplication:
   - Each thread computes one element of result matrix.
   - Uses 2D grid and block structure.

4. cudaMalloc → allocate memory on GPU
5. cudaMemcpy → transfer data between CPU and GPU
6. Kernel <<<blocks, threads>>> executes on GPU

7. This program demonstrates basic GPU parallel programming using CUDA.
*/

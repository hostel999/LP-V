#include <iostream>
#include <omp.h>   // used only for timing (omp_get_wtime)

using namespace std;

// Sequential Bubble Sort
void sequential_bubble(int arr[], int n)
{
    for (int i = 0; i < n - 1; i++)
    {
        for (int j = 0; j < n - i - 1; j++)
        {
            if (arr[j] > arr[j + 1])
            {
                // swap
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

int main()
{
    int n;
    cout << "Enter total number of elements: ";
    cin >> n;

    int *arr = new int[n];

    cout << "Enter elements:\n";
    for (int i = 0; i < n; i++)
        cin >> arr[i];

    // start time
    double start = omp_get_wtime();

    sequential_bubble(arr, n);

    // end time
    double end = omp_get_wtime();

    cout << "\nSorted array:\n";
    for (int i = 0; i < n; i++)
        cout << arr[i] << " ";

    cout << "\nExecution Time (Sequential): " << end - start << " seconds\n";

    delete[] arr;
    return 0;
}

/*
================ HOW TO RUN =================

1. Save file as:
   bubble.cpp

2. Compile (IMPORTANT: include OpenMP for timing):
   g++ -fopenmp bubble.cpp -o bubble

3. Run:
   Windows:
       ./bubble

4. Sample Input:
   Enter total number of elements: 5
   Enter elements:
   5 3 1 4 2

5. Output:
   Sorted array:
   1 2 3 4 5

============================================
*/

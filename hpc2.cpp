#include<iostream>
#include<omp.h>
using namespace std;

// ── Bubble Sort (Odd-Even) ──
void bubble(int a[], int n) {
    for(int i = 0; i < n; i++) {
        int start = i % 2;
        #pragma omp parallel for
        for(int j = start; j < n-1; j += 2)
            if(a[j] > a[j+1])
                swap(a[j], a[j+1]);
    }
}

// ── Merge Helper ──
void merge(int a[], int l, int m, int r) {
    int n1=m-l+1, n2=r-m;
    int L[n1], R[n2];
    for(int i=0;i<n1;i++) L[i]=a[l+i];
    for(int i=0;i<n2;i++) R[i]=a[m+1+i];
    int i=0, j=0, k=l;
    while(i<n1 && j<n2) a[k++] = L[i]<=R[j] ? L[i++] : R[j++];
    while(i<n1) a[k++]=L[i++];
    while(j<n2) a[k++]=R[j++];
}

// ── Merge Sort ──
void mergesort(int a[], int l, int r) {
    if(l >= r) return;
    int m = (l+r)/2;
    #pragma omp parallel sections
    {
        #pragma omp section
        mergesort(a, l, m);
        #pragma omp section
        mergesort(a, m+1, r);
    }
    merge(a, l, m, r);
}

// ── Main ──
int main() {
    int n;
    cout << "Enter n: "; cin >> n;
    int a[n], b[n];
    cout << "Enter elements: ";
    for(int i=0;i<n;i++) { cin >> a[i]; b[i]=a[i]; }

    double t;

    // Bubble
    t = omp_get_wtime();
    bubble(a, n);
    cout << "Bubble Time: " << omp_get_wtime()-t << "s | Sorted: ";
    for(int i=0;i<n;i++) cout << a[i] << " ";

    // Merge
    t = omp_get_wtime();
    mergesort(b, 0, n-1);
    cout << "\nMerge  Time: " << omp_get_wtime()-t << "s | Sorted: ";
    for(int i=0;i<n;i++) cout << b[i] << " ";

    cout << endl;
    return 0;
}




/*
STEPS TO RUN ON UBUNTU:
optional
1. Install compiler:
   sudo apt update
   sudo apt install g++

2. Save file:
   mergesort.cpp

3. Compile with OpenMP:
   g++ -fopenmp hpc2.cpp -o hpc2

4. Run program:
   ./hpc2

5. (Optional) Set threads:
   export OMP_NUM_THREADS=4
   ./mergesort
   
👉 Example run:

Enter total number of elements: 5
Enter elements: 5 2 9 1 6

👉 Output:

Sorted array is:
1 2 5 6 9
Execution Time: 0.000123
*/
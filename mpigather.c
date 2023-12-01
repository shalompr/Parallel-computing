#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <time.h>

void bubbleSort(int arr[], int n) {
int temp;
for (int i = 0; i < n - 1; i++) {
for (int j = 0; j < n - i - 1; j++) {
if (arr[j] > arr[j + 1]) {
temp = arr[j];
arr[j] = arr[j + 1];
arr[j + 1] = temp;
}
}
}
}

int main(int argc, char *argv[]) {
MPI_Init(&argc, &argv);

int rank, size;
MPI_Comm_rank(MPI_COMM_WORLD, &rank);
MPI_Comm_size(MPI_COMM_WORLD, &size);

int n = 7; // Size of the array
int arr[n];

// Initialize the array with specific values on the root process
if (rank == 0) {
int specificValues[] = {22, 90, 77, 55, 33, 11, 1};
for (int i = 0; i < n; i++) {
arr[i] = specificValues[i];
}
}

int local_size = n / size; // Size of each local array
int local_arr[local_size];

// Scatter the array to local arrays
MPI_Scatter(arr, local_size, MPI_INT, local_arr, local_size, MPI_INT, 0,
MPI_COMM_WORLD);

// Measure execution time
double start_time = MPI_Wtime();

// Perform parallel bubble sort
for (int i = 0; i < n; i++) {
if (i % 2 == 0) {

// Even phase
bubbleSort(local_arr, local_size);
}

MPI_Barrier(MPI_COMM_WORLD); // Synchronize processes before gathering
MPI_Gather(local_arr, local_size, MPI_INT, arr, local_size, MPI_INT, 0,
MPI_COMM_WORLD);

if (i % 2 == 1) {
// Odd phase
bubbleSort(arr, n);
}

MPI_Barrier(MPI_COMM_WORLD); // Synchronize processes before gathering
MPI_Gather(arr, local_size, MPI_INT, local_arr, local_size, MPI_INT, 0,
MPI_COMM_WORLD);
}

// Measure execution time
double end_time = MPI_Wtime();
double execution_time = end_time - start_time;

if (rank == 0) {
// Print the sorted array
printf("Sorted array: ");
for (int i = 0; i < n; i++) {
printf("%d ", arr[i]);
}
printf("\n");

printf("Execution Time: %f seconds\n", execution_time);

}

MPI_Finalize();
return 0;
}
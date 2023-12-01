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
int local_size = n / size; // Size of each local array

int local_arr[local_size];
int arr[n];

if (rank == 0) {
// Initialize the array with specific values on the root process
int specificValues[] = {22, 90, 77, 55, 33, 11, 1};
for (int i = 0; i < n; i++) {
arr[i] = specificValues[i];
}
}

// Broadcast the array to all processes
MPI_Bcast(arr, n, MPI_INT, 0, MPI_COMM_WORLD);

// Scatter the array to local arrays
MPI_Scatter(arr, local_size, MPI_INT, local_arr, local_size, MPI_INT, 0,
MPI_COMM_WORLD);

// Measure execution time
double start_time = MPI_Wtime();

// Perform parallel bubble sort
for (int i = 0; i < n; i++) {
bubbleSort(local_arr, local_size);

// Allgather to ensure correct ordering for the next iteration
MPI_Allgather(local_arr, local_size, MPI_INT, arr, local_size, MPI_INT,
MPI_COMM_WORLD);
}

// Calculate local sum
int local_sum = 0;
for (int i = 0; i < local_size; i++) {
local_sum += local_arr[i];
}

// Allreduce local sums to find the global sum
int global_sum;
MPI_Allreduce(&local_sum, &global_sum, 1, MPI_INT, MPI_SUM,
MPI_COMM_WORLD);

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

printf("Global Sum: %d\n", global_sum);
printf("Execution Time: %f seconds\n", execution_time);
}

MPI_Finalize();
return 0;
}
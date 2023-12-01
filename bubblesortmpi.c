#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <sys/time.h>

#define SIZE 7

void swap(int *xp, int *yp) {
int temp = *xp;
*xp = *yp;
*yp = temp;
}

void bubbleSort(int arr[], int n) {
for (int i = 0; i < n-1; i++)
for (int j = 0; j < n-i-1; j++)
if (arr[j] > arr[j+1])
swap(&arr[j], &arr[j+1]);
}

int main(int argc, char** argv) {
int rank, size;
MPI_Init(&argc, &argv);
MPI_Comm_rank(MPI_COMM_WORLD, &rank);
MPI_Comm_size(MPI_COMM_WORLD, &size);

int *data;

int localSize = SIZE / size;
data = (int *)malloc(SIZE * sizeof(int));

if (rank == 0) {
// Initialize array with specified values
int initialData[SIZE] = {22, 90, 77, 55, 33, 11, 1};
for (int i = 0; i < SIZE; i++)
data[i] = initialData[i];
}

// Scatter the data to all processes
MPI_Scatter(data, localSize, MPI_INT, data, localSize, MPI_INT, 0,
MPI_COMM_WORLD);

// Measure the start time
double start_time;
if (rank == 0)
start_time = MPI_Wtime();

// Perform local bubble sort
bubbleSort(data, localSize);

// Gather the sorted data
MPI_Gather(data, localSize, MPI_INT, data, localSize, MPI_INT, 0,
MPI_COMM_WORLD);

// Measure the end time
double end_time;
if (rank == 0) {
end_time = MPI_Wtime();
printf("Sorted array: ");

for (int i = 0; i < SIZE; i++) {
printf("%d ", data[i]);
}
printf("\n");
printf("Execution Time: %f seconds\n", end_time - start_time);
}

MPI_Finalize();
free(data);
return 0;
}
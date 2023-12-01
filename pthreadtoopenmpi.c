#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define NUM_THREADS 4
#define ARRAY_SIZE 1000

int array[ARRAY_SIZE];
int sum = 0;

int main() {
long t;

// Initialize array
for (int i = 0; i < ARRAY_SIZE; ++i) {
array[i] = i + 1;
}

// Parallel region with OpenMP

#pragma omp parallel num_threads(NUM_THREADS) private(t)
{
int localSum = 0;

// Each thread computes its local sum
#pragma omp for
for (int i = 0; i < ARRAY_SIZE; ++i) {
localSum += array[i];
}

// Critical section to update the global sum
#pragma omp critical
{
sum += localSum;
}
}

printf("Sum: %d\n", sum);

return 0;
}
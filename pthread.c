#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define NUM_THREADS 4
#define ARRAY_SIZE 1000

int array[ARRAY_SIZE];
int sum = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER; // Declare mutex

void *computeSum(void *threadID) {
long id = (long)threadID;
int localSum = 0;

for (int i = id * (ARRAY_SIZE / NUM_THREADS); i < (id + 1) * (ARRAY_SIZE /
NUM_THREADS); ++i) {
localSum += array[i];
}

// Critical section
pthread_mutex_lock(&mutex);
sum += localSum;
pthread_mutex_unlock(&mutex);

pthread_exit(NULL);
}

int main() {
pthread_t threads[NUM_THREADS];
long t;

// Initialize array
for (int i = 0; i < ARRAY_SIZE; ++i) {
array[i] = i + 1;
}

// Measure execution time
clock_t start_time = clock();

// Create threads
for (t = 0; t < NUM_THREADS; ++t) {
pthread_create(&threads[t], NULL, computeSum, (void *)t);
}

// Join threads
for (t = 0; t < NUM_THREADS; ++t) {
pthread_join(threads[t], NULL);
}

// Measure execution time
clock_t end_time = clock();
double execution_time = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;

printf("Sum: %d\n", sum);

printf("Execution Time: %f seconds\n", execution_time);

pthread_exit(NULL);
}
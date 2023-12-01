#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

void oddEvenBubbleSort(int arr[], int n) {
int temp;
int sorted = 0;

while (!sorted) {
sorted = 1;

// Odd phase
#pragma omp parallel for shared(arr) private(temp) reduction(&&:sorted)
for (int i = 1; i < n - 1; i += 2) {
if (arr[i] > arr[i + 1]) {
temp = arr[i];
arr[i] = arr[i + 1];
arr[i + 1] = temp;
sorted = 0; // Set sorted to false if a swap occurred
}

}

// Even phase
#pragma omp parallel for shared(arr) private(temp) reduction(&&:sorted)
for (int i = 0; i < n - 1; i += 2) {
if (arr[i] > arr[i + 1]) {
temp = arr[i];
arr[i] = arr[i + 1];
arr[i + 1] = temp;
sorted = 0; // Set sorted to false if a swap occurred
}
}

// Ensure all threads have finished their work before checking 'sorted'
#pragma omp barrier
}
}

int main() {
int n = 10; // Adjust the size of the array as needed
int arr[] = {64, 34, 25, 12, 22, 11, 90, 88, 75, 50};

printf("Original array: ");
for (int i = 0; i < n; i++) {
printf("%d ", arr[i]);
}
printf("\n");

// Perform odd-even phase parallel bubble sort
oddEvenBubbleSort(arr, n);

printf("Sorted array: ");
for (int i = 0; i < n; i++) {
printf("%d ", arr[i]);
}
printf("\n");

return 0;
}
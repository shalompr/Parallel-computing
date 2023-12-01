#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

void bubbleSort(int arr[], int n) {
int temp;
#pragma omp parallel for
for (int i = 0; i < n - 1; i++) {
#pragma omp parallel for shared(arr) private(temp)
for (int j = 0; j < n - i - 1; j++) {
if (arr[j] > arr[j + 1]) {
#pragma omp critical
{
temp = arr[j];

arr[j] = arr[j + 1];
arr[j + 1] = temp;
}
}
}
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

// Perform parallel bubble sort
bubbleSort(arr, n);

printf("Sorted array: ");
for (int i = 0; i < n; i++) {
printf("%d ", arr[i]);
}
printf("\n");
return 0;
}
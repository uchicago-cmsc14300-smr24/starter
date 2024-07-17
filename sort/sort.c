#include "sort.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static inline void swap(int *arr, int i, int j)
{
    int tmp = arr[i];
    arr[i] = arr[j];
    arr[j] = tmp;
}

void insertion_sort(int *arr, int len)
{
    for (int i = 0; i < len; i++) {
        int key = arr[i];

        int j;
        for (j = i - 1; j >= 0 && arr[j] > key; j--) {
            arr[j + 1] = arr[j];
        }

        arr[j + 1] = key;
    }
}

void selection_sort(int *arr, int len)
{
    for (int i = 0; i < len - 1; i++) {
        int min_idx = i;
        for (int j = i + 1; j < len; j++) {
            if (arr[j] < arr[min_idx]) {
                min_idx = j;
            }
        }
        swap(arr, min_idx, i);
    }
}

void bubble_sort(int *arr, int len)
{
    bool swapped;

    for (int i = 0; i < len - 1; i++) {
        swapped = false;
        for (int j = 0; j < len - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                swap(arr, j, j + 1);
                swapped = true;
            }
        }

        if (!swapped) {
            break;
        }
    }
}

static void merge(int *arr, int *scratch, int start, int mid, int end)
{
    int len1 = mid - start;
    int len2 = end - mid;

    if (arr[mid - 1] <= arr[mid]) {
        return;
    }

    int *arr1 = scratch + start;
    int *arr2 = scratch + mid;

    memcpy(arr1, &arr[start], len1 * sizeof(int));
    memcpy(arr2, &arr[mid], len2 * sizeof(int));

    int front1 = 0, front2 = 0, curr_idx = start;

    while (front1 < len1 && front2 < len2) {
        if (arr1[front1] <= arr2[front2]) {
            arr[curr_idx++] = arr1[front1++];
        } else {
            arr[curr_idx++] = arr2[front2++];
        }
    }

    while (front1 < len1) {
        arr[curr_idx++] = arr1[front1++];
    }

    while (front2 < len2) {
        arr[curr_idx++] = arr2[front2++];
    }
}

static void merge_sort_impl(int *arr, int *scratch, int start, int end)
{
    if (end - start <= 1) {
        return;
    }

    int mid = start + (end - start) / 2;

    merge_sort_impl(arr, scratch, start, mid);
    merge_sort_impl(arr, scratch, mid, end);
    merge(arr, scratch, start, mid, end);
}

void merge_sort(int *arr, int len)
{
    /* Reduce the number of allocations by pre-allocating a ``scratch''
     * area for all sub-calls */
    int *scratch = malloc(len * sizeof(int));

    merge_sort_impl(arr, scratch, 0, len);

    free(scratch);
}

static int partition(int *arr, int start, int end)
{
    int pivot = arr[start];
    int next = end - 1;

    for (int i = end - 1; i > start; i--) {
        if (arr[i] > pivot) {
            swap(arr, next--, i);
        }
    }

    swap(arr, start, next);
    return next;
}

static void quick_sort_impl(int *arr, int start, int end)
{
    if (end - start <= 1) {
        return;
    }

    int p = partition(arr, start, end);

    quick_sort_impl(arr, start, p);
    quick_sort_impl(arr, p + 1, end);
}

void quick_sort(int *arr, int len)
{
    quick_sort_impl(arr, 0, len);
}

static void bubble_up(int *arr, int i)
{
    while (i > 0) {
        int parent = (i - 1) / 2;

        if (arr[parent] < arr[i]) {
            swap(arr, parent, i);
            i = parent;
        } else {
            break;
        }
    }
}

static void bubble_down(int *arr, int len, int i)
{
    for (;;) {
        int largest = i;
        int left = 2 * i + 1;
        int right = 2 * i + 2;

        if (left < len && arr[left] > arr[largest]) {
            largest = left;
        }

        if (right < len && arr[right] > arr[largest]) {
            largest = right;
        }

        if (largest != i) {
            swap(arr, i, largest);
            i = largest;
        } else {
            break;
        }
    }
}

void heap_sort(int *arr, int len)
{
    (void) bubble_up;
    for (int i = len / 2 - 1; i >= 0; i--) {
        bubble_down(arr, len, i);
    }

    for (int i = len - 1; i >= 1; i--) {
        swap(arr, 0, i);
        bubble_down(arr, i, 0);
    }
}

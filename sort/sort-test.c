#include "sort.h"
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

int *make_random_arr(int len);

bool is_sorted(int *arr, int len);

void time_sort(int *arr, int len, const char *name,
               void (*sort)(int *arr, int len));

struct sort_alg {
    const char *name;
    void (*sort)(int *arr, int len);
};

struct sort_alg ALGS[] = {
    {"Insertion Sort", insertion_sort},
    {"Selection Sort", selection_sort},
    {"Bubble Sort", bubble_sort},
    {"Merge Sort", merge_sort},
    {"Quick Sort", quick_sort},
    {"Heap Sort", heap_sort},
};

int N_ALGS = sizeof(ALGS) / sizeof(ALGS[0]);

int main(void)
{
    srand(time(NULL)); // seed the random-number generator

    int N = 0x1 << 16; // 2 ^ 20 = 1,048,576 elements
    int *ref_arr = make_random_arr(N);

    for (int i = 0; i < N_ALGS; i++) {
        int *arr = malloc(N * sizeof(*arr));
        memcpy(arr, ref_arr, N * sizeof(*arr));

        time_sort(arr, N, ALGS[i].name, ALGS[i].sort);

        free(arr);
    }

    free(ref_arr);
    return EXIT_SUCCESS;
}

int *make_random_arr(int len)
{
    int *arr = malloc(len * sizeof(*arr));

    for (int i = 0; i < len; i++) {
        arr[i] = rand() % (10 * len);
    }

    return arr;
}

bool is_sorted(int *arr, int len)
{
    if (len <= 1) {
        return true;
    }

    for (int i = 1; i < len; i++) {
        if (arr[i - 1] > arr[i]) {
            return false;
        }
    }

    return true;
}

void time_sort(int *arr, int len, const char *name,
        void (*sort)(int *arr, int len))
{
    struct timespec start, stop;

    clock_gettime(CLOCK_REALTIME, &start);
    sort(arr, len);
    clock_gettime(CLOCK_REALTIME, &stop);

    double elapsed = (stop.tv_sec - start.tv_sec) * 1000.0
        + (stop.tv_nsec - start.tv_nsec) / 1000000.0;
    if (is_sorted(arr, len)) {
        printf("%s takes %.02f milliseconds to sort %d elements (verified)\n",
                name, elapsed, len);
    } else {
        printf("%s BUG!\n", name);
    }
}


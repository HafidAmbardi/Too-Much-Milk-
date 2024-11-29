#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdatomic.h>
#include <time.h>

#define NUM_THREADS 4

atomic_int counter = 0;
pthread_t threads[NUM_THREADS];
int thread_ids[NUM_THREADS];

void* critical_section(void* arg) {
    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);

    // Simulate critical section work
    for (int i = 0; i < 1000000; i++) {
        atomic_fetch_add(&counter, 1);
    }

    clock_gettime(CLOCK_MONOTONIC, &end);
    double elapsed_time = (end.tv_sec - start.tv_sec) +
                          (end.tv_nsec - start.tv_nsec) / 1e9;
    printf("Thread %d elapsed time: %.6f seconds\n", *(int*)arg, elapsed_time);

    return NULL;
}

int main() {
    struct timespec start, end;

    // Record start time
    clock_gettime(CLOCK_MONOTONIC, &start);

    // Create threads
    for (int i = 0; i < NUM_THREADS; i++) {
        thread_ids[i] = i;
        pthread_create(&threads[i], NULL, critical_section, &thread_ids[i]);
    }

    // Wait for threads to complete
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    // Record end time
    clock_gettime(CLOCK_MONOTONIC, &end);

    // Calculate elapsed time
    double elapsed_time = (end.tv_sec - start.tv_sec) +
                          (end.tv_nsec - start.tv_nsec) / 1e9;

    // Final counter value and performance analysis
    printf("Final counter value: %d\n", atomic_load(&counter));
    printf("Elapsed time: %.6f seconds\n", elapsed_time);

    return 0;
}

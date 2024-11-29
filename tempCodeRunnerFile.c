#include <stdio.h>
#include <pthread.h>
#include <stdatomic.h>

#define NUM_THREADS 3
#define ITERATIONS 1000000

volatile int turn[NUM_THREADS];
volatile int flag[NUM_THREADS];
volatile int counter = 0;

void init_lock() {
    for (int i = 0; i < NUM_THREADS; i++) {
        flag[i] = 0;
        turn[i] = 0;
    }
}

void lock(int thread_id) {
    flag[thread_id] = 1;
    for (int other = 0; other < NUM_THREADS; other++) {
        if (other == thread_id) continue;
        turn[thread_id] = other;
        while (flag[other] && turn[thread_id] == other) {
            // Busy-wait
        }
    }
}

void unlock(int thread_id) {
    flag[thread_id] = 0;
}

void *critical_section(void *arg) {
    int thread_id = *(int *)arg;

    for (int i = 0; i < ITERATIONS; i++) {
        lock(thread_id);

        // Critical section
        counter++;

        unlock(thread_id);
    }

    printf("Thread %d completed %d iterations.\n", thread_id, ITERATIONS);
    return NULL;
}

int main() {
    pthread_t threads[NUM_THREADS];
    int thread_ids[NUM_THREADS];

    init_lock();

    for (int i = 0; i < NUM_THREADS; i++) {
        thread_ids[i] = i;
        pthread_create(&threads[i], NULL, critical_section, &thread_ids[i]);
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("Final counter value: %d (Expected: %d)\n", counter, NUM_THREADS * ITERATIONS);

    return 0;
}

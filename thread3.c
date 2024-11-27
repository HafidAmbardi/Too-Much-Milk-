#include <stdio.h>
#include <pthread.h>
#include <stdatomic.h>

#define NUM_THREADS 3
#define ITERATIONS 10

// Shared variables for the locking algorithm
volatile int turn[NUM_THREADS];
volatile int flag[NUM_THREADS];

// Shared resource
volatile int counter = 0;

// Function to initialize the locking mechanism
void init_lock() {
    for (int i = 0; i < NUM_THREADS; i++) {
        flag[i] = 0;
        turn[i] = 0;
    }
}

// Lock function for a specific thread
void lock(int thread_id) {
    flag[thread_id] = 1;
    for (int other = 0; other < NUM_THREADS; other++) {
        if (other == thread_id) continue;

        // Set turn to the other thread
        turn[thread_id] = other;

        // Wait while the other thread wants to enter and it's their turn
        while (flag[other] && turn[thread_id] == other) {
            // Busy-wait
        }
    }
}

// Unlock function for a specific thread
void unlock(int thread_id) {
    flag[thread_id] = 0;
}

// Critical section function
void *critical_section(void *arg) {
    int thread_id = *(int *)arg;

    for (int i = 0; i < ITERATIONS; i++) {
        lock(thread_id);

        // Start of critical section
        printf("Thread %d: entering critical section\n", thread_id);
        counter++;
        printf("Thread %d: counter = %d\n", thread_id, counter);
        printf("Thread %d: leaving critical section\n", thread_id);
        // End of critical section

        unlock(thread_id);
    }
    return NULL;
}

int main() {
    pthread_t threads[NUM_THREADS];
    int thread_ids[NUM_THREADS];

    init_lock();

    // Create threads
    for (int i = 0; i < NUM_THREADS; i++) {
        thread_ids[i] = i;
        pthread_create(&threads[i], NULL, critical_section, &thread_ids[i]);
    }

    // Wait for threads to complete
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("Final counter value: %d\n", counter);

    return 0;
}

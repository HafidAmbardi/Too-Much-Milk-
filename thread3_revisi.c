#include <stdio.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

// Shared variables
volatile bool noteA = false;     // Represents "note left by A"
volatile bool noteB = false;     // Represents "note left by B"
volatile bool noteC = false;     // Represents "note left by C"
volatile bool milkAvailable = false; // Represents whether milk is available

// Thread A function
void* threadA(void* arg) {
    noteA = true; // Leave note A

    // Wait while either note B or note C is present
    while (noteB || noteC) {
        // Do nothing
    }

    // Check if milk is not available, then buy milk
    if (!milkAvailable) {
        printf("Thread A: Buying milk\n");
        milkAvailable = true;
    }

    noteA = false; // Remove note A
    return NULL;
}

// Thread B function
void* threadB(void* arg) {
    noteB = true; // Leave note B

    // Wait while either note A or note C is present
    while (noteA || noteC) {
        // Do nothing
    }

    // Check if milk is not available, then buy milk
    if (!milkAvailable) {
        printf("Thread B: Buying milk\n");
        milkAvailable = true;
    }

    noteB = false; // Remove note B
    return NULL;
}

// Thread C function
void* threadC(void* arg) {
    noteC = true; // Leave note C

    // Wait while either note A or note B is present
    while (noteA || noteB) {
        // Do nothing
    }

    // Check if milk is not available, then buy milk
    if (!milkAvailable) {
        printf("Thread C: Buying milk\n");
        milkAvailable = true;
    }

    noteC = false; // Remove note C
    return NULL;
}

void run_test() {
    pthread_t t1, t2, t3;
    pthread_t threads[3];
    void* (*thread_funcs[3])(void*) = {threadA, threadB, threadC};

    // Reset shared variables
    noteA = false;
    noteB = false;
    noteC = false;
    milkAvailable = false;

    // Shuffle the order of thread functions
    for (int i = 0; i < 3; i++) {
        int j = rand() % 3;
        void* (*temp)(void*) = thread_funcs[i];
        thread_funcs[i] = thread_funcs[j];
        thread_funcs[j] = temp;
    }

    // Create threads with random delays
    for (int i = 0; i < 3; i++) {
        pthread_create(&threads[i], NULL, thread_funcs[i], NULL);
        usleep(rand() % 1000); // Random delay
    }

    // Wait for threads to finish
    for (int i = 0; i < 3; i++) {
        pthread_join(threads[i], NULL);
    }

    // Check final state
    if (milkAvailable) {
        printf("Milk has been bought successfully!\n");
    } else {
        printf("No milk was bought!\n");
    }
}

int main() {
    srand(time(NULL)); // Seed the random number generator

    int test_runs = 10; // Number of test runs
    for (int i = 0; i < test_runs; i++) {
        printf("Test run %d:\n", i + 1);
        run_test();
        printf("\n");
    }

    return 0;
}

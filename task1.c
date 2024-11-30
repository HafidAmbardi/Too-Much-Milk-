#include <stdio.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>

// Shared variables
volatile bool noteA = false; // Represents "note left by A"
volatile bool noteB = false; // Represents "note left by B"
volatile int milkAvailable = 0; // Represents the amount of milk available
volatile int milkBoughtSuccessfullyCounter = 0; // Counter for successful milk purchases
volatile int tooMuchMilkCounter = 0; // Counter for too much milk
volatile int deadlockCounter = 0; // Counter for deadlocks
#define MAXMILK 1000000 // Maximum number of milk to buy
#define PROGRESS_INTERVAL 10000 // Progress interval

// Thread A function
void *threadA(void *arg) {
    while (milkBoughtSuccessfullyCounter < MAXMILK) {
        noteA = true; // Leave note A

        // Wait while note B is present
        while (noteB) {
            // Do nothing, wait for noteB to clear
        }

        // Check if milk is not available, then buy milk
        if (milkAvailable == 0) {
            milkAvailable++;
            if (milkAvailable > 1) {
                tooMuchMilkCounter++;
                milkAvailable = 0;
            } else {
                milkBoughtSuccessfullyCounter++;
                if (milkBoughtSuccessfullyCounter % PROGRESS_INTERVAL == 0) {
                    printf("Progress: %d milk bought successfully\n", milkBoughtSuccessfullyCounter);
                    printf("Progress: %d too much milk bought\n", tooMuchMilkCounter);
                    printf("Progress: %d deadlocks resolved\n\n", deadlockCounter);
                    fflush(stdout);
                }
            }
        }

        noteA = false; // Remove note A
        usleep(rand() % 10); // Sleep for a random short time
    }
    return NULL;
}

// Thread B function
void *threadB(void *arg) {
    while (milkBoughtSuccessfullyCounter < MAXMILK) {
        noteB = true; // Leave note B

        // Wait while note A is present
        while (noteA) {
            // Do nothing, wait for noteA to clear
        }

        // Check if milk is not available, then buy milk
        if (milkAvailable == 0) {
            milkAvailable++;
            if (milkAvailable > 1) {
                tooMuchMilkCounter++;
                milkAvailable = 0;
            } else {
                milkBoughtSuccessfullyCounter++;
                if (milkBoughtSuccessfullyCounter % PROGRESS_INTERVAL == 0) {
                    printf("Progress: %d milk bought successfully\n", milkBoughtSuccessfullyCounter);
                    printf("Progress: %d too much milk bought\n", tooMuchMilkCounter);
                    printf("Progress: %d deadlocks resolved\n\n", deadlockCounter);
                    fflush(stdout);
                }
            }
        }

        noteB = false; // Remove note B
        usleep(rand() % 10); // Sleep for a random short time
    }
    return NULL;
}

// Function to reset milkAvailable to 0 after a short time (simulate milk consumption)
void *resetMilk(void *arg) {
    while (milkBoughtSuccessfullyCounter < MAXMILK) {
        usleep(50); // Sleep for 0.05 seconds
        milkAvailable = 0;
    }
    return NULL;
}

// Function to monitor and resolve deadlocks
void *monitorDeadlock(void *arg) {
    while (milkBoughtSuccessfullyCounter < MAXMILK) {
        if (noteA && noteB) {
            // Deadlock detected
            deadlockCounter++;
            noteA = false;
            noteB = false;
        }
        usleep(10); // Sleep for a short time before checking again
    }
    return NULL;
}

int main() {
    pthread_t t1, t2, t3, t4;

    // Seed the random number generator
    srand(time(NULL));

    // Create threads
    pthread_create(&t1, NULL, threadA, NULL);
    pthread_create(&t2, NULL, threadB, NULL);
    pthread_create(&t3, NULL, resetMilk, NULL);
    pthread_create(&t4, NULL, monitorDeadlock, NULL);

    // Wait for threads to finish
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    pthread_join(t3, NULL);
    pthread_join(t4, NULL);

    // Check final state
    printf("\nMilk bought successfully: %d times\n", milkBoughtSuccessfullyCounter);
    printf("Too much milk bought: %d times\n", tooMuchMilkCounter);
    printf("Deadlocks resolved: %d times\n", deadlockCounter);

    return 0;
}

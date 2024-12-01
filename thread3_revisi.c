#include <stdio.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>

volatile bool noteA = false; 
volatile bool noteB = false; 
volatile bool noteC = false; 
volatile int milkAvailable = 0; 
volatile int tooMuchMilkCounter = 0; 
volatile int milkBoughtSuccessfullyCounter = 0; 
volatile int deadlockCounter = 0; 
#define MAXMILK 1000000 
#define PROGRESS_INTERVAL 100000 

void *threadA(void *arg) {
    while (milkBoughtSuccessfullyCounter <= MAXMILK) {
        noteA = true; 
   
        while (noteB || noteC) {
            // Do nothing
        }

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

        noteA = false; 
        usleep(rand() % 10); 
    }
    return NULL;
}

void *threadB(void *arg) {
    while (milkBoughtSuccessfullyCounter <= MAXMILK) {
        noteB = true; 
        
        while (noteA || noteC) {
            // Do nothing
        }

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

        noteB = false; 
        usleep(rand() % 10); 
    }
    return NULL;
}

void *threadC(void *arg) {
    while (milkBoughtSuccessfullyCounter <= MAXMILK) {
        noteC = true;

        while (noteA || noteB) {
            // Do nothing
        }

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

        noteC = false; 
        usleep(rand() % 10); 
    }
    return NULL;
}

void *resetMilk(void *arg) {
    while (milkBoughtSuccessfullyCounter <= MAXMILK) {
        usleep(50); 
        milkAvailable = 0;
    }
    return NULL;
}

void *monitorDeadlock(void *arg) {
    while (milkBoughtSuccessfullyCounter <= MAXMILK) {
        if (noteA && noteB && noteC) {
           
            deadlockCounter++;
            noteA = false;
            noteB = false;
            noteC = false;
        }
        usleep(10); 
    }
    return NULL;
}

int main() {
    pthread_t t1, t2, t3, t4, t5;

    srand(time(NULL));

    pthread_create(&t1, NULL, threadA, NULL);
    pthread_create(&t2, NULL, threadB, NULL);
    pthread_create(&t3, NULL, threadC, NULL);
    pthread_create(&t4, NULL, resetMilk, NULL);
    pthread_create(&t5, NULL, monitorDeadlock, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    pthread_join(t3, NULL);
    pthread_join(t4, NULL);
    pthread_join(t5, NULL);

    printf("Milk bought successfully: %d times\n", milkBoughtSuccessfullyCounter);
    printf("Too much milk bought: %d times\n", tooMuchMilkCounter);
    printf("Deadlocks resolved: %d times\n", deadlockCounter);

    return 0;
}

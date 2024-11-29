#include <stdio.h>

#include <pthread.h>

#include <stdbool.h>

// Shared variables
volatile bool noteA = false; // Represents "note left by A"
volatile bool noteB = false; // Represents "note left by B"
volatile bool noteC = false; // Represents "note left by C"
volatile int milkAvailable = 0; // Represents the amount of milk available
volatile int tooMuchMilkCounter = 0; // Counter for too much milk
volatile int milkBoughtSuccessfullyCounter = 0; // Counter for successful milk purchases

// Thread A function
void * threadA(void * arg) {
  noteA = true; // Leave note A

  // Wait while either note B or note C is present
  while (noteB || noteC) {
    // Do nothing
  }

  // Check if milk is not available, then buy milk
  if (milkAvailable == 0) {
    printf("Thread A: Buying milk\n");
    milkAvailable++;
    // Check if too much milk was bought
    if (milkAvailable > 1) {
      tooMuchMilkCounter++;
      milkAvailable = 0;
    } else {
      milkBoughtSuccessfullyCounter++;
    }
  }

  noteA = false; // Remove note A
  return NULL;
}

// Thread B function
void * threadB(void * arg) {
  noteB = true; // Leave note B

  // Wait while either note A or note C is present
  while (noteA || noteC) {
    // Do nothing
  }

  // Check if milk is not available, then buy milk
  if (milkAvailable == 0) {
    printf("Thread B: Buying milk\n");
    milkAvailable++;
    // Check if too much milk was bought
    if (milkAvailable > 1) {
      tooMuchMilkCounter++;
      milkAvailable = 0;
    } else {
      milkBoughtSuccessfullyCounter++;
    }

  }

  noteB = false; // Remove note B
  return NULL;
}

// Thread C function
void * threadC(void * arg) {
  noteC = true; // Leave note C

  // Wait while either note A or note B is present
  while (noteA || noteB) {
    // Do nothing
  }

  // Check if milk is not available, then buy milk
  if (milkAvailable == 0) {
    printf("Thread C: Buying milk\n");
    milkAvailable++;
    // Check if too much milk was bought
    if (milkAvailable > 1) {
      tooMuchMilkCounter++;
      milkAvailable = 0;
    } else {
      milkBoughtSuccessfullyCounter++;
    }
  }

  noteC = false; // Remove note C
  return NULL;
}

int main() {
  pthread_t t1, t2, t3;

  // Create threads
  pthread_create( & t1, NULL, threadA, NULL);
  pthread_create( & t2, NULL, threadB, NULL);
  pthread_create( & t3, NULL, threadC, NULL);

  // Wait for threads to finish
  pthread_join(t1, NULL);
  pthread_join(t2, NULL);
  pthread_join(t3, NULL);

  // Check final state
  printf("Milk bought successfully: %d times\n", milkBoughtSuccessfullyCounter);
  printf("Too much milk bought: %d times\n", tooMuchMilkCounter);

  return 0;
}

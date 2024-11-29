#include <pthread.h>
#include <stdio.h>

#define NUM_THREADS 3
#define ITERATIONS 1000000

pthread_mutex_t mutex;
volatile int counter = 0;

void *critical_section(void *arg) {
    int thread_id = *(int *)arg;

    for (int i = 0; i < ITERATIONS; i++) {
        pthread_mutex_lock(&mutex); // Memasuki critical section

        printf("Thread %d: entering critical section\n", thread_id);
        counter++;
        printf("Thread %d: counter = %d\n", thread_id, counter);
        printf("Thread %d: leaving critical section\n", thread_id);

        pthread_mutex_unlock(&mutex); // Keluar dari critical section
    }

    return NULL;
}

int main() {
    pthread_t threads[NUM_THREADS];
    int thread_ids[NUM_THREADS];

    pthread_mutex_init(&mutex, NULL);

    for (int i = 0; i < NUM_THREADS; i++) {
        thread_ids[i] = i;
        pthread_create(&threads[i], NULL, critical_section, &thread_ids[i]);
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    pthread_mutex_destroy(&mutex);

    printf("Final counter value: %d (Expected: %d)\n", counter, NUM_THREADS * ITERATIONS);

    return 0;
}

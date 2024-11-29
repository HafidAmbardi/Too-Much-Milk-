#include <stdio.h>
#include <pthread.h>

int milk = 0;       // Variabel bersama: jumlah susu
int noteA = 0;      // Catatan untuk thread A
int noteB = 0;      // Catatan untuk thread B
int noteC = 0;      // Catatan untuk thread C
int success = 0;    // Hitungan keberhasilan
int counter = 0;    // Counter untuk thread yang masuk critical section

pthread_mutex_t lock; // Mutex untuk mengontrol akses ke output

void* personA(void* arg) {
    noteA = 1;                // Tinggalkan catatan A
    while (noteB || noteC) {  // Tunggu jika ada catatan B atau C
        // Busy waiting
    }
    pthread_mutex_lock(&lock);
    printf("Thread A masuk critical section, counter = %d\n", ++counter);
    pthread_mutex_unlock(&lock);

    if (milk == 0) {          // Bagian kritis: cek dan beli susu
        milk = 1;
    }

    pthread_mutex_lock(&lock);
    printf("Thread A keluar critical section, counter = %d\n", --counter);
    pthread_mutex_unlock(&lock);

    noteA = 0;                // Hapus catatan A
    return NULL;
}

void* personB(void* arg) {
    noteB = 1;                // Tinggalkan catatan B
    while (noteA || noteC) {  // Tunggu jika ada catatan A atau C
        // Busy waiting
    }
    pthread_mutex_lock(&lock);
    printf("Thread B masuk critical section, counter = %d\n", ++counter);
    pthread_mutex_unlock(&lock);

    if (milk == 0) {          // Bagian kritis: cek dan beli susu
        milk = 1;
    }

    pthread_mutex_lock(&lock);
    printf("Thread B keluar critical section, counter = %d\n", --counter);
    pthread_mutex_unlock(&lock);

    noteB = 0;                // Hapus catatan B
    return NULL;
}

void* personC(void* arg) {
    noteC = 1;                // Tinggalkan catatan C
    while (noteA || noteB) {  // Tunggu jika ada catatan A atau B
        // Busy waiting
    }
    pthread_mutex_lock(&lock);
    printf("Thread C masuk critical section, counter = %d\n", ++counter);
    pthread_mutex_unlock(&lock);

    if (milk == 0) {          // Bagian kritis: cek dan beli susu
        milk = 1;
    }

    pthread_mutex_lock(&lock);
    printf("Thread C keluar critical section, counter = %d\n", --counter);
    pthread_mutex_unlock(&lock);

    noteC = 0;                // Hapus catatan C
    return NULL;
}

void test_case() {
    pthread_t threadA, threadB, threadC;

    milk = 0;   // Reset susu sebelum setiap tes
    noteA = 0;  // Reset catatan
    noteB = 0;
    noteC = 0;

    // Membuat thread
    pthread_create(&threadA, NULL, personA, NULL);
    pthread_create(&threadB, NULL, personB, NULL);
    pthread_create(&threadC, NULL, personC, NULL);

    // Menunggu ketiga thread selesai
    pthread_join(threadA, NULL);
    pthread_join(threadB, NULL);
    pthread_join(threadC, NULL);

    // Periksa hasil
    if (milk == 1) {
        success++;
    }
}

int main() {
    int total_tests = 1000000;

    // Inisialisasi mutex
    pthread_mutex_init(&lock, NULL);

    for (int i = 0; i < total_tests; i++) {
        // Cetak angka tes saat ini dengan mengganti yang sebelumnya
        printf("\rRunning test case: %d/%d", i + 1, total_tests);
        fflush(stdout); // Pastikan output langsung muncul
        test_case();
    }

    // Cetak hasil akhir
    printf("\nTes selesai. Keberhasilan: %d/%d\n", success, total_tests);

    // Hancurkan mutex
    pthread_mutex_destroy(&lock);

    return 0;
}

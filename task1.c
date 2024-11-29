#include <stdio.h>
#include <pthread.h>

int milk = 0;       // Variabel bersama: jumlah susu
int noteA = 0;      // Catatan untuk thread A
int noteB = 0;      // Catatan untuk thread B
int success = 0;    // Hitungan keberhasilan

void* personA(void* arg) {
    printf("Thread A: Leaving note\n");
    noteA = 1;                // Tinggalkan catatan A
    while (noteB) {           // Tunggu jika ada catatan B
        printf("Thread A: Waiting for noteB to be 0\n");
    }
    if (milk == 0) {          // Bagian kritis: cek dan beli susu
        printf("Thread A: Buying milk\n");
        milk = 1;
    }
    printf("Thread A: Removing note\n");
    noteA = 0;                // Hapus catatan A
    return NULL;
}

void* personB(void* arg) {
    printf("Thread B: Leaving note\n");
    noteB = 1;                // Tinggalkan catatan B
    while (noteA) {           // Tunggu jika ada catatan A
        printf("Thread B: Waiting for noteA to be 0\n");
    }
    if (milk == 0) {          // Bagian kritis: cek dan beli susu
        printf("Thread B: Buying milk\n");
        milk = 1;
    }
    printf("Thread B: Removing note\n");
    noteB = 0;                // Hapus catatan B
    return NULL;
}

void test_case() {
    pthread_t threadA, threadB;

    milk = 0;   // Reset susu sebelum setiap tes
    noteA = 0;  // Reset catatan
    noteB = 0;

    // Membuat thread
    pthread_create(&threadA, NULL, personA, NULL);
    pthread_create(&threadB, NULL, personB, NULL);

    // Menunggu kedua thread selesai
    pthread_join(threadA, NULL);
    pthread_join(threadB, NULL);

    // Periksa hasil
    if (milk == 1) {
        success++;
    }
    printf("Final milk count: %d\n", milk); // Cetak jumlah susu setelah pengujian
    printf("\n");
}

int main() {
    int total_tests = 1000000;

    for (int i = 0; i < total_tests; i++) {
        printf("Running test case %d/%d\n", i + 1, total_tests);
        test_case();
    }

    // Cetak hasil akhir
    printf("\nTes selesai. Keberhasilan: %d/%d\n", success, total_tests);
    return 0;
}

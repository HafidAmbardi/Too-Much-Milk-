#include <stdio.h>
#include <pthread.h>

int milk = 0;       // Variabel bersama: jumlah susu
int noteA = 0;      // Catatan untuk thread A
int noteB = 0;      // Catatan untuk thread B
int success = 0;    // Hitungan keberhasilan

void* personA(void* arg) {
    noteA = 1;                // Tinggalkan catatan A
    while (noteB) {           // Tunggu jika ada catatan B
        // Busy waiting
    }
    if (milk == 0) {          // Bagian kritis: cek dan beli susu
        milk = 1;
    }
    noteA = 0;                // Hapus catatan A
    return NULL;
}

void* personB(void* arg) {
    noteB = 1;                // Tinggalkan catatan B
    while (noteA) {           // Tunggu jika ada catatan A
        // Busy waiting
    }
    if (milk == 0) {          // Bagian kritis: cek dan beli susu
        milk = 1;
    }
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
}

int main() {
    int total_tests = 1000000;

    for (int i = 0; i < total_tests; i++) {
        // Cetak angka tes saat ini dengan mengganti yang sebelumnya
        printf("\rRunning test case: %d/%d", i + 1, total_tests);
        fflush(stdout); // Pastikan output langsung muncul
        test_case();
    }

    // Cetak hasil akhir
    printf("\nTes selesai. Keberhasilan: %d/%d\n", success, total_tests);
    return 0;
}

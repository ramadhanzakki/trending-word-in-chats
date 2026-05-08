#include "header.h"
#include <stdio.h>

int main() {
    Arena arena;
    HashTable ht;

    // Inisialisasi arena memory
    arenaInit(&arena, 286);

    // Inisialisasi hash table
    hashTableInit(&arena, &ht, 10);

    // Simulasi input kata
    hashTableProcessWord(&arena, &ht, "apel");
    hashTableProcessWord(&arena, &ht, "mangga");
    hashTableProcessWord(&arena, &ht, "apel");
    hashTableProcessWord(&arena, &ht, "jeruk");
    hashTableProcessWord(&arena, &ht, "apel");
    hashTableProcessWord(&arena, &ht, "mangga");
    hashTableProcessWord(&arena, &ht, "pisang");

    // Tampilkan trending word
    printf("=== TRENDING WORD ===\n\n");
    displayTrending(&arena, &ht);

    // Tampilkan visualisasi arena
    printf("\n=== ARENA DUMP ===\n\n");
    arenaDump(&arena);

    return 0;
}
#include "header.h"
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

int main() {
    Arena arena;
    HashTable ht;

    arenaInit(&arena, ARENA_MAX_SIZE);
    hashTableInit(&arena, &ht, 10);

    int choice;
    char inputWord[100];
    char inputFile[100];

    while(true) {
        printf("\n=== MENU TRENDING CHAT ===\n");
        printf("1. Masukkan kata\n");
        printf("2. Import chats from csv\n");
        printf("3. Tampilkan trending word\n");
        printf("4. Tampilkan visualisasi arena\n");
        printf("5. Reset arena\n");
        printf("6. Tampilkan Hash Table\n");
        printf("0. Keluar\n");
        printf("Pilih menu: ");
        
        // Mencegah error kalau user input huruf pas milih menu
        if (scanf("%d", &choice) != 1) {
            while(getchar() != '\n'); 
            printf("Input ga valid, masukin angka ya!\n");
            continue;
        }
        getchar(); // Clear buffer newline

        switch(choice) {
            case 1:
                printf("Ketik kalimat atau kata: ");
                fgets(inputWord, sizeof(inputWord), stdin);
                
                // Ilangin karakter newline (\n) di akhir input
                inputWord[strcspn(inputWord, "\n")] = 0; 

                if(strlen(inputWord) > 0) {
                    processSentence(&arena, &ht, inputWord);
                    printf("-> Input '%s' berhasil diproses dan difilter!\n", inputWord);
                }
                break;
            
            case 2:
                printf("\n=== IMPORTING CHATS ===\n\n");
                fgets(inputFile, sizeof(inputFile), stdin);
                inputFile[strcspn(inputFile, "\n")] = 0;
                processFile(&arena, &ht, inputFile);
                break;
                
            case 3:
                printf("\n=== TRENDING WORD ===\n\n");
                displayTop10(&arena, &ht);
                break;
                
            case 4:
                printf("\n=== ARENA DUMP ===\n\n");
                arenaDump(&arena);
                break;

            case 5:
                printf("\n=== ARENA RESET ===\n\n");
                arenaReset(&arena);
                hashTableInit(&arena, &ht, 10);
                printf("[INFO] : Memori berhasil direset");
                break;

            case 6:
                printf("\n=== Hash Table ===\n\n");
                displayHashTable(&arena, &ht);
                break;
                
            case 0:
                printf("Sip, keluar dari program...\n");
                return 0;
                
            default:
                printf("Pilihan ga ada di menu bro, coba lagi.\n");
        }
    }

    return 0;
}
#include "header.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>

static unsigned char global_buffer[ARENA_MAX_SIZE];

void arenaInit(Arena *a, size_t size){
    if (size > ARENA_MAX_SIZE){
        printf("[WARNING] => Size terlalu besar\n");
        return;
    }

    a->buffer = global_buffer;
    a->capacity = size;
    a->offset = 0;
}

int arenaAlloc(Arena *a, size_t size){
    if (a->offset + size > a->capacity){
        printf("[WARNING] => Memory tidak cukup\n");
        return -1;
    }

    size_t oldOffset = a->offset;
    a->offset += size;

    return oldOffset;
}

void* arenaGet(Arena *a, size_t offset){
    if (offset >= a->offset){
        return NULL;
    }

    return (void*)(a->buffer + offset);
}

void arenaReset(Arena *a){
    a->offset = 0;
}

void arenaDump(Arena *a){
    printf("=== VISUALISASI ARENA ===\n");
    printf("Kapasitas = %zu | Offset = %zu\n\n",
            a->capacity,
            a->offset);

    double percentage = ((double)a->offset/a->capacity) * 100.0;
    int box = 20;
    int filledBox = (int)((percentage / 100.0) * box);

    if (filledBox > box){
        filledBox = box;
    }

    printf("Memory : [");

    for (size_t i = 0; i < box; i++){
        if (i < filledBox){
            printf("#");
        }else {
            printf(".");
        }
    }

    printf("] %.2f%%\n\n", percentage);
}

void hashTableInit(Arena *a, HashTable *ht, size_t size){
    ht->size = size;

    ht->tableOffset = arenaAlloc(a, size * sizeof(int));

    int* table = (int*)arenaGet(a, ht->tableOffset);

    for (size_t i = 0; i < size; i++){
        table[i] = -1;
    }
}

unsigned long hash_djb2(const char *str){
    unsigned long hash = 5381;

    int c;

    while ((c = *str++)){
        hash = ((hash << 5) + hash) + c;
    }

    return hash;
}

void hashTableProcessWord(Arena *a, HashTable *ht, const char *word){

    unsigned long hash = hash_djb2(word);
    int index = hash % ht->size;
    int* table = (int*)arenaGet(a, ht->tableOffset);
    int currentOffset = table[index];

    // cek apakah kata sudah ada
    while (currentOffset != -1){
        NodeWord* currentNode = (NodeWord*)arenaGet(a, currentOffset);
        char* existingWord = (char*)arenaGet(a, currentNode->wordOffset);

        if (strcmp(existingWord, word) == 0){
            currentNode->count++;
            return;
        }
        currentOffset = currentNode->nextOffset;
    }

    // simpan string
    int wordOffset = arenaAlloc(a, strlen(word) + 1);
    char* storedWord = (char*)arenaGet(a, wordOffset);
    strcpy(storedWord, word);

    // buat node baru
    int newNodeOffset = arenaAlloc(a, sizeof(NodeWord));
    NodeWord* newNode = (NodeWord*)arenaGet(a, newNodeOffset);

    newNode->wordOffset = wordOffset;
    newNode->count = 1;

    // linked list
    newNode->nextOffset = table[index];
    table[index] = newNodeOffset;
}

void displayHashTable(Arena *a, HashTable *ht){
    int* table = (int*)arenaGet(a, ht->tableOffset);

    for (int i = 0; i < ht->size; i++){
        int currentOffset = table[i];
        printf("Bucket[%d]", i);

        while (currentOffset != -1){
            NodeWord* node = (NodeWord*)arenaGet(a, currentOffset);
            char* word = (char*)arenaGet(a, node->wordOffset);

            printf(" -> %s (%d)", word, node->count);

            currentOffset = node->nextOffset;
        }

        printf("\n");
    }
}

void processSentence(Arena *a, HashTable *ht, const char *sentence) {
    char wordBuf[128];
    int wordLen = 0;
    
    for (int i = 0; sentence[i] != '\0'; i++) {
        int c = sentence[i];
        if (isalnum(c)) {
            if (wordLen < 127) {
                wordBuf[wordLen++] = tolower(c);
            }
        } else {
            if (wordLen > 0) {
                wordBuf[wordLen] = '\0';
                hashTableProcessWord(a, ht, wordBuf);
            }
            wordLen = 0;
        }
    }
    
    if (wordLen > 0) {
        wordBuf[wordLen] = '\0';
        hashTableProcessWord(a, ht, wordBuf);
    }
}

void processFile(Arena *a, HashTable *ht, const char *filename){
    FILE *file = fopen(filename, "r");

    if (!file){
        printf("[ERROR] => File tidak bisa dibuka\n");
        return;
    }
    
    char wordBuf[128];
    int wordLen = 0;
    int totalWords = 0;
    int c;

    printf("[INFO] => Membaca file\n");

    while ((c = fgetc(file)) != EOF){
        if (isalnum(c)){
            if (wordLen < 127){
                wordBuf[wordLen++] = tolower(c);
            }
        } else {
            if (wordLen >= 1){
                wordBuf[wordLen] = '\0';
                hashTableProcessWord(a, ht, wordBuf);
                totalWords++;
            }
            wordLen = 0;
        }
    }

    printf("[INFO] => Berhasil membaca file\n");
    printf("[INFO] Total kata pada file = %d\n", totalWords);

    fclose(file);
}

void displayTop10(Arena *a, HashTable *ht) {
    NodeWord *top10[10];

    for (int i = 0; i < 10; i++){
        top10[i] = NULL;
    }
    
    int *table = (int *)arenaGet(a, ht->tableOffset);

    if (table == NULL)
    {
        printf("[WARNING] : Tidak ada data yang dapat ditampilkan\n");
        return;
    }

    for (int i = 0; i < ht->size; i++) {
        int currentOffset = table[i];

        while (currentOffset != -1){
            NodeWord *node = (NodeWord*)arenaGet(a,currentOffset);
        
            for (int j = 0; j < 10; j++){
                if (top10[j] == NULL || node->count > top10[j]->count){
                    for (int k = 0; k > j; k--){
                        top10[k] = top10[k-1];
                    }
                    top10[j] = node;
                    break;
                }
            }
            currentOffset = node->nextOffset;
        }
    }

    if (top10[0] == NULL){
        printf("[WARNING] : Belum ada data yang tersimpan\n");
    } else {
        for (int i = 0; i < 10; i++){
            if (top10[i] != NULL){
                char *word = (char*)arenaGet(a,top10[i]->wordOffset);
                printf("%d. %s (%d)\n", i + 1, word, top10[i]->count);
            }
        }
    }
}

void searchWord(Arena *arena, HashTable *ht, const char *sentence){
    if (sentence = ""){
        printf("[WARNING] : Kamu belum mengisi kolom pencarian\n");
        return;
    }

    char wordBuf[128];
    int wordLen = 0;

    for (int i = 0; sentence[i] != '\0'; i++){
        int c = sentence[i];

        if (isalnum(c)){
            if (wordLen < 127){
                wordBuf[wordLen++] = tolower(c);
            }
        } else {
            if (wordLen > 0){
                wordBuf[wordLen] = '\0';
            }
            wordLen = 0;
        }
    }

    if (wordLen > 0){
        wordBuf[wordLen] = '\0';
    }
    
    unsigned long hash = hash_djb2(wordBuf);
    int index = hash % ht->size;

    int *table = (int*)arenaGet(arena, ht->tableOffset);
    int currentOffset = table[index];

    while (currentOffset != -1){
        NodeWord *node = (NodeWord*)arenaGet(arena, currentOffset);
        char *word = (char*)arenaGet(arena, node->wordOffset);

        if (strcmp(word, wordBuf) == 0){
            printf("Berhasil menemukan kata \"%s\" pada table ke-%d dengan jumlah kata %d kata\n",
                word,
                index,
                node->count
            );
            return;
        }
        
        currentOffset = node->nextOffset;
    }

    printf("[WARNING] : Kata tidak ditemukan\n");
    
}
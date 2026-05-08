#include "header.h"
#include <stdio.h>
#include <string.h>

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
    if (offset > a->offset){
        return NULL;
    }

    return (void*)(a->buffer + offset);
}

void arenaReset(Arena *a){
    a->offset = 0;
}

void arenaDump(Arena *a){
    const int bytesPerRow = 8;

    printf("=== VISUALISASI ARENA ===\n");
    printf("Capacity = %zu | Offset = %zu\n\n",
           a->capacity,
           a->offset);

    for (size_t i = 0; i < a->capacity; i++){

        if (i % bytesPerRow == 0){
            printf("%04zu : ", i);
        }

        if (i < a->offset){
            printf("# ");
        } else {
            printf(". ");
        }

        if ((i + 1) % bytesPerRow == 0){
            printf("\n");
        }
    }

    printf("\n");
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

        NodeWord* currentNode =
            (NodeWord*)arenaGet(a, currentOffset);

        char* existingWord =
            (char*)arenaGet(a, currentNode->wordOffset);

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

    NodeWord* newNode =
        (NodeWord*)arenaGet(a, newNodeOffset);

    newNode->wordOffset = wordOffset;
    newNode->count = 1;

    // linked list
    newNode->nextOffset = table[index];

    table[index] = newNodeOffset;
}

void displayTrending(Arena *a, HashTable *ht){

    int* table = (int*)arenaGet(a, ht->tableOffset);

    for (int i = 0; i < ht->size; i++){

        int currentOffset = table[i];

        while (currentOffset != -1){

            NodeWord* node =
                (NodeWord*)arenaGet(a, currentOffset);

            char* word =
                (char*)arenaGet(a, node->wordOffset);

            printf("Bucket[%d] -> %s (%d)\n",
                   i,
                   word,
                   node->count);

            currentOffset = node->nextOffset;
        }
    }
}
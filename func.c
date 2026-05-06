#include "header.h"
#include <stdio.h>

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
    
    size_t offsetTime = a->offset;
    a->offset += size;
    return offsetTime;
}

void* arenaGet(Arena *a, size_t offset){
    if (offset > a->offset) return NULL;
    return (void*)(a->buffer + offset);
}

void arenaReset(Arena *a){
    a->offset = 0;
}

void arenaDump(Arena *a){
    const int bytesPerRow = 8;

    printf("=== VISUALISASI ARENA ===\n");
    printf("Arena Dump (Capacity = %zu, offset = %zu)\n\n", a->capacity, a->offset);

    for (size_t i = 0; i < a->capacity; i++){
        if (i % bytesPerRow == 0){
            printf("%04zu", i);
        }

        if (i == a->offset){
            printf("| ");
        } else if (i < a->offset){
            printf("# ");
        } else {
            printf(". ");
        }
        
        if ((i + 1) % bytesPerRow == 0){
            printf("\n");
        }
    }
    
    if (a->capacity % bytesPerRow != 0){
        printf("\n");
    }
}

void hashTableInit(Arena *a, HashTable *ht, size_t size){
    ht->size = size;
    ht->tableOffset = arenaAlloc(a, size * sizeof(int));

    if (ht->tableOffset == -1){
        return;
    }
    

    int* tableArray = (int*) arenaGet(a, ht->tableOffset);

    for (size_t i = 0; i < size; i++){
        tableArray[i] = -1;
    }
}

void hashTableProcessWord(Arena *a, HashTable *ht, const char *word){
    unsigned long hashWord = hash_djb2(word);
    int index = hashWord % ht->size;

    int* tableArray = (int*)arenaGet(a, ht->tableOffset);
    int currentOffset = tableArray[index];

    while (currentOffset != -1)
    {
        NodeWord* currentNode = (NodeWord*)arenaGet(a, currentOffset);
        char* nodeWord = (char*)arenaGet(a, currentNode->wordOffset);
        
        if (strcmp(nodeWord, word) == 0)
        {
            currentNode->nextOffset++;
            return;
        }
        
        currentOffset = currentNode->nextOffset;
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
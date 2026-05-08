#ifndef HEADER_H
#define HEADER_H

#include <stddef.h>

#define ARENA_MAX_SIZE 286

typedef struct {
    unsigned char *buffer;
    size_t capacity;
    size_t offset;
} Arena;

typedef struct {
    int wordOffset;
    int count;
    int nextOffset;
} NodeWord;

typedef struct {
    int tableOffset;
    int size;
} HashTable;

// Arena Functions
void arenaInit(Arena *a, size_t size);
int arenaAlloc(Arena *a, size_t size);
void* arenaGet(Arena *a, size_t offset);
void arenaReset(Arena *a);
void arenaDump(Arena *a);

// Hash Table Functions
void hashTableInit(Arena *a, HashTable *ht, size_t size);
unsigned long hash_djb2(const char *str);
void hashTableProcessWord(Arena *a, HashTable *ht, const char *word);
void displayTrending(Arena *a, HashTable *ht);

#endif
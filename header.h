#ifndef HEADER_H
#define HEADER_H

#include <stddef.h>

#define ARENA_MAX_SIZE 3200

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
void displayHashTable(Arena *a, HashTable *ht);

// File & Thrending Functions
void processFile(Arena *a, HashTable *ht, const char *filename);
void processSentence(Arena *a, HashTable *ht, const char *sentence);
void displayTop10(Arena *a, HashTable *ht);
void countWord(Arena *arena, HashTable *ht);

#endif
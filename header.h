#ifndef TRENDING_CHAT_H
#define TRENDING_CHAT_H

#include <stdint.h>
#include <stddef.h>

#define ARENA_MAX_SIZE 1024

// Arena
typedef struct {
    unsigned char* buffer;
    size_t capacity;
    size_t offset;
} Arena;

// Linked List untuk penyimpanan kata dan frekuensinya
typedef struct {
    int wordOffset;
    int count;
    int nextOffset;
} NodeWord;

// Hash Table 
typedef struct {
    int tableOffset;
    int size;
} HashTable;

// Function Arena Allocator
void arenaInit(Arena *a, size_t size);
int arenaAlloc(Arena *a, size_t size);
void arenaGet(Arena *a, size_t offset);
void arenaReset(Arena *a);
void arenaDUmp(Arena *a);

// Function hash table
void hashTableInit(Arena *a, HashTable *ht, size_t size);
void hashTableProcessWord(Arena *a, HashTable *ht, const char *word);
void displayTrending(Arena *a, HashTable *ht);

// Function hashing
unsigned long hash_djb2(const char *str);

#endif
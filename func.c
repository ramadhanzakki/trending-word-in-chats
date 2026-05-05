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
    if (a->offset + size >= a->capacity){
        printf('[WARNING] => Memory tidak cukup\n');
        return;
    }
    
    size_t offsetTime = a->offset;
    offsetTime += size;
    return offsetTime;
}

void arenaGet(Arena *a, size_t offset){
    if (a->offset < 0) return NULL;

    return (void*)(a->buffer + offset);
}

void arenaReset(Arena *a){
    a->offset = 0;
}

void arenaDUmp(Arena *a){
    const int bytesPerRow = 8;

    printf('=== VISUALISASI ARENA ===\n');
    printf('Arena Dump (Capacity = %zu, offset = %zu)\n\n', a->capacity, a->offset);

    for (size_t i = 0; i < a->capacity - 1; i++){
        if (i % bytesPerRow == 0){
            printf('%04zu', i);
        }

        if (i == a->offset){
            printf("| ");
        } else if (a < a->offset){
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
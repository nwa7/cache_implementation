/*
 * cache_impl.h
 *
 * 20493-01 Computer Architecture
 * Term Project on Implementation of Cache Mechanism
 *
 * Skeleton Code Prepared by Prof. HyungJune Lee
 * Nov 15, 2023
 *
 */
#pragma once
/* DO NOT CHANGE THE FOLLOWING DEFINITIONS EXCEPT 'DEFAULT_CACHE_ASSOC */

#ifndef _CACHE_IMPL_H_
#define _CACHE_IMPL_H_

#define WORD_SIZE_BYTE 4                // 1 word = 4 bytes
#define DEFAULT_CACHE_SIZE_BYTE 32      // cache size = 32 bytes
#define DEFAULT_CACHE_BLOCK_SIZE_BYTE 8 // block size = 8 bytes
#define DEFAULT_CACHE_ASSOC                                                    \
  1 /* This can be changed to 1(for direct mapped cache) or 4(for fully assoc  \
       cache) */
#define DEFAULT_MEMORY_SIZE_WORD                                               \
  128                           // main memory size = 128 words, 128 * 4 bytes
#define CACHE_ACCESS_CYCLE 1    // cache access time = 1 cycle
#define MEMORY_ACCESS_CYCLE 100 // main memory access time = 100 cycles
#define CACHE_SET_SIZE                                                         \
  ((DEFAULT_CACHE_SIZE_BYTE) /                                                 \
   (DEFAULT_CACHE_BLOCK_SIZE_BYTE * DEFAULT_CACHE_ASSOC))

/* Function Prototypes */
void init_memory_content();
void init_cache_content();
void print_cache_entries();
int check_cache_data_hit(void *addr, char type);
int access_memory(void *addr, char type);

/* Cache Entry Structure */
typedef struct cache_entry {
  int valid;     // present = 1, not present = 0
  int tag;       // tag of the stored data
  int timestamp; /* for approximate LRU: it should be equal to data access so
                    far, most recent access time */
  char data[DEFAULT_CACHE_BLOCK_SIZE_BYTE];
} cache_entry_t;

#endif

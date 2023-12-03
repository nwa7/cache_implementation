/*
 * cache.c
 *
 * 20493-01 Computer Architecture
 * Term Project on Implementation of Cache Mechanism
 *
 * Skeleton Code Prepared by Prof. HyungJune Lee
 * Nov 15, 2023
 *
 */

#include "cache_impl.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


/* get the global variables in other files */

extern int num_cache_hits;
extern int num_cache_misses;

extern int num_bytes;
extern int num_access_cycles;

extern int global_timestamp;

/* new variable used in this file */
cache_entry_t cache_array[CACHE_SET_SIZE][DEFAULT_CACHE_ASSOC];
// data in cache with size (number of sets X number of associations)
int memory_array[DEFAULT_MEMORY_SIZE_WORD]; // data in memory with size of 128
                                            // words

/* DO NOT CHANGE THE FOLLOWING FUNCTION */
void init_memory_content() {
  unsigned char sample_upward[16] = {0x001, 0x012, 0x023, 0x034, 0x045,
                                     0x056, 0x067, 0x078, 0x089, 0x09a,
                                     0x0ab, 0x0bc, 0x0cd, 0x0de, 0x0ef};
  unsigned char sample_downward[16] = {0x0fe, 0x0ed, 0x0dc, 0x0cb, 0x0ba,
                                       0x0a9, 0x098, 0x087, 0x076, 0x065,
                                       0x054, 0x043, 0x032, 0x021, 0x010};
  int index, i = 0, j = 1, gap = 1;

  for (index = 0; index < DEFAULT_MEMORY_SIZE_WORD; index++) {
    memory_array[index] = (sample_upward[i] << 24) | (sample_upward[j] << 16) |
                          (sample_downward[i] << 8) | (sample_downward[j]);
    if (++i >= 16)
      i = 0; // cycle
    if (++j >= 16)
      j = 0; // cycle

    if (i == 0 && j == i + gap) // difference of i and j == gap
      j = i + (++gap);          // increases 1 gap and new j for each cycle

    printf("mem[%d] = %#x\n", index, memory_array[index]);
  }
}

/* DO NOT CHANGE THE FOLLOWING FUNCTION */
// This function is to initialize contents of the cache memory
void init_cache_content() {
  int i, j;

  // initialize cache data
  for (i = 0; i < CACHE_SET_SIZE; i++) {
    for (j = 0; j < DEFAULT_CACHE_ASSOC; j++) {
      cache_entry_t *pEntry = &cache_array[i][j];
      pEntry->valid = 0;     // invalid
      pEntry->tag = -1;      // no tag
      pEntry->timestamp = 0; // no access trial
    }
  }
}

/* DO NOT CHANGE THE FOLLOWING FUNCTION */
/* This function is an utility function to print all the cache entries. It will
 * be useful for your debugging */
void print_cache_entries() {
  int i, j, k;
  printf("ENTRY >> \n");
  // for each set
  for (i = 0; i < CACHE_SET_SIZE; i++) {
    printf("[Set %d] ", i);
    // for each entry in a set
    for (j = 0; j < DEFAULT_CACHE_ASSOC; j++) {
      cache_entry_t *pEntry = &cache_array[i][j];
      printf("V: %d Tag: %#x Time: %d Data: ", pEntry->valid, pEntry->tag,
             pEntry->timestamp);
      for (k = 0; k < DEFAULT_CACHE_BLOCK_SIZE_BYTE; k++) {
        printf("%#x(%d) ", pEntry->data[k], k);
      }
      printf("\t");
    }
    printf("\n");
  }
}

// this function is to return the data in the cache
int check_cache_data_hit(void *addr, char type) {
  char *addres = addr;
  int address=atoi(addres);
  int template=address;
  address=address/8;
/*  int block_address = address / DEFAULT_CACHE_BLOCK_SIZE_BYTE;
  int tag = block_address >>
            ((4 / DEFAULT_CACHE_ASSOC) -
             1); // the 4 could also be the blocknumber, 4 bytes for 1 word
  */  
  int temp1 = 1 << ((4 / DEFAULT_CACHE_ASSOC)-1);
  int tag=address/4*DEFAULT_CACHE_ASSOC;//>>(temp1-1);
  int set = address % (4/DEFAULT_CACHE_ASSOC);

  /* add this cache access cycle to global access cycle */
  /* check all entries in a set */
  int j;
  for (j = 0; j < DEFAULT_CACHE_ASSOC; j++) {
    cache_entry_t *pEntry = &cache_array[set][j];

    // check if the tag matches and the entry is valid
    if (pEntry->tag == tag && pEntry->valid == 1) {
      pEntry->timestamp = global_timestamp;
      // difference depending on type
      int memo=pEntry->data[template/4];
      memo+=(pEntry->data[(template/DEFAULT_MEMORY_SIZE_WORD)+1]<<(32-(8*(template%DEFAULT_MEMORY_SIZE_WORD))));
  
 int f;
  if(type=='b'){
     memo=memo&0xff;
  }
  if(type=='h'){
     memo=memo&0xffff;
  }
  if(type=='w'){
     memo=memo&0xffffffff;
  }
      return memo;
    }
  }
  return -1; // if the data is not in the cache
}

// This function is to find the entry index in set for copying to cache
int find_entry_index_in_set(int cache_index) {
  int entry_index;

  if (DEFAULT_CACHE_ASSOC == 1) { // If set has only 1 entry, return index 0
    return 0;
  }

  /* Check if there exists any empty cache space by checking 'valid' */
  int j;
  // int i; unusued param

  for (j = 0; j < DEFAULT_CACHE_ASSOC; j++) {
    cache_entry_t *pEntry = &cache_array[cache_index][j];
    if (pEntry->valid == 0) {
      return j;
    }
  }
  /* Otherwise, search over all entries to find the least recently used entry
   * by checking timestamp */
  int timestamp_min = -1;
  for (j = 0; j < DEFAULT_CACHE_ASSOC; j++) {
    cache_entry_t *pEntry = &cache_array[cache_index][j];
    if (timestamp_min == -1 || timestamp_min > pEntry->timestamp) {
      entry_index = j;
      timestamp_min = pEntry->timestamp;
    }
  }
  // return the cache index for copying from memory
  return entry_index;
}

int access_memory(void *addr, char type) {
  char *address = addr;
  int temp=atoi(address);
  int template=temp;
  temp=temp/8;
/*  printf("%d", temp);
  printf("hh");
  int block_address = temp / DEFAULT_CACHE_BLOCK_SIZE_BYTE;
  int tag = block_address >>
            ((4 / DEFAULT_CACHE_ASSOC) -
             1); // the 4 could also be the blocknumber, 4 bytes for 1 word
*/
  int temp1 = 1 << ((4 / DEFAULT_CACHE_ASSOC)-1);
  int set = temp % (4/DEFAULT_CACHE_ASSOC);
  int tag=temp/4*DEFAULT_CACHE_ASSOC;//>>(temp1-1);
  printf("%d", tag);
  printf("hh");
  printf("%d", set);
  printf("hh");

  /* add this cache access cycle to global access cycle */
  /* check all entries in a set */
  
  int memo=memory_array[template/4];
  memo>>(8*(template%4));
  //printf(" mem %d", memo);
  memo+=(memory_array[(template/DEFAULT_MEMORY_SIZE_WORD)+1]<<(32-(8*(template%DEFAULT_MEMORY_SIZE_WORD))));
  
  //printf(" mem %d", memo);
 int f;
  if(type=='b'){
     memo=memo&0xff;
     
  //printf(" mem %d", memo);
  }
  if(type=='h'){
     memo=memo&0xffff;
     
  //printf(" mem %d", memo);
  }
  if(type=='w'){
     memo=memo&0xffffffff;
     
  //printf(" mem %d", memo);
  }
  int block=template/8;
  block*=2;
  int index=find_entry_index_in_set(set);
    cache_entry_t *pEntry = &cache_array[set][index]; 

    for(int k=0;k<(DEFAULT_CACHE_BLOCK_SIZE_BYTE/2);k++){
    pEntry->data[3-k]=memory_array[block]>>(24-(8*k));
    pEntry->data[7-k]=memory_array[block+1]>>(24-(8*k));

    }
    pEntry->valid=1;
    pEntry->timestamp=global_timestamp;
    pEntry->tag=tag;
    //printf("%li", memo);
    return memo;
  // get the entry index
  /* Add this main memory access cycle to global access cycle */
  /* Fetch the data from the main memory and copy them to the cache */
  /* void *addr: addr is byte address, whereas your main memory address is
   * word address due to 'int memory_array[]' */

  /* You need to invoke find_entry_index_in_set() for copying to the cache */

  /* Return the accessed data with a suitable type (b, h, w)*/
 // if the data is not in the memory
}
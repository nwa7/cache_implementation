/*
 * main.c
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
#include <stdlib.h>
#include <string.h>

int num_cache_hits = 0;
int num_cache_misses = 0;

int num_bytes = 0;
int num_access_cycles = 0;

int global_timestamp = 0;

int retrieve_data(void *addr, char data_type) {
  char *ad = addr;
  int value_returned = -1;

  // checking if data is in cache
  value_returned = check_cache_data_hit(ad, data_type);
  num_access_cycles += CACHE_ACCESS_CYCLE; // adding the number of clock cycles
                                           // to go through cache
  if (value_returned != -1) {
    num_cache_hits++;      // Hit
    return value_returned; // returning address in cache
  }

  // if it's not in the cache checking if it is in memory
  value_returned = access_memory(ad, data_type);
  num_access_cycles += MEMORY_ACCESS_CYCLE; // adding the number of clock cycles
                                            // to go through memory
  if (value_returned != -1) {               // check data in memory
    num_cache_misses++;                     // Miss
    return value_returned;                  // returning address in memory

  } else {

    printf("data neither in cache nor memory");
  }

  return value_returned;
}

int main(void) {
  FILE *ifp = NULL, *ofp = NULL;
  unsigned long int access_addr;
  char access_type; // 'b'(byte), 'h'(halfword), or 'w'(word)

  init_memory_content(); // 1) Invoke init_memory_content
  init_cache_content();  // 2) Invoke init_cache_content

  ifp = fopen("access_input_3.txt", "r"); // 3) open input file in reading mode
  if (ifp == NULL) {
    printf("Can't open input file\n");
    return -1;
  }
  ofp = fopen("access_output_3.txt", "w"); // open output file in writing mode
  if (ofp == NULL) {
    printf("Can't open output file\n");
    fclose(ifp);
    return -1;
  }

  fprintf(ofp, "[Accessed Data]\n");

  char line[256];

  while (fgets(line, sizeof(line), ifp) !=
         NULL) { // we're iterating on each line in ifp
    sscanf(line, "%lx %c", &access_addr, &access_type);
    int accessed_data = retrieve_data(
        line, access_type); // call retrieve data to 4) Read each line
    fprintf(
        ofp, "%lx %c 0x%x \n", access_addr, access_type,
        accessed_data); // print addr and data_type accessed in the output file,

    global_timestamp++; // incrementing number of accesses
  }
  // hit ratio
  double hit_ratio =
      (double)num_cache_hits / (num_cache_misses + num_cache_hits);
  fprintf(ofp, "Hit ratio = %.2f    ( %d / %d ) \n", hit_ratio, num_cache_hits,
          num_cache_misses +
              num_cache_hits); // number of hits / number of accesses

  // bandwidth
  double bandwidth = (double)num_bytes / num_access_cycles;
  fprintf(ofp, "Bandwidth = %.2f    ( %d / %d ) \n", bandwidth, num_bytes,
          num_access_cycles); // number of bytes / number of clock cycles

  // closing documents
  fclose(ifp);
  fclose(ofp);

  print_cache_entries(); // print the final cache entries
  return 0;
}

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
  int value_returned = -1; /* accessed data */
  printf("%d", *ad);
  printf("a");

  value_returned = check_cache_data_hit(ad, data_type);
  num_access_cycles += CACHE_ACCESS_CYCLE;
  if (value_returned != -1) { // check data in cache
    // Hit
    printf("Hit!!!\n");
    num_cache_hits++;
    return value_returned;
  }

  value_returned = access_memory(ad, data_type);
  num_access_cycles += MEMORY_ACCESS_CYCLE;
  if (value_returned != -1) { // check data in memory
    // Miss
    num_cache_misses++;
    printf("Miss!!!\n");
    return value_returned;

  } else {

    printf("data neither in cache nor memory");
  }

  return value_returned;
}

int main(void) {
  FILE *ifp = NULL, *ofp = NULL;
  unsigned long int access_addr; /* byte address (located at 1st column) in
                                    "access_input.txt" */
  char access_type; /* 'b'(byte), 'h'(halfword), or 'w'(word) (located at 2nd
                       column) in "access_input.txt" */
  // int accessed_data; /* This is the data that you want to retrieve first from
  // cache, and then from memory * /
  init_memory_content(); // 1) Invoke init_memory_content
  init_cache_content();  // 2) Invoke init_cache_content

  ifp = fopen("access_input.txt", "r"); // 3) open access_input.txt file
  if (ifp == NULL) {
    printf("Can't open input file\n");
    return -1;
  }
  ofp = fopen("access_output.txt", "w");
  if (ofp == NULL) {
    printf("Can't open output file\n");
    fclose(ifp);
    return -1;
  }

  fprintf(ofp, "[Accessed Data]\n");

  char line[256];

  print_cache_entries();
  /* Fill out here by invoking retrieve_data() */
  while (fgets(line, sizeof(line), ifp) !=
         NULL) { // need to read all the input data
    sscanf(line, "%lx %c", &access_addr, &access_type);
    // call retrieve data to 4) Read each line
    // int accessed_data = retrieve_data(line, line[4]);
    int accessed_data = retrieve_data(line, access_type);
    printf("Calling retrieve_data with addr: %lx, data_type: %c\n", access_addr,
           access_type);

    fprintf(
        ofp, "%lx %c 0x%x \n", access_addr, access_type,
        accessed_data); // print addr and data_type accessed in the output file,
    printf(" data: %X ", accessed_data);

    global_timestamp++;
  }
  // hit ratio
  double hit_ratio =
      (double)num_cache_hits / (num_cache_misses + num_cache_hits);
  fprintf(ofp, "Hit ratio = %.2f    ( %d / %d ) \n", hit_ratio, num_cache_hits,
          num_cache_misses + num_cache_hits);

  // bandwidth
  double bandwidth = (double)num_bytes / num_access_cycles;
  fprintf(ofp, "Bandwidth = %.2f    ( %d / %d ) \n", bandwidth, num_bytes,
          num_access_cycles);

  // closing documents
  fclose(ifp);
  fclose(ofp);

  print_cache_entries(); // print the final cache entries
  return 0;
}

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
  // int temp = atoi(ad);
  printf("%d", *ad);
  printf("a");

  value_returned = check_cache_data_hit(ad, data_type);
  if (value_returned != -1) { // check data in cache
    // Hit
    printf("Hit!!!");
    return value_returned;
  }

  value_returned = access_memory(ad, data_type);
  if (value_returned != -1) { // check data in memory
    // Miss
    printf("Miss!!!");
    // in case of miss event retrieve data from the main memory by invoking
    // access memory
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
  char access_type;  /* 'b'(byte), 'h'(halfword), or 'w'(word) (located at 2nd
                        column) in "access_input.txt" */
  int accessed_data; /* This is the data that you want to retrieve first from
                        cache, and then from memory */
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

  /* Fill out here by invoking retrieve_data() */
  while (fgets(line, sizeof(line), ifp) !=
         NULL) { // need to read all the input data
    sscanf(line, "%lx %c", &access_addr, &access_type);
    // call retrieve data to 4) Read each line
    // accessed_data = retrieve_data((void *)access_addr, access_type);
    printf("Calling retrieve_data with addr: %lx, data_type: %c\n", access_addr,
           access_type);

    fprintf(ofp, "%lx %c \n", access_addr, access_type);
    // print addr and data_type accessed in the output file,
    // when accessed_data ready add 0x%x

    // print hit ratio and bandwidth for each cache mechanism as regards to
    // cache association size
    /** if (check_cache_data_hit((void *)access_addr, access_type) != -1) {
      num_cache_hits++;
    }; **/
    global_timestamp++;
  }

  /**double hit_ratio = (double)num_cache_hits / global_timestamp;
  fprintf(ofp, "Hit Ratio: %.2f%%\n",
          hit_ratio * 100); // print hit_ratio in ofp
**/
  fclose(ifp);
  fclose(ofp);

  print_cache_entries(); // print the final cache entries
  return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include "readromcode.h"

#define write_test
//#define read_test

#define NUM_TRANS (1) // Define the number of transactions

int main () {
  //Establish an initial return value. 0 = success
  int ret=0;

  // Define input and output arrays
  int ps_to_romcode[CODE_SIZE / sizeof(int)];
  int internal_bram[CODE_SIZE / sizeof(int)];
  int romcode_to_ps[CODE_SIZE / sizeof(int)];

  // Call any preliminary functions required to prepare input for the test.

#ifdef write_test
  // Initialize input data in ps_to_romcode
  for (int i = 0; i < CODE_SIZE / sizeof(int); i++) {
      ps_to_romcode[i] = i; // Example: set input data incrementally
  }


  // Call the top-level function multiple times, passing input stimuli as needed.
  for (int i = 0; i < NUM_TRANS; i++) {
	  readromcode(ps_to_romcode, internal_bram, romcode_to_ps, 0); // Assuming cmd is 0 for testing write ddr data to bram
  }
#endif

#ifdef read_test
  // Initialize input data in internal_bram
  for (int i = 0; i < CODE_SIZE / sizeof(int); i++) {
	  internal_bram[i] = i; // Example: set input data incrementally
  }


  // Call the top-level function multiple times, passing input stimuli as needed.
  for (int i = 0; i < NUM_TRANS; i++) {
	  readromcode(ps_to_romcode, internal_bram, romcode_to_ps, 1); // Assuming cmd is 1 for testing write bram data to ddr4
  }
#endif

  // Capture the output results of the function, write to a file
  FILE *output_file = fopen("output.dat", "w");
  if (output_file == NULL) {
      perror("Error opening output file");
      return 1;
  }

#ifdef write_test
  // Write the contents of internal_bram to the output file
  for (int i = 0; i < CODE_SIZE / sizeof(int); i++) {
      fprintf(output_file, "%d\n", internal_bram[i]);
  }
#endif

#ifdef read_test
  // Write the contents of internal_bram to the output file
  for (int i = 0; i < CODE_SIZE / sizeof(int); i++) {
      fprintf(output_file, "%d\n", romcode_to_ps[i]);
  }
#endif
  fclose(output_file);

  // Compare the results of the function against expected results
  ret = system("diff --brief -w output.dat output.golden.dat");

  if (ret != 0) {
      printf("Test failed !!!\n");
      ret = 1;
  } else {
      printf("Test passed!\n");
  }

  return ret;
}

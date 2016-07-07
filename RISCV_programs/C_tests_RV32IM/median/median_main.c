//**************************************************************************
// Median filter bencmark
//--------------------------------------------------------------------------
//
// This benchmark performs a 1D three element median filter. The
// input data (and reference data) should be generated using the
// median_gendata.pl perl script and dumped to a file named
// dataset1.h You should not change anything except the
// HOST_DEBUG and PREALLOCATE macros for your timing run.

//--------------------------------------------------------------------------

// Original: from MIT, for SMIPS tests
// This version: modified at Bluespec, for RISC-V tests

// ----------------------------------------------------------------

#include <stdio.h>

#include "median.h"
#include "median.i"

// Input/Reference Data

#include "dataset1.h"

// Helper functions

int verify( int n, int test[], int correct[] )
{
  int i;
  for ( i = 0; i < n; i++ ) {
    if ( test[i] != correct[i] ) {
      return 0;
    }
  }
  return 1;
}

//--------------------------------------------------------------------------
// Main

int main( int argc, char* argv[] )
{
  int results_data[DATA_SIZE];

  // Do the filter
  int cycles, insts;
  median( DATA_SIZE, input_data, results_data );

  // Check the results
  if (verify( DATA_SIZE, results_data, verify_data ))
      printf ("Verify = ok\n");
  else
      printf ("Verify = not ok\n");
  return 0;
}

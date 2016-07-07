// *************************************************************************
// multiply filter bencmark
// -------------------------------------------------------------------------
//
// This benchmark tests the software multiply implemenation. The
// input data (and reference data) should be generated using the
// multiply_gendata.pl perl script and dumped to a file named
// dataset1.h You should not change anything except the
// HOST_DEBUG and PREALLOCATE macros for your timing run.

//--------------------------------------------------------------------------

// Original: from MIT, for SMIPS tests
// This version: modified at Bluespec, for RISC-V tests

// ----------------------------------------------------------------

#include <stdio.h>
#include <stdint.h>

#include "multiply.h"
#include "multiply.i"

//--------------------------------------------------------------------------
// Input/Reference Data

#include "dataset1.h"

//--------------------------------------------------------------------------
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
  int i;
  int results_data[DATA_SIZE];

  // Do the multiply
  for (i = 0; i < DATA_SIZE; i++) {
    results_data[i] = multiply( input_data1[i], input_data2[i] );
  }

  // Check the results
  if (verify( DATA_SIZE, results_data, verify_data ))
      printf ("Verify = ok\n");
  else
      printf ("Verify = not ok\n");

  return 0;
}

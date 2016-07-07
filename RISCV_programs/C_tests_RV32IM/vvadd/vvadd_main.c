//**************************************************************************
// Vector-vector add benchmark
//--------------------------------------------------------------------------
//
// This benchmark uses adds to vectors and writes the results to a
// third vector. The input data (and reference data) should be
// generated using the vvadd_gendata.pl perl script and dumped
// to a file named dataset1.h The smips-gcc toolchain does not
// support system calls so printf's can only be used on a host system,
// not on the smips processor simulator itself. You should not change
// anything except the HOST_DEBUG and PREALLOCATE macros for your timing
// runs.

//--------------------------------------------------------------------------

// Original: from MIT, for SMIPS tests
// This version: modified at Bluespec, for RISC-V tests

// ----------------------------------------------------------------

#include <stdio.h>

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
// vvadd function

void vvadd( int n, int a[], int b[], int c[] )
{
  int i;
  for ( i = 0; i < n; i++ )
    c[i] = a[i] + b[i];
}

//--------------------------------------------------------------------------
// Main

int main( int argc, char* argv[] )
{
  int results_data[DATA_SIZE];

  // Do the vvadd

  vvadd(DATA_SIZE, input1_data, input2_data, results_data );

  // Check the results

  if (verify( DATA_SIZE, results_data, verify_data ))
      printf ("Verify = ok\n");
  else
      printf ("Verify = not ok\n");

  return 0;
}

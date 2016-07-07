//**************************************************************************
// Quicksort benchmark
//--------------------------------------------------------------------------
//
// This benchmark uses quicksort to sort an array of integers. The 
// implementation is largely adapted from Numerical Recipes for C. The
// input data (and reference data) should be generated using the 
// qsort_gendata.pl perl script and dumped to a file named 
// dataset1.h The smips-gcc toolchain does not support system calls
// so printf's can only be used on a host system, not on the smips
// processor simulator itself. You should not change anything except
// the HOST_DEBUG and PREALLOCATE macros for your timing run.

//--------------------------------------------------------------------------

// Original: from MIT, for SMIPS tests
// This version: modified at Bluespec, for RISC-V tests

#include <stdio.h>
#include <stdint.h>
#include "riscv_counters.h"

// ----------------------------------------------------------------

// The INSERTION_THRESHOLD is the size of the subarray when the 
// algorithm switches to using an insertion sort instead of
// quick sort. 

#define INSERTION_THRESHOLD 7 

// NSTACK is the required auxiliary storage. 
// It must be at least 2*lg(DATA_SIZE) 

#define NSTACK 50 

// Swap macro for swapping two values.

#define SWAP(a,b) temp=(a);(a)=(b);(b)=temp; 

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
// Quicksort function

void sort( int n, int arr[] ) 
{ 
  int i,j,k;
  int ir = n;
  int l = 1;
  int jstack = 0; 
  int a, temp; 

  int istack[NSTACK]; 

  for (;;) { 

    // Insertion sort when subarray small enough. 
    if ( ir-l < INSERTION_THRESHOLD ) { 

      for ( j = l+1; j <= ir; j++ ) { 
        a = arr[j-1]; 
        for ( i = j-1; i >= l; i-- ) { 
          if ( arr[i-1] <= a ) break; 
          arr[i] = arr[i-1]; 
        } 
        arr[i] = a; 
      } 

      if ( jstack == 0 ) break; 

      // Pop stack and begin a new round of partitioning. 
      ir = istack[jstack--]; 
      l = istack[jstack--]; 

    } 
    else { 

      // Choose median of left, center, and right elements as 
      // partitioning element a. Also rearrange so that a[l-1] <= a[l] <= a[ir-]. 

      k = (l+ir) >> 1; 
      SWAP(arr[k-1],arr[l]) 
      if ( arr[l-1] > arr[ir-1] ) { 
        SWAP(arr[l-1],arr[ir-1]) 
      } 
      if ( arr[l] > arr[ir-1] ) { 
        SWAP(arr[l],arr[ir-1]) 
      } 
      if ( arr[l-1] > arr[l] ) { 
        SWAP(arr[l-1],arr[l]) 
      } 

      // Initialize pointers for partitioning. 
      i = l+1; 
      j = ir; 

      // Partitioning element. 
      a = arr[l]; 

      for (;;) {                       // Beginning of innermost loop. 
        do i++; while (arr[i-1] < a);  // Scan up to find element > a. 
        do j--; while (arr[j-1] > a);  // Scan down to find element < a.         
        if (j < i) break;              // Pointers crossed. Partitioning complete. 
        SWAP(arr[i-1],arr[j-1]);       // Exchange elements. 
      }                                // End of innermost loop. 

      // Insert partitioning element. 
      arr[l] = arr[j-1]; 
      arr[j-1] = a; 
      jstack += 2; 

      // Push pointers to larger subarray on stack, 
      // process smaller subarray immediately. 

      if ( ir-i+1 >= j-l ) { 
        istack[jstack]   = ir; 
        istack[jstack-1] = i; 
        ir = j-1; 
      } 
      else { 
        istack[jstack]   = j-1; 
        istack[jstack-1] = l; 
        l = i; 
      } 
    } 

  } 

}

//--------------------------------------------------------------------------
// Main

int main( int argc, char* argv[] )
{
    int result_data[DATA_SIZE];
    int i;
    uint64_t c0, i0, c1, i1;

    // Do the sort
    for (i = 0; i < DATA_SIZE; i++) {
	result_data[i] = input_data[i];
    }
    c0 = read_cycle ();
    i0 = read_instret ();
    sort(DATA_SIZE, result_data);
    c1 = read_cycle() - c0;
    i1 = read_instret () - i0;

    // Check the results
    if (verify( DATA_SIZE, result_data, verify_data ))
	printf ("Verify = ok\n");
    else
	printf ("Verify = not ok\n");

    // printf ("Cycles: %d\n", c1);
    // printf ("Insts:  %d\n", i1);

    return 0;
}

#*****************************************************************************        
# Median filter (assembly version)
#-----------------------------------------------------------------------------
#
# Author : Christopher Batten
# Date   : February 8, 2006
#
# Simple 1D three element median filter benchmark ...
#

#--------------------------------------------------------------------------
# Headers and Defines
#--------------------------------------------------------------------------

#define rN             $4
#define rBasePtrInput  $5
#define rBasePtrOutput $6

#define rPtrInput      $7  
#define rPtrOutput     $8
#define rCount         $9
#define rA             $10
#define rB             $11
#define rC             $12
#define rTemp          $13

#--------------------------------------------------------------------------
# median_asm( int n, int input[], int results[] )
#--------------------------------------------------------------------------

        .text
        .align 2
        .globl median_asm
        .type  median_asm,@function
        .ent   median_asm

median_asm:

        # Number of output elements is n-2 (since we are working
        # with a 3 element median filter, there is no output value
        # for the first and last elemetns)

        move  rCount, rN
        addiu rCount, rCount, -2

        # Zero the ends

        sw    $0, 0(rBasePtrOutput)

        sll   rTemp, rN, 2
        addu  rTemp, rBasePtrOutput, rTemp 
        sw    $0, -4(rTemp)

        # Initialize the input and output pointers

        move  rPtrInput,  rBasePtrInput
        move  rPtrOutput, rBasePtrOutput

        addiu rPtrInput,  rPtrInput,  4
        addiu rPtrOutput, rPtrOutput, 4

 

medianloop:  

        # Load three elements from the input vector

        lw  rA, -4(rPtrInput)
        lw  rB,  0(rPtrInput)
        lw  rC,  4(rPtrInput)

        sltu rTemp, rA, rB      #. if ( A < B ) 
        beq  $0, rTemp, l1      #. {
                                #.
        sltu rTemp, rB, rC      #.   if ( B < C )
        beq  $0, rTemp, l2      #.   {
        sw   rB, 0(rPtrOutput)  #.     *rPtrOutput = B
        j    done               #.   }
                                #.
l2:     sltu rTemp, rC, rA      #.   else if ( C < A )
        beq  $0, rTemp, l3      #.   {
        sw   rA, 0(rPtrOutput)  #.     *rPtrOutput = A
        j    done               #.   }
                                #.
l3:     sw   rC, 0(rPtrOutput)  #.   else 
        j    done               #.     *rPtrOutput = C
                                #.
                                #. }
                                #. else {
                                #.
l1:     sltu rTemp, rA, rC      #.   if ( A < C )
        beq  $0, rTemp, l4      #.   {
        sw   rA, 0(rPtrOutput)  #.     *rPtrOutput = A
        j    done               #.   }
                                #.
l4:     sltu rTemp, rC, rB      #.   else if ( C < B )
        beq  $0, rTemp, l5      #.   {
        sw   rB, 0(rPtrOutput)  #.     *rPtrOutput = B
        j    done               #.   }
                                #.
l5:     sw   rC, 0(rPtrOutput)  #.   else
                                #.     *rPtrOutput = C
                                #.
                                #.}

done:
 
        # Increment the pointers and repeat

        addiu rPtrInput, rPtrInput,   4
        addiu rPtrOutput, rPtrOutput, 4
        addiu rCount, rCount, -1
        bne $0, rCount, medianloop


        # Return

        jr $31




#*****************************************************************************        
# multiply function (assembly version)
#-----------------------------------------------------------------------------
#
# Please see the tutorial on SMIPS assembly for more information on the
# SMIPS calling convention. Briefly, the C compiler will put the first
# four arguments to the function in registers 4-7. The return value should
# be placed in register 2. You should only use registers 8-15,24,25 since
# the C compiler will be using the other registers. Do not modify 
# registers 4-7 directly since this will essentially implement 
# call-by-reference semantics. Instead, copy the arguments into a different 
# set of registers.
#

#--------------------------------------------------------------------------
# Headers and Defines
#--------------------------------------------------------------------------

# The X argument will be in register 4 and the Y argument will be
# in register 5 when called. Place the result in register 2.

#define rX      $4
#define rY      $5
#define rResult $2

#define rCount  $7  
#define rTemp   $8
#define rXCopy  $9
#define rYCopy  $10

#--------------------------------------------------------------------------
# int multiply_asm( int x, int y )
#--------------------------------------------------------------------------

        .text
        .align 2
        .globl multiply_asm
        .type  multiply_asm,@function
        .ent   multiply_asm

multiply_asm:

        # Zero result, count

        addiu rXCopy, rX, 0
        addiu rYCopy, rY, 0
        li    rResult, 0 
        li    rCount, 32

        # We do the multiply loop 32 times, one for each bit in X
        
multiplyloop:  

        andi  rTemp, rXCopy, 1            # 
        beq   $0, rTemp, l1               # if (( x & 0x1 ) == 1) 
        addu  rResult, rResult, rYCopy    #  result = result + y
l1:                                       # 
        srl   rXCopy, rXCopy, 1           # x = x >> 1 
        sll   rYCopy, rYCopy, 1           # y = y << 1
 
        # Loop check

        addiu rCount, rCount, -1          
        bne   $0, rCount, multiplyloop

        # The C code uses a jalr instruction to call this function
        # so we can use a jr to return back to where the function 
        # was called.

        jr $31




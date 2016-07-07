	# CPI Test 1

	# Purpose: measure 'best case' CPI (cycles/instruction).

	# Loops a number of times over straight-line sequence of NOPs.
	# (no loads/stores, no register dependencies).

	# A properly pipelined implementation with a branch predictor
	# should get a CPI of 1 (modulo small costs at startup/shutdown
	# and loop-start/loop-exit).  With sufficient loop iterations
	# those costs should be negligible.
	
#define PC_RESET_VALUE  0x200
#define ITERS           512
#define	N               14

	.text
	.globl	_start
	.org	PC_RESET_VALUE

_start:
	addi	s0, zero, ITERS
1:	addi	s0, s0, -1
	.rept	N
	nop
	.endr
	bnez	s0, 1b
halt:
	ebreak

	
	.end

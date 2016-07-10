# Copyright (c) 2013-2016 Bluespec, Inc. All Rights Reserved

# ================================================================
# Please edit these definitions for your
# preferences and installation-specific directories

ASM_TESTS = ../RISCV_programs/asm_tests_RV32IM
C_TESTS   = ../RISCV_programs/C_tests_RV32IM

SIM_EXEFILE = VmkTop_Sim_Standalone

# ================================================================
# Run all tests

.PHONY: do_tests
do_tests:
	make  do_test_CPI_test1 
	make  do_test_hello
	make  do_test_median
	make  do_test_multiply
	make  do_test_print
	make  do_test_qsort
	make  do_test_towers
	make  do_test_vvadd
	make  do_test_intOpsTest
	make  do_test_filters

# ================================================================
# Run individual tests

.PHONY: do_test_CPI_test1
do_test_CPI_test1:
	env SIM_ELF_FILENAME=$(ASM_TESTS)/CPI_test1/CPI_test1   ./$(SIM_EXEFILE)

.PHONY: do_test_hello
do_test_hello:
	env SIM_ELF_FILENAME=$(C_TESTS)/hello/hello             ./$(SIM_EXEFILE)

.PHONY: do_test_median
do_test_median:
	env SIM_ELF_FILENAME=$(C_TESTS)/median/median_main      ./$(SIM_EXEFILE)

.PHONY: do_test_multiply
do_test_multiply:
	env SIM_ELF_FILENAME=$(C_TESTS)/multiply/multiply_main  ./$(SIM_EXEFILE)

.PHONY: do_test_print
do_test_print:
	env SIM_ELF_FILENAME=$(C_TESTS)/print/print             ./$(SIM_EXEFILE)

.PHONY: do_test_qsort
do_test_qsort:
	env SIM_ELF_FILENAME=$(C_TESTS)/qsort/qsort_main        ./$(SIM_EXEFILE)

.PHONY: do_test_towers
do_test_towers:
	env SIM_ELF_FILENAME=$(C_TESTS)/towers/towers_main      ./$(SIM_EXEFILE)

.PHONY: do_test_vvadd
do_test_vvadd:
	env SIM_ELF_FILENAME=$(C_TESTS)/vvadd/vvadd_main        ./$(SIM_EXEFILE)

.PHONY: do_test_intOpsTest
do_test_intOpsTest:
	env SIM_ELF_FILENAME=$(C_TESTS)/intOpsTest/intOpsTest   ./$(SIM_EXEFILE)

.PHONY: do_test_filters
do_test_filters:
	env SIM_ELF_FILENAME=$(C_TESTS)/filters/filters         ./$(SIM_EXEFILE)

// Copyright (c) 2013-2016 Bluespec, Inc. All Rights Reserved

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

// ================================================================
// These are C functions to model memory.
// Each function has a basic form for calls from C code,
// and a variant for calls from BSV code ("bsv_...").
// The BSV-call versions are imported into BSV using 'import "BDPI"'
// and have arg and result types are chosen for simple BSV linkage.

// ================================================================
// Each instance of a memory has the following structure

typedef struct Mem_Model_s {
    uint8_t  *memp;         // The actual memory (malloc'd)
    uint64_t  base_addr;    // Logical base byte address
    uint64_t  limit;        // Logical byte address just beyond the memory.

    // The following is info recorded while loading an ELF file into the memory
    uint64_t  min_addr;
    uint64_t  max_addr;
    uint64_t  pc_start;     // Addr of 'start' label
    uint64_t  pc_exit;      // Addr of 'exit' label
} Mem_Model;

// ================================================================
// Basic memory model allocation, read, write

// ----------------
// Allocation

extern Mem_Model *c_mem_init   (uint64_t mem_base_addr, uint64_t mem_size);
extern uint64_t   bsv_mem_init (uint64_t mem_base_addr, uint64_t mem_size);

// ----------------
// Generic read, write, with n_bytes = {1,2,4,8}, and p_data pointing to data buffer
// Function result is 0 for failure, 1 for sucess

extern
bool c_mem_read   (Mem_Model *p_mem_model, uint64_t verbosity, uint64_t addr, uint64_t n_bytes, uint64_t *p_data);
extern
int  bsv_mem_read (uint64_t   p_mem_model, uint64_t verbosity, uint64_t addr, uint64_t n_bytes, uint64_t *p_data);

extern
bool c_mem_write   (Mem_Model *p_mem_model, uint64_t verbosity, uint64_t addr, uint64_t x, uint64_t n_bytes);
extern
int  bsv_mem_write (uint64_t   p_mem_model, uint64_t verbosity, uint64_t addr, uint64_t x, uint64_t n_bytes);

// ----------------
// Wrappers for c_mem_read, specialized for 1-, 2- and 4-byte reads

extern
uint8_t c_mem_read8   (Mem_Model *p_mem_model, uint64_t verbosity, uint64_t addr);
extern
uint8_t bsv_mem_read8 (uint64_t   p_mem_model, uint64_t verbosity, uint64_t addr);

extern
uint16_t c_mem_read16   (Mem_Model *p_mem_model, uint64_t verbosity, uint64_t addr);
extern
uint16_t bsv_mem_read16 (uint64_t   p_mem_model, uint64_t verbosity, uint64_t addr);

extern
uint32_t c_mem_read32   (Mem_Model *p_mem_model, uint64_t verbosity, uint64_t addr);
extern
uint32_t bsv_mem_read32 (uint64_t   p_mem_model, uint64_t verbosity, uint64_t addr);

// ================================================================
// Utilities to load ELF files into the memory model
// and return properties of the loaded ELF image

// ----------------
// Load an ELF file into memory.
// Checks that the ELF file has specified bitwidth (32 or 64).

extern void c_mem_load_elf   (Mem_Model *p_mem_model, char *elf_filename, uint64_t bitwidth,
			      const char *start_symbol, const char *exit_symbol);
extern void bsv_mem_load_elf (uint64_t   p_mem_model, char *elf_filename, uint64_t bitwidth,
			      char *start_symbol, char *exit_symbol);

// ----------------
// Variant of c_mem_load_elf(), where the ELF filename is taken from
// the environment variable SIM_ELF_FILENAME.
// Returns 1 (success) if SIM_ELF_FILENAME is defined, and file is loaded
//         0 (failure) if SIM_ELF_FILENAME is not defined

extern int c_mem_load_elf2   (Mem_Model *p_mem_model, uint64_t bitwidth);
extern int bsv_mem_load_elf2 (uint64_t   p_mem_model, uint64_t bitwidth);

// ----------------
// Return various properties of the loaded ELF file

// Address of '_start' label in ELF binary
extern uint64_t c_mem_get_start_pc   (Mem_Model *p_mem_model);
extern uint64_t bsv_mem_get_start_pc (uint64_t   p_mem_model);

// Address of 'exit' label in ELF binary
extern uint64_t c_mem_get_exit_pc   (Mem_Model *p_mem_model);
extern uint64_t bsv_mem_get_exit_pc (uint64_t   p_mem_model);

// Lowest address in ELF binary
extern uint64_t c_mem_get_min_addr   (Mem_Model *p_mem_model);
extern uint64_t bsv_mem_get_min_addr (uint64_t   p_mem_model);

// Highest address in ELF binary
extern uint64_t c_mem_get_max_addr   (Mem_Model *p_mem_model);
extern uint64_t bsv_mem_get_max_addr (uint64_t   p_mem_model);

// ================================================================

#ifdef __cplusplus
}
#endif

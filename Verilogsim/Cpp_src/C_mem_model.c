// Copyright (c) 2013-2016 Bluespec, Inc. All Rights Reserved

// ================================================================
// Standard C includes

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <inttypes.h>
#include <limits.h>
#include <string.h>
#include <ctype.h>
#include <fcntl.h>
#include <gelf.h>
#include <assert.h>

// #define __USE_XOPEN
#include <poll.h>

// ================================================================
// Project includes

#include "C_mem_model.h"

// ================================================================
// Union to convert between between uint64_t and other types

static
union {
    uint64_t   u64;
    int64_t    i64;
    uint32_t   u32;
    int32_t    i32;
    uint16_t   u16;
    int16_t    i16;
    uint8_t    u8;
    int8_t     i8;
    char       ch;
    void      *xp;
} U;

// ================================================================
// Check/report if block [addr,addr+n_bytes] goes outside [base,limit]

static
bool bad_addr (Mem_Model* p_mem_model, uint64_t verbosity, uint64_t addr, uint64_t n_bytes)
{
    bool b1 = (addr < p_mem_model->base_addr);                // below base
    bool b2 = (p_mem_model->limit < (addr + n_bytes));        // above limit
    if ((b1 || b2) && (verbosity > 0)) {
	if (b1)
	    fprintf (stderr, "ERROR: c_mem_model: addr 0x%0" PRIx64 " < base address\n", addr);
	if (b2)
	    fprintf (stderr,
		     "ERROR: c_mem_model: addr 0x%0" PRIx64 "+n_bytes 0x%0" PRIx64 "= 0x%0" PRIx64 " > limit\n",
		     addr, n_bytes, addr + n_bytes);
	fprintf (stderr,
		 "       Base: 0x%0" PRIx64 ", Limit: 0x%0" PRIx64 "\n",
		 p_mem_model->base_addr, p_mem_model->limit);
    }
    return (b1 || b2);
}

// ================================================================
// Memory creation and initialization 

Mem_Model *c_mem_init (uint64_t mem_base_addr, uint64_t mem_size)
{
    FILE      *fp;
    int        n;
    Mem_Model *p_mem_model;

    fprintf (stdout, "INFO: c_mem_init (base 0x%0" PRIx64", size 0x%0" PRIx64 ")\n",
	     mem_base_addr, mem_size);

    p_mem_model = (Mem_Model *) malloc (sizeof (Mem_Model));
    if (p_mem_model == NULL) {
	fprintf (stderr, "ERROR: c_mem_init: could not malloc Mem_Model object\n");
	exit (1);
    }
    bzero (p_mem_model, sizeof (Mem_Model));

    // Round up mem_size to multiple of 8 bytes (64b word)
    mem_size = (mem_size + 7) & (~ 0x7llu);

    p_mem_model->memp = (uint8_t *) malloc (mem_size);
    if (p_mem_model->memp == NULL) {
	fprintf (stderr, "ERROR: c_mem_init: could not malloc %0" PRId64 " byte memory\n",
		 mem_size);
	exit (1);
    }
    p_mem_model->base_addr = mem_base_addr;
    p_mem_model->limit     = mem_base_addr + mem_size;

    // Initialize each memory byte
#ifdef INITIAL_MEMZERO_ON
    uint32_t  init_val = 0;
#else
    // to 0xAA (matching default "unspecified" value in BSV)
    uint32_t  init_val = 0xaaaaAAAA;
#endif
    uint32_t  *p32 = (uint32_t *) p_mem_model->memp;
    uint32_t   j;
    for (j = 0; j < (mem_size >> 2); j++)
	p32 [j] = init_val;

    return p_mem_model;
}

uint64_t bsv_mem_init (uint64_t mem_base_addr, uint64_t mem_size)
{
    Mem_Model *p_mem_model = c_mem_init (mem_base_addr, mem_size);
    return (uint64_t) (p_mem_model);
}

// ================================================================
// Read mem[addr,addr+n_bytes]
// where n_bytes = 1, 2, 4 or 8 only
// Function result is 0 for failure, 1 for sucess
// Value from memory is returned in p_data.

bool c_mem_read (Mem_Model *p_mem_model, uint64_t verbosity, uint64_t addr, uint64_t n_bytes, uint64_t *p_data)
{
    uint8_t  *p1;
    uint16_t *p2;
    uint32_t *p4;
    uint64_t *p8;

    if (bad_addr (p_mem_model, verbosity, addr, n_bytes)) {
	if (verbosity > 0)
	    fprintf (stderr, "       c_mem_read: returning 0\n");
	return 0;
    }

    U.xp  = p_mem_model->memp;
    U.u64 = U.u64 + addr;
    switch (n_bytes) {
      case 1: p1 = (uint8_t  *) U.xp; *p_data = *p1; break;
      case 2: p2 = (uint16_t *) U.xp; *p_data = *p2; break;
      case 4: p4 = (uint32_t *) U.xp; *p_data = *p4; break;
      case 8: p8 = (uint64_t *) U.xp; *p_data = *p8; break;
      default: {
	  fprintf (stderr,
		   "ERROR: c_mem_read: n_bytes %0" PRId64 " should be 1, 2, 4 or 8\n",
		   n_bytes);
	  return 0;
      }
    }
    if (verbosity > 1) {
	printf ("c_mem_read (addr %0" PRIx64 ", n_bytes %0" PRId64 ") => data %0" PRIx64 "\n", addr, n_bytes, *p_data);
    }
    return 1;
}

int bsv_mem_read (uint64_t p_mem_model, uint64_t verbosity, uint64_t addr, uint64_t n_bytes, uint64_t *p_data)
{
    return (int) c_mem_read ((Mem_Model *) p_mem_model, verbosity, addr, n_bytes, p_data);
}

// ================================================================
// Write x to mem[addr,addr+n_bytes]
// where n_bytes = 1, 2, 4 or 8 only
// Function result is is 0 for failure, 1 for sucess

bool c_mem_write (Mem_Model *p_mem_model, uint64_t verbosity, uint64_t addr, uint64_t x, uint64_t n_bytes)
{
    uint8_t  *p1;
    uint16_t *p2;
    uint32_t *p4;
    uint64_t *p8;

    if (verbosity > 1) {
	fprintf (stdout, "c_mem_write (addr 0x%0" PRIx64 ", data 0x%0" PRIx64 ", n_bytes %" PRId64 ")\n", addr, x, n_bytes);
    }

    if (bad_addr (p_mem_model, verbosity, addr, n_bytes)) {
	if (verbosity > 0)
	    fprintf (stderr, "       c_mem_write: ignoring.\n");
	return 0;
    }

    U.xp  = p_mem_model->memp;
    U.u64 = U.u64 + addr;
    switch (n_bytes) {
      case 1: p1 = (uint8_t *)  U.xp; *p1 = x; break;
      case 2: p2 = (uint16_t *) U.xp; *p2 = x; break;
      case 4: p4 = (uint32_t *) U.xp; *p4 = x; break;
      case 8: p8 = (uint64_t *) U.xp; *p8 = x; break;
      default: {
	  fprintf (stderr,
		   "ERROR: c_mem_write: n_bytes %0" PRId64 " should be 1, 2, 4 or 8\n",
		   n_bytes);
	  return 0;
      }
    }
    return 1;
}

int bsv_mem_write (uint64_t p_mem_model, uint64_t verbosity, uint64_t addr, uint64_t x, uint64_t n_bytes)
{
    return (int) c_mem_write ((Mem_Model *) p_mem_model, verbosity, addr, x, n_bytes);
}

// ================================================================
// Specialized versions of c_mem_read, reading only an 8b, 16b or 32b value,
// and exiting on error.

uint8_t c_mem_read8 (Mem_Model *p_mem_model, uint64_t verbosity, uint64_t addr)
{
    uint64_t data;

    if (c_mem_read (p_mem_model, verbosity, addr, 1, & data))
	return data;
    else
	exit (1);
}

uint8_t bsv_mem_read8 (uint64_t p_mem_model, uint64_t verbosity, uint64_t addr)
{
    return (uint8_t) c_mem_read8 ((Mem_Model *) p_mem_model, verbosity, addr);
}

uint16_t c_mem_read16 (Mem_Model *p_mem_model, uint64_t verbosity, uint64_t addr)
{
    uint64_t data;

    if (c_mem_read (p_mem_model, verbosity, addr, 2, & data))
	return data;
    else
	exit (1);
}

uint16_t bsv_mem_read16 (uint64_t p_mem_model, uint64_t verbosity, uint64_t addr)
{
    return (uint16_t) c_mem_read16 ((Mem_Model *) p_mem_model, verbosity, addr);
}

uint32_t c_mem_read32 (Mem_Model *p_mem_model, uint64_t verbosity, uint64_t addr)
{
    uint64_t data;

    if (c_mem_read (p_mem_model, verbosity, addr, 4, & data))
	return data;
    else
	exit (1);
}

uint32_t bsv_mem_read32 (uint64_t p_mem_model, uint64_t verbosity, uint64_t addr)
{
    return (uint32_t) c_mem_read32 ((Mem_Model *) p_mem_model, verbosity, addr);
}

// ****************************************************************
// ****************************************************************
// ****************************************************************
// Utilities to load ELF files into the memory model
// and discover properties of the ELF image

// ================================================================
// Load an ELF file into memory.
// Checks that the ELF file has specified bitwidth (32 or 64).

void c_mem_load_elf (Mem_Model *p_mem_model, char *elf_filename, uint64_t bitwidth,
		     const char *start_symbol, const char *exit_symbol)
{
    int fd, n_initialized = 0;
    Elf *e;

    if ((p_mem_model == NULL) || (p_mem_model->memp == NULL)) {
	fprintf (stderr, "ERROR: c_mem_load_elf: memory model not initialized\n");
	exit (1);
    }

    if (start_symbol == NULL)
      start_symbol = "_start";
    if (exit_symbol == NULL)
      exit_symbol = "exit";
    
    // verify the elf library version
    if (elf_version (EV_CURRENT) == EV_NONE) {
        fprintf (stderr, "ERROR: c_mem_load_elf: Failed to initialize the libelfg library!\n");
	exit (1);
    }

    // open the file for reading
    fd = open (elf_filename, O_RDONLY, 0);
    if (fd < 0) {
        fprintf (stderr, "ERROR: c_mem_load_elf: could not open elf input file: %s\n", elf_filename);
	exit (1);
    }

    // initialize the Elf pointer with the open file
    e = elf_begin (fd, ELF_C_READ, NULL);
    if (e == NULL) {
        fprintf (stderr, "ERROR: c_mem_load_elf: elf_begin() initialization failed!\n");
	exit (1);
    }

    // Verify that the file read in is an ELF file
    if (elf_kind (e) != ELF_K_ELF) {
        elf_end (e);
        fprintf (stderr, "ERROR: c_mem_load_elf: specified file '%s' is not an ELF file!\n", elf_filename);
	exit (1);
    }

    // Verify that we are looking at a RISC-V executable
    GElf_Ehdr ehdr;
    if (gelf_getehdr (e, &ehdr) == NULL) {
        elf_end (e);
        fprintf (stderr, "ERROR: c_mem_load_elf: get_getehdr() failed: %s\n", elf_errmsg(-1));
	exit (1);
    }

    // Verify we are dealing with a correct 32/64-bit little endian RISC-V executable
    if (bitwidth == 32) {
	if (gelf_getclass (e) != ELFCLASS32) {
	    elf_end (e);
	    fprintf (stderr, "ERROR: c_mem_load_elf: %s is not a 32-bit ELF file\n",
		     elf_filename);
	    exit (1);
	}
    }
    else if (bitwidth == 64) {
	if (gelf_getclass (e) != ELFCLASS64) {
	    elf_end (e);
	    fprintf (stderr, "ERROR: c_mem_load_elf: %s is not a 64-bit ELF file\n",
		     elf_filename);
	    exit (1);
	}
    }
    else {
        fprintf (stderr, "ERROR: c_mem_load_elf: bitwidth is %0" PRId64 "; should be 32 or 64 only\n",
		 bitwidth);
	exit (1);
    }


    if (ehdr.e_machine != 243) { // EM_RISCV is not defined, but this returns 243 when used with a valid elf file.
        elf_end (e);
        fprintf (stderr, "ERROR: c_mem_load_elf: %s is not a RISC-V ELF file\n", elf_filename);
	exit (1);
    }

    if (ehdr.e_ident[EI_DATA] != ELFDATA2LSB) {
        elf_end (e);
        fprintf (stderr, "ERROR: c_mem_load_elf: %s is a big-endian 64-bit RISC-V executable which is not supported\n", elf_filename);
	exit (1);
    }

    // Grab the string section index
    size_t shstrndx;
    shstrndx = ehdr.e_shstrndx;

    // Iterate through each of the sections looking for code that should be loaded
    Elf_Scn  *scn   = 0;
    GElf_Shdr shdr;

    p_mem_model->min_addr = 0xFFFFFFFFFFFFFFFFllu;
    p_mem_model->max_addr = 0x0000000000000000llu;
    p_mem_model->pc_start = 0xFFFFFFFFFFFFFFFFllu;
    p_mem_model->pc_exit  = 0xFFFFFFFFFFFFFFFFllu;

    while ((scn = elf_nextscn (e,scn)) != NULL) {
        // get the header information for this entry
        gelf_getshdr (scn, &shdr);

	Elf_Data *data = 0;
	// If we find a code/data section, load it into the model
	if ((shdr.sh_type == SHT_PROGBITS || shdr.sh_type == SHT_INIT_ARRAY || shdr.sh_type == SHT_FINI_ARRAY) &&
            ((shdr.sh_flags & SHF_WRITE) || (shdr.sh_flags & SHF_ALLOC) || (shdr.sh_flags & SHF_EXECINSTR))) {
	    data = elf_getdata (scn, data);
            uint64_t sh_addr = shdr.sh_addr & 0x3fffffff;

	    if (bad_addr (p_mem_model, 1, sh_addr, data->d_size)) exit (1);

	    memcpy (p_mem_model->memp + sh_addr - p_mem_model->base_addr,
		    (uint8_t*) data->d_buf,
		    data->d_size);
	    n_initialized += data->d_size;
	    if (shdr.sh_addr < p_mem_model->min_addr)
		p_mem_model->min_addr = sh_addr;
	    if (p_mem_model->max_addr < (sh_addr + shdr.sh_size + 4))
		p_mem_model->max_addr = sh_addr + shdr.sh_size + 4;
	}

	// If we find the symbol table, search for the start address location
	if (shdr.sh_type == SHT_SYMTAB) {
 	    // Get the section data
	    data = elf_getdata (scn, data);

	    // Get the number of symbols in this section
	    int symbols = shdr.sh_size / shdr.sh_entsize;

	    // search for the uart_default symbols we need to potentially modify.
	    GElf_Sym sym;
	    int i;
	    for (i = 0; i < symbols; ++i) {
	        // get the symbol data
	        gelf_getsym (data, i, &sym);

		// get the name of the symbol
		char *name = elf_strptr (e, shdr.sh_link, sym.st_name);

		// Look for, and remember PC of the start symbol
		if (strcmp (name, start_symbol) == 0) {
		    p_mem_model->pc_start = sym.st_value;
		}
		// Look for, and remember PC of the exit symbol
		else if (strcmp (name, exit_symbol) == 0) {
		    p_mem_model->pc_exit = sym.st_value;
		}
	    }
	}
    }

    elf_end (e);

    if (p_mem_model->pc_start == -1) {
	fprintf (stderr, "ERROR: c_mem_load_elf: no '_start' label found\n");
	exit (1);
    }
}

// ----------------

void bsv_mem_load_elf (uint64_t p_mem_model, char *elf_filename, uint64_t bitwidth,
		       char *start_symbol, char *exit_symbol)
{
    c_mem_load_elf ((Mem_Model *) p_mem_model, elf_filename, bitwidth,
		    start_symbol, exit_symbol);
}

// ================================================================
// Variant of c_mem_load_elf(), where the ELF filename is taken from
// the environment variable SIM_ELF_FILENAME, the start symbol from
// SIM_ELF_START_SYM, and the exit symbol from SIM_ELF_EXIT_SYM.
// Returns 1 (success) if SIM_ELF_FILENAME is defined, and file is loaded
//         0 (failure) if SIM_ELF_FILENAME is not defined

int c_mem_load_elf2 (Mem_Model *p_mem_model, uint64_t bitwidth)
{
    char *elf_filename;
    char *start_symbol, *exit_symbol;

    elf_filename = getenv ("SIM_ELF_FILENAME");
    if (elf_filename == 0) {
	fprintf (stderr, "c_mem_load_elf2: no SIM_ELF_FILENAME in environment; not loading any file\n");
	return 0;
    }
    fprintf (stdout, "INFO: c_mem_load_elf2: loading ELF file:\n    %s\n", elf_filename);

    // If the start and exit symbols are not defined, defaults will be used
    start_symbol = getenv ("SIM_ELF_START_SYM");
    exit_symbol = getenv ("SIM_ELF_EXIT_SYM");

    c_mem_load_elf (p_mem_model, elf_filename, bitwidth, start_symbol, exit_symbol);
    return 1;
}

// ----------------

int bsv_mem_load_elf2 (uint64_t p_mem_model, uint64_t bitwidth)
{
    return c_mem_load_elf2 ((Mem_Model *) p_mem_model, (uint64_t) bitwidth);
}

// ================================================================
// Address of '_start' label in ELF binary

uint64_t c_mem_get_start_pc (Mem_Model *p_mem_model)
{
    return p_mem_model->pc_start;
}

uint64_t bsv_mem_get_start_pc (uint64_t p_mem_model)
{
    return (uint64_t) c_mem_get_start_pc ((Mem_Model *) p_mem_model);
}

// ================================================================
// Address of 'exit' label in ELF binary

uint64_t c_mem_get_exit_pc (Mem_Model *p_mem_model)
{
    return p_mem_model->pc_exit;
}

uint64_t bsv_mem_get_exit_pc (uint64_t p_mem_model)
{
    return (uint64_t) c_mem_get_exit_pc ((Mem_Model *) p_mem_model);
}

// ================================================================
// Lowest address in ELF binary

uint64_t c_mem_get_min_addr (Mem_Model *p_mem_model)
{
    return p_mem_model->min_addr;
}

uint64_t bsv_mem_get_min_addr (uint64_t p_mem_model)
{
    return (uint64_t) c_mem_get_min_addr ((Mem_Model *) p_mem_model);
}

// ================================================================
// Highest address in ELF binary

uint64_t c_mem_get_max_addr (Mem_Model *p_mem_model)
{
    return p_mem_model->max_addr;
}

uint64_t bsv_mem_get_max_addr (uint64_t p_mem_model)
{
    return (uint64_t) c_mem_get_max_addr ((Mem_Model *) p_mem_model);
}

// ================================================================

// Hand-written System Verilog import statements for imported C functions

import "DPI-C" function longint unsigned c_getenv_u64 (string name);

import "DPI-C" function void c_putchar (byte unsigned ch);

import "DPI-C" function byte unsigned c_trygetchar ();

import "DPI-C" function longint unsigned   bsv_mem_init (longint unsigned mem_base_addr, longint unsigned mem_size);

import "DPI-C" function int bsv_mem_load_elf2 (longint unsigned p_mem_model,
					       longint unsigned bitwidth);

import "DPI-C" function longint unsigned bsv_mem_get_exit_pc (longint unsigned  p_mem_model);

import "DPI-C" function longint unsigned bsv_mem_get_min_addr (longint unsigned  p_mem_model);

import "DPI-C" function longint unsigned bsv_mem_get_max_addr (longint unsigned  p_mem_model);

import "DPI-C" function int unsigned bsv_mem_read32 (longint unsigned p_mem_model,
						     longint unsigned verbosity,
						     longint unsigned addr);

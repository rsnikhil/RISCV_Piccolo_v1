# `RISCV_Piccolo_v1`
Implementation of RISC-V RV32IM. Simple in-order 3-stage pipeline. Low resources (e.g., FPGA softcore).

MIT License (see LICENSE.txt)

----------------------------------------------------------------

## Introduction

This is a preliminary release of Bluespec Inc.'s _Piccolo_, which is a
an implementation of the RISC-V Instruction Set Architecture (ISA).

* RV32IM variant of the RISC-V ISA.

* Simple 3-stage in-order pipeline, target for low-resource applications, such as FPGA softcores.

* User- and Machine-mode privilege levels.

* No memory management, no floating point.

This repository contains all the Verilog source files for the Piccolo
CPU, as well as surrounding modules that enable system-level execution
(Top_Sim_Standalone, SoC_Top, Sim_Driver, ICache, DCache, Fabric,
Mem_Controller, Mem_Model, UART).

In addition, this repository contains two pre-built simulation
executables that you can run immediately (on 64-bit Linux platforms)
built from the same source code.  One uses Bluespec's Bluesim
simulator, and the other uses CVC's Verilog Open Source Simulator.

Finally, this repository contains some pre-compiled RISC-V ELF
binaries for C and RISC-V assembly language programs, compiled using
the RISC-V gcc toolchain.

You can run the simulators on these ELF files, i.e., you will be
simulating a CPU-Caches-Fabric-Memory-UART "SoC" system, where the CPU
is the Piccolo RISC-V CPU, and it is executing one of the ELF
binaries.

If you have the RISC-V gcc toolchain (from `riscv.org`) you should be
able to compile other programs into ELF executables and run them on
these simulators.  The Makefiles in the Bluesim/ and Verilogsim
directories provide examples of how to invoke the simulator on an
executable.

NOTE: these simulators have been compiled to run on 64-bit x86 Linux
platforms only (such as Ubuntu and Debian).  We have no plans to
produce versions for other platforms (e.g., Windows, Mac OS).

----------------------------------------------------------------

## Repository contents:

* `Bluesim/`

  Contains a pre-built Linux executable using Bluespec, Inc.'s Bluesim
  simulator.  The Makefile allows running the simulator on any of the
  pre-compiled ELF files in `Programs/` directory.  The `README` in
  the directory has more details on how to run a RISC-V simulation on
  an ELF file and how to get more verbose traces.

  For example, `$ make do_test_hello` runs the "Hello World!" program.

* `Verilogsim/`

  Contains a pre-built Linux executable using CVC's Verilog simulator.
  The Makefile allows running the simulator on
  any of the pre-compiled ELF files in `Programs/` directory.  The
  `README` in the directory has more details on how to run a RISC-V
  simulation on an ELF file and how to get more verbose traces.

  For example, `$ make do_test_hello` runs the "Hello World!" program.

  * `verilog/`

     Verilog RTL from which Verilogsim was built.  This Verilog is not
     hand-written, but generated from Bluespec BSV sources (not
     included) by Bluespec's bsc compiler.  It's moderately readable
     (YMMV).

* `RISCV_Programs/`

    * `C_tests_RV32IM/`

    * `asm_tests_RV32IM/`

    These contain a number of sub-directories (e.g., "hello/",
    containing a "Hello World!" program).  Each directory contains a
    .c (C) or .S (assembly) source file, a pre-compiled RISC-V RV32IM
    ELF executable, and a .text dis-assembly of the ELF file.

----------------------------------------------------------------
## Running the Bluesim simulator on RISV-V ELF files

You should be in the Bluesim directory: `$ cd Bluesim`

To run an individual program, e.g., "Hello World!": `$ make do_test_hello`

To run all programs (file `sample_transcript` is a transcript of this): `$ make do_tests`

If you set the environment variable SIM_VERBOSITY (to 1, 2, ...) it
will produce increasingly detailed simulation traces indicating
activity on a clock-by-clock basis in the CPU pipeline, caches,
interconnect fabric and memory controller.

The `make` commands invoke the Bluesim executable: `$ top_Sim_Standalone_exe`

If you provide the flag `-V` it will dump VCDs waveforms to the file `dump.vcd`.

If you provide the flag `-V foo.vcd` it will dump VCDs waveforms to the file `foo.vcd`.

----------------------------------------------------------------
## Running the Verilog simulator on RISV-V ELF files

You should be in the Verilogsim directory: `$ cd Verilogsim`

To run an individual program, e.g., "Hello World!": `$ make do_test_hello`

To run all programs (file `sample_transcript` is a transcript of this): `$ make do_tests`

If you set the environment variable SIM_VERBOSITY (to 1, 2, ...) it
will produce increasingly detailed simulation traces indicating
activity on a clock-by-clock basis in the CPU pipeline, caches,
interconnect fabric and memory controller.

The `make` commands invoke the CVC Verilog simulation executable: `$ top_Sim_Standalone_exe_v`

If you provide the flag `+bscvcd` it will dump VCDs waveforms to the file `dump.vcd`.

----------------------------------------------------------------

## Synthesizing the Piccolo CPU by itself.

The simulations include the Piccolo CPU itself, caches, an
interconnect fabric, a memory controller, a memory model, a UART
model, a system controller to load ELF files, etc.

If you wish to synthesize the CPU by itself, and connect it to your own
caches/memory, the verilog module `mkCPU.v` is the top-level module.

----------------------------------------------------------------

## Notes

Although only simulators are provided here, this is not just a
simulation model, but a fully synthesizable implementation.  On Xilinx
Virtex-7 FPGAs, the CPU by itself takes from 1.5K to 2.5K LUTs, from
580 to 750 flipflops, and from 0 to 9 DSPs, at over a 100 MHz,
depending on which features are included/excluded (e.g. RV32 'M'
instructions for integer multiply/ divide/ remainder).

Both simulation models (Bluesim and Verilog sim) simulate the actual
synthesizable code, fully cycle-accurate (you can generate VCDs from
either simulation).  Bluesim simulation is about 20x faster than
Verilog simulation.

Piccolo demonstrates an ideal CPI (Cycles per Instruction) of exactly
1.0 for all codes that do not include integer divides or I/O (which
take multiple cycles), when the caches here are replaced by TCMs
(Tightly Coupled Memories, not included here), where reads always
return a result in 1 cycle.

For Verilog simulation, we only test with the CVC Open Source Verilog
simulator.  If you need help building for other Verilog simulators,
please contact Bluespec, Inc. support.

If you have CVC you should be able to rebuild what's in Verilogsim.
Or, you may be able to load these into some other Verilog simulator
(Caution: contains VPI for importing some C routines, and
unfortunately VPI details may vary somewhat across Verilog
simulators).

If you are interested in the Bluespec BSV source codes, or variants
that include Supervisor privilege level, Virtual Memory management and
Floating Point, please contact Bluespec, Inc. support.  The BSV
sources and variants are not, at the moment, open-sourced.

To compile your own ELF files to run on these simulators, you will
need the RISC-V tool chain (gcc and friends) from `riscv.org`.
Further, they will have to be configured to generate RV32IM ELF files,
to use newlib (since there is no OS running), and stdio should
ultimately become Loads/Stores to a UART device (Piccolo does not
support the non-standard RISC-V `htif` mechanism).  For help in
configuring the RISC-V tools to generate such code, please contact
Bluespec, Inc. support.

----------------------------------------------------------------

## References

Bluespec support: `support@bluespec.com`

Bluespec, Inc. web site [www.bluespec.com](http://www.bluespec.com).

RISC-V Foundation web site [www.riscv.org](https://riscv.org)

CVC Open Source Verilog simulator: [http://www.tachyon-da.com](http://www.tachyon-da.com)

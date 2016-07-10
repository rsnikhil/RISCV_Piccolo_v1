// Top-level driver for "verilated" objects (Verilog compiled with verilator)

#include <verilated.h>

#include "VmkTop_Sim_Standalone.h"

vluint64_t main_time = 0;    // Current simulation time

double sc_time_stamp () {    // Called by $time in Verilog
    return main_time;
}

int main (int argc, char **argv, char **env) {
    Verilated::commandArgs (argc, argv);    // remember args

    VmkTop_Sim_Standalone* mkTop_Sim_Standalone = new VmkTop_Sim_Standalone;    // create instance of model

    mkTop_Sim_Standalone->RST_N = 0;    // assert reset
    mkTop_Sim_Standalone->CLK = 0;      // assert reset

    while (! Verilated::gotFinish ()) {
	if (main_time > 10) {
	    mkTop_Sim_Standalone->RST_N = 1;    // Deassert reset
	}

	// Toggle clock
	if ((main_time % 10) == 1) {
	    mkTop_Sim_Standalone->CLK = 1;
	}
	else if ((main_time % 10) == 6) {
	    mkTop_Sim_Standalone->CLK = 0;
	}

	mkTop_Sim_Standalone->eval ();
	main_time++;
    }

    mkTop_Sim_Standalone->final ();    // Done simulating
    delete mkTop_Sim_Standalone;
}

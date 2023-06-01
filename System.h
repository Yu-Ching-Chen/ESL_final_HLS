#ifndef SYSTEM_H_
#define SYSTEM_H_
#include <systemc>
using namespace sc_core;

#include "Testbench.h"
#include "defines.h"

#ifndef NATICE_SYSTEMC
#include "PE_wrap.h"
#else
#include "PE.h"
#endif

class System: public sc_module {
    public:
        SC_HAS_PROCESS( System );
        System( sc_module_name n);
        ~System();
    private:
        Testbench tb;
#ifndef NATIVE_SYSTEMC
        PE_wrapper pe;
#else
        PE pe;
#endif
        sc_clock clk;
        sc_signal<bool> rst;
#ifndef NATIVE_SYSTEMC
//        cynw_p2p<data_t> channel_tb2pe_a; 
//        cynw_p2p<data_t> channel_tb2pe_b; 
//        cynw_p2p<angle_t> channel_tb2pe_z; 
//        cynw_p2p<bool> channel_mode; 
//        cynw_p2p<data_t> channel_pe2tb_a; 
//        cynw_p2p<data_t> channel_pe2tb_b; 
//        cynw_p2p<angle_t> channel_pe2tb_z; 
        cynw_p2p<input_t> channel_tb2pe; 
        cynw_p2p<output_t> channel_pe2tb;
#else
        sc_fifo<data_t> channel_tb2pe_a; 
        sc_fifo<data_t> channel_tb2pe_b; 
        sc_fifo<angle_t> channel_tb2pe_z; 
        sc_fifo<bool> channel_mode; 
        sc_fifo<data_t> channel_pe2tb_a; 
        sc_fifo<data_t> channel_pe2tb_b; 
        sc_fifo<angle_t> channel_pe2tb_z; 
#endif
};


#endif

#include "System.h"

System::System(sc_module_name n) : sc_module( n ),
    tb("tb"), pe("pe"), clk("clk", CLOCK_PERIOD, SC_NS), rst("rst")
{
    tb.i_clk(clk);
    tb.o_rst(rst);
    pe.i_clk(clk);
    pe.i_rst(rst);

#ifndef NATIVE_SYSTEMC
    tb.o_data(channel_tb2pe);
    tb.i_data(channel_pe2tb);

    pe.i_data(channel_tb2pe);
    pe.o_data(channel_pe2tb);
#else
    tb.o_a(channel_tb2pe_a);
    tb.o_b(channel_tb2pe_b);
    tb.o_z(channel_tb2pe_z);
    tb.o_mode(channel_mode);
    tb.i_a(channel_pe2tb_a);
    tb.i_b(channel_pe2tb_b);
    tb.i_z(channel_pe2tb_z);

    pe.i_a(channel_tb2pe_a);
    pe.i_b(channel_tb2pe_b);
    pe.i_z(channel_tb2pe_z);
    pe.i_mode(channel_mode);
    pe.o_a(channel_pe2tb_a);
    pe.o_b(channel_pe2tb_b);
    pe.o_z(channel_pe2tb_z);
#endif
    tb.read_data();
}

System::~System() {

}

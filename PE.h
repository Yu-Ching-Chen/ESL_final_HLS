#ifndef PE_H
#define PE_H

#include <systemc>
using namespace sc_core;


#ifndef NATIVE_SYSTEMC
#include <cynw_p2p.h>
#include <cynw_fixed.h>
#endif

#include "defines.h"

class PE : public sc_module {
    public:
        sc_in_clk i_clk;
        sc_in< bool > i_rst;
    #ifndef NATIVE_SYSTEMC
        // cynw_p2p<data_t>::in i_a;
        // cynw_p2p<data_t>::in i_b;
        // cynw_p2p<angle_t>::in i_z;
        // cynw_p2p<bool>::in i_mode;
        // cynw_p2p<data_t>::out o_a;
        // cynw_p2p<data_t>::out o_b;
        // cynw_p2p<angle_t>::out o_z;
        cynw_p2p<input_t>::in i_data;
        cynw_p2p<output_t>::out o_data;
    #else
        sc_fifo_in<data_t> i_a;
        sc_fifo_in<data_t> i_b;
        sc_fifo_in<angle_t> i_z;
        sc_fifo_in<bool> i_mode;
        sc_fifo_in<data_t> o_a;
        sc_fifo_in<data_t> o_b;
        sc_fifo_in<angle_t> o_z;
    #endif

        SC_HAS_PROCESS( PE );
        PE( sc_module_name n);
        ~PE() = default;

    private:
        void comp();
        CORDIC_output_t CORDIC(data_t x_0, data_t y_0, angle_t theta_0, bool mode);
        CORDIC_output_t CORDIC_one(data_t x_0, data_t y_0, angle_t theta_0, bool mode, unsigned int SFL, angle_t theta_const);
};

#endif

#ifndef TESTBENCH_H_
#define TESTBENCH_H_

#include <string>
#include <queue>
using namespace std;

#include <systemc>
using namespace sc_core;

#ifndef NATICE_SYSTEMC
#include <cynw_p2p.h>
#include <cynw_fixed.h>
#endif

#include <defines.h>

class Testbench : public sc_module {
    public:
        sc_in_clk i_clk;
        sc_out< bool > o_rst;
#ifndef NATIVE_SYSTEMC
        // cynw_p2p<data_t>::base_out o_a;
        // cynw_p2p<data_t>::base_out o_b;
        // cynw_p2p<angle_t>::base_out o_z;
        // cynw_p2p<bool>::base_out o_mode;
        // cynw_p2p<data_t>::base_in i_a;
        // cynw_p2p<data_t>::base_in i_b;
        // cynw_p2p<angle_t>::base_in i_z;
        cynw_p2p<input_t>::base_out o_data;
        cynw_p2p<output_t>::base_in i_data;
#else
        sc_fifo_out<data_t> o_a;
        sc_fifo_out<data_t> o_b;
        sc_fifo_out<angle_t> o_z;
        sc_fifo_out<bool> o_mode;
        sc_fifo_out<data_t> i_a;
        sc_fifo_out<data_t> i_b;
        sc_fifo_out<angle_t> i_z;
#endif

        int read_data();
        void output_data();

        SC_HAS_PROCESS(Testbench);
        Testbench(sc_module_name n);
        ~Testbench();

    private:
        void feed_data();
        void fetch_data();
        int testcase_n;
        double **testcase;
        double **out_data;
        queue<double> input_time_q;
        queue<double> output_time_q;
        double latency;
        double throughput;
        void latency_and_throughput();
};

#endif

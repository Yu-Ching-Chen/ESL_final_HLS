#include "PE.h"

#ifndef NATICE_SYSTEMC
#include "stratus_hls.h"
#endif

#include "angle_table.h"

PE::PE(sc_module_name n) : sc_module(n) {
    SC_THREAD(comp);
    sensitive << i_clk.pos();
    dont_initialize();
    reset_signal_is(i_rst, false);

#ifndef NATIVE_SYSTEMC
    i_data.clk_rst(i_clk, i_rst);
    o_data.clk_rst(i_clk, i_rst);
    // i_a.clk_rst(i_clk, i_rst);
    // i_b.clk_rst(i_clk, i_rst);
    // i_z.clk_rst(i_clk, i_rst);
    // i_mode.clk_rst(i_clk, i_rst);
    // o_a.clk_rst(i_clk, i_rst);
    // o_b.clk_rst(i_clk, i_rst);
    // o_z.clk_rst(i_clk, i_rst);
#endif
}

void PE::comp() {
    {
#ifndef NATIVE_SYSTEMC
        HLS_DEFINE_PROTOCOL("reset");
        i_data.reset();
        o_data.reset();
        // i_a.reset();
        // i_b.reset();
        // i_z.reset();
        // i_mode.reset();
        // o_a.reset();
        // o_b.reset();
        // o_z.reset();
        wait();
#endif
    }
    while (true) {
#ifndef NATIVE_SYSTEMC
        #ifdef PIPELINE
        HLS_PIPELINE_LOOP(SOFT_STALL, II, "comp loop pipe");
        #endif
#endif
#ifndef NATIVE_SYSTEMC
        input_t data;
        output_t output_data;
#endif
        data_t input_a;
        data_t input_b;
        angle_t input_z;
        bool input_mode;
        data_t output_a;
        data_t output_b;
        angle_t output_z;
        CORDIC_output_t CORDIC_out;
        {
#ifndef NATIVE_SYSTEMC
            HLS_DEFINE_PROTOCOL("input");
            data = i_data.get();
            input_a = data.x;
            input_b = data.y;
            input_z = data.theta;
            input_mode = data.mode;
            // input_a = i_a.get();
            // input_b = i_b.get();
            // input_z = i_z.get();
            // input_mode = i_mode.get();
#else
            input_a = i_a.read();
            input_b = i_b.read();
            input_z = i_z.read();
            input_mode = i_mode.read();
#endif
            wait();
        }
        CORDIC_out = CORDIC(input_a, input_b, input_z, input_mode);
        output_a = CORDIC_out.x;
        output_b = CORDIC_out.y;
        output_z = CORDIC_out.theta;
        {
#ifndef NATIVE_SYSTEMC
            HLS_DEFINE_PROTOCOL("output");
            output_data.x = output_a;
            output_data.y = output_b;
            output_data.theta = output_z;
            o_data.put(output_data);
            // o_a.put(output_a);
            // o_b.put(output_b);
            // o_z.put(output_z);
#else
            o_a.write(output_a);
            o_b.write(output_b);
            o_z.write(output_z);
#endif
            wait();
        }
    }
}

CORDIC_output_t PE::CORDIC(data_t x_0, data_t y_0, angle_t theta_0, bool mode) {
    data_t cor_x;
    data_t cor_y;
    angle_t cor_theta;
    CORDIC_output_t CORDIC_out;
    data_t factor = 0.6072776441;    
   
    // input correction
    if (mode) {   // vectoring mode
        if (y_0 > 0) {
            cor_x = y_0; // rotate -90 degree
            cor_y = -x_0;
            cor_theta = theta_0 + 90;
        } else {
            cor_x = -y_0; // rotate 90 degree
            cor_y = x_0;  
            cor_theta = theta_0 - 90;
        }
    } else {     // rotation mode
        if(theta_0 > 0) {
            cor_x = -y_0; // rotate 90 degree
            cor_y = x_0;  
            cor_theta = theta_0 - 90;
        } else {
            cor_x = y_0;  // rotate -90 degree
            cor_y = -x_0;
            cor_theta = theta_0 + 90;
        }
    }
    // computation
    CORDIC_out = CORDIC_one(cor_x, cor_y, cor_theta, mode, 0, ANGLE0);
    CORDIC_out = CORDIC_one(CORDIC_out.x, CORDIC_out.y, CORDIC_out.theta, mode, 1, ANGLE1);
    CORDIC_out = CORDIC_one(CORDIC_out.x, CORDIC_out.y, CORDIC_out.theta, mode, 2, ANGLE2);
    CORDIC_out = CORDIC_one(CORDIC_out.x, CORDIC_out.y, CORDIC_out.theta, mode, 3, ANGLE3);
    CORDIC_out = CORDIC_one(CORDIC_out.x, CORDIC_out.y, CORDIC_out.theta, mode, 4, ANGLE4);
    CORDIC_out = CORDIC_one(CORDIC_out.x, CORDIC_out.y, CORDIC_out.theta, mode, 5, ANGLE5);
    CORDIC_out = CORDIC_one(CORDIC_out.x, CORDIC_out.y, CORDIC_out.theta, mode, 6, ANGLE6);
    CORDIC_out.x = CORDIC_out.x * factor;
    CORDIC_out.y = CORDIC_out.y * factor;
    return CORDIC_out; 
}


CORDIC_output_t PE::CORDIC_one(data_t x_0, data_t y_0, angle_t theta_0, bool mode, unsigned int SFL, angle_t theta_const) {
    data_t tmp_x;
    data_t tmp_y;
    angle_t tmp_theta;
    data_t x;
    data_t y;
    angle_t theta;
    CORDIC_output_t o_data;

    tmp_x = x_0 >> SFL;
    tmp_y = y_0 >> SFL;
    if (mode) { // vectoring mode
        if (y > 0) { // rotate negative degree
            tmp_x = -tmp_x;
            tmp_theta = theta_const;
        } else {     // rotate positive degree
            tmp_y = -tmp_y;
            tmp_theta = -theta_const;
        }
    } else { // rotation mode
        if (theta_0 > 0) { // rotate positive degree
            tmp_y = -tmp_y;
            tmp_theta = -theta_const;
        } else {           // rotate negative degree
            tmp_x = -tmp_x;
            tmp_theta = theta_const;
        }
    }
    x = x_0 + tmp_y;
    y = tmp_x + y_0; 
    theta = theta_0 + tmp_theta;
    o_data.x = x;
    o_data.y = y;
    o_data.theta = theta;
    return o_data;
}

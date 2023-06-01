#include <cstdio>
using namespace std;

#include "Testbench.h"

Testbench::Testbench(sc_module_name n) : sc_module(n) {
    SC_THREAD(feed_data);
    sensitive << i_clk.pos();
    dont_initialize();
    SC_THREAD(fetch_data);
    sensitive << i_clk.pos();
    dont_initialize();
}

Testbench::~Testbench() {
    int i;

    output_data();
    for (i = 0; i < testcase_n; i++) delete testcase[i];
    delete testcase;
    cout << "Average latency = " << latency << " cycles" << endl;
    cout << "Average throughput = " << throughput / 1e6 << " M sample/sec" << endl;
    cout << "Simulation Finished" << endl;
}

int Testbench::read_data() {
    FILE *fp_s = NULL; // source file handler
    int i;

    fp_s = fopen("./testcase.txt", "r");
    if (fp_s == NULL) {
        printf("fopen ./testcase.txt error\n");
        return -1;
    }
    cout << "Read data start" << endl;
    fscanf(fp_s, "%d", &testcase_n);
    cout << testcase_n << endl;
    testcase = new double*[testcase_n];
    out_data = new double*[testcase_n];
    for (i = 0; i < testcase_n; i++) {
        testcase[i] = new double[3];
        out_data[i] = new double[3];
        fscanf(fp_s, "%lf %lf %lf", &testcase[i][0], &testcase[i][1], &testcase[i][2]);
    }
    fclose(fp_s);
    cout << "Read data finished" << endl;
    return 0;
}

void Testbench::output_data() {
    int i;

    FILE *fp_s = NULL; // source file handler
    fp_s = fopen("./out.txt", "w");
    for (i = 0; i < testcase_n; i++) {
        fprintf(fp_s, "%g %g %g\n", out_data[i][0], out_data[i][1], out_data[i][2]);
    }
    fclose(fp_s);
    return;
}

void Testbench::feed_data() {
    int i;
    data_t pe_a;
    data_t pe_b;
    angle_t pe_z;
#ifndef NATIVE_SYSTEMC
    input_t data;
#endif

#ifndef NATIVE_SYSTEMC
    // o_a.reset();
    // o_b.reset();
    // o_z.reset();
    // o_mode.reset();
    o_data.reset();
#endif
    o_rst.write(false);
    wait(2);
    o_rst.write(true);
    wait(1);
    cout << "Simulation start" << endl;  
    for (i = 0; i < testcase_n; i++) {
        pe_a = testcase[i][0];
        pe_b = testcase[i][1];
        pe_z = testcase[i][2];
#ifndef NATIVE_SYSTEMC
        data.x = pe_a;
        data.y = pe_b;
        data.theta = pe_z;
        data.mode = false;
        input_time_q.push(sc_time_stamp().to_double());
        o_data.put(data);
        // o_a.put(pe_a);
        // o_b.put(pe_b);
        // o_z.put(pe_z);
        // o_mode.put(false);
#else
        o_a.write(pe_a);
        o_b.write(pe_b);
        o_z.write(pe_z);
        o_mode.write(false);
#endif
        wait();
    }
    for (i = 0; i < 10000; i++) wait();
    cout << "Simulation timeout" << endl;
    sc_stop();
}

void Testbench::fetch_data() {
    int i;
    data_t pe_a;
    data_t pe_b;
    angle_t pe_z;
#ifndef NATIVE_SYSTEMC
    output_t data;
#endif

#ifndef NATIVE_SYSTEMC
    // i_a.reset();
    // i_b.reset();
    // i_z.reset();
    i_data.reset();
#endif
    wait(2);
    wait(1);
    for (i = 0; i < testcase_n; i++) {
        
#ifndef NATIVE_SYSTEMC
        data = i_data.get();
        pe_a = data.x;
        pe_b = data.y;
        pe_z = data.theta;
        // pe_a = i_a.get();
        // pe_b = i_b.get();
        // pe_z = i_z.get();

#else
        pe_a = i_a.read();
        pe_b = i_b.read();
        pe_z = i_z.read();
#endif
        out_data[i][0] = pe_a;
        out_data[i][1] = pe_b;
        out_data[i][2] = pe_z;
        output_time_q.push(sc_time_stamp().to_double());
        wait();
    }
    latency_and_throughput();
    sc_stop();
}

void Testbench::latency_and_throughput() {
    double cnt = 0;
    double lat = 0;
    double thr = 0;
    double last;
    double tmp1;
    double tmp2;
    while(!input_time_q.empty()) {
        tmp1 = input_time_q.front();
        tmp2 = output_time_q.front();
        input_time_q.pop();
        output_time_q.pop();
        lat = lat + (tmp2 - tmp1) / 1e4;
        if (cnt != 0) thr = thr + 1e12 / (tmp2 - last);
        last = tmp2;
        cnt = cnt + 1;
    }
    latency = lat / (double) cnt;
    throughput = thr / (double) (cnt - 1);
    return;
}


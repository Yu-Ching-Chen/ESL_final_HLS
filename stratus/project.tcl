#*******************************************************************************
# Copyright 2015 Cadence Design Systems, Inc.
# All Rights Reserved.
#
#*******************************************************************************
#
# Stratus Project File
#
############################################################
# Project Parameters
############################################################
#
# Technology Libraries
#
set LIB_PATH "[get_install_path]/share/stratus/techlibs/GPDK045/gsclib045_svt_v4.4/gsclib045/timing"
set LIB_LEAF "slow_vdd1v2_basicCells.lib"
use_tech_lib    "$LIB_PATH/$LIB_LEAF"

#
# Global synthesis attributes.
#
set_attr clock_period           10.0
set_attr message_detail         3
#set_attr default_input_delay    0.1
#set_attr sched_aggressive_1 on
#set_attr unroll_loops on
#set_attr flatten_arrays all
#set_attr timing_aggression 0
#set_attr default_protocol true

#
# Simulation Options
#
### 1. You may add your own options for C++ compilation here.
set_attr cc_options             "-DCLOCK_PERIOD=10.0 -g"
#enable_waveform_logging -vcd
set_attr end_of_sim_command "make saySimPassed"

#
# Testbench or System Level Modules
#
### 2. Add your testbench source files here.
define_system_module main ../main.cpp
define_system_module System ../Testbench.cpp
define_system_module tb ../System.cpp

#
# SC_MODULEs to be synthesized
#
### 3. Add your design source files here (to be high-level synthesized).
define_hls_module PE ../PE.cpp

#
# Simulation Configurations
#
use_systemc_simulator builtin
use_verilog_simulator xcelium ;# 'xcelium' or 'vcs'
enable_waveform_logging -vcd ;# to store signal transitions vcd, shm (for Xcelium), or fsdb
set_systemc_options -version 2.3 -gcc 6.3 ;# enables C++14

### 4. Define your HLS configuration (arbitrary names, BASIC and DPA in this example).
define_hls_config PE BASIC
define_hls_config PE DPA       --dpopt_auto=op,expr
define_hls_config PE PIPELINE4 -DPIPELINE -DII=4
define_hls_config PE PIPELINE3 -DPIPELINE -DII=3
define_hls_config PE PIPELINE2 -DPIPELINE -DII=2
define_hls_config PE PIPELINE1 -DPIPELINE -DII=1

### 5.1 The behavioral simulation (C++ only).
define_sim_config B
### 5.2 The Verilog simulation for HLS config "BASIC".
define_sim_config V_BASIC "PE RTL_V BASIC" 
define_sim_config V_PIPELINE4 "PE RTL_V PIPELINE4"
define_sim_config V_PIPELINE3 "PE RTL_V PIPELINE3"
define_sim_config V_PIPELINE2 "PE RTL_V PIPELINE2"
define_sim_config V_PIPELINE1 "PE RTL_V PIPELINE1"
### 5.3 The Verilog simulation for HLS config "DPA".
define_sim_config V_DPA "PE RTL_V DPA"

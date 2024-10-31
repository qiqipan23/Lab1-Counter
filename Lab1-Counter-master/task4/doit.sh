#!/bin/sh

#cleanup
rm -rf obj_dir
rm -f counter.vcd

#run Verilator to translate Verilog into C++, includeing C++ testbench
verilator -Wall --cc --trace top.sv bin2bcd.sv counter.sv --exe top_tb.cpp

#build c++ project via make automatically generated by Verilator
make -j -C obj_dir/ -f Vcounter.mk Vcounter

#run executable simulation file
obj_dir/Vcounter
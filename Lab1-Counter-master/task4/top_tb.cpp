#include "Vcounter.h"
#include "verilated.h"
#include "verilated_vcd_c.h"
#include "vbuddy.cpp"

int main(int argc, char **argv, char ** env){
    //init top verilog instance
    int i;
    int clk;

    Verilated::commandArgs(argc, argv);
    //init top verilog insatance
    Vcounter* top = new Vcounter;
    //init trace dump
    Verilated::traceEverOn(true);
    VerilatedVcdC* tfp = new VerilatedVcdC;
    top->trace (tfp, 99);
    tfp->open ("counter.vcd");

    //init Vbuddy
    if (vbdOpen()!=1) return(-1);
    vbdHeader("Lab 1: Counter");

    //set Vbuddy to one-shot mode
    vbdSetMode(1);

    //initialize simulation inputs
    top->clk = 1;
    top->rst = 1;
    top->ld = 0;

    //run simulation for many clock cycles
    for(i = 0; i < 300; i++){

        //dump variables into VCD file and toggle clock
        for(clk = 0; clk < 2; clk ++){
            tfp->dump (2*i+clk);  //unit is in ps!!!
            top->clk = !top->clk;
            top->eval ();
        }


        vbdHex(4, (int(top->count) >> 16) & 0xF);
        vbdHex(3, (int(top->count) >> 8) & 0xF);
        vbdHex(2, (int(top->count) >> 4) & 0xF);
        vbdHex(1, int(top->count) & 0xF);
        vbdCycle(i+1);

        //++++ Send Count value to Vbuddy
        /*vbdPlot((int(top->count) >> 16), 0, 255);
        vbdPlot((int(top->count) >> 8), 0, 255);
        vbdPlot((int(top->count) >> 4), 0, 255);
        vbdPlot(int(top->count), 0, 255);
        vbdCycle(i+1);*/
        //---- enf od Cbuddy output section

        top->rst = (i < 2) | (i == 15);
        top->ld = vbdFlag(); // read flag, which resets to 0 in ONE-SHOT Mode
        if(Verilated::gotFinish()) exit(0);
    }

    vbdClose();
    tfp->close();
    exit(0);
} 
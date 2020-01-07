`timescale 1ns / 1ps
`include "Parameter.v"
// fpga4student.com 
// FPGA projects, VHDL projects, Verilog projects 
// Verilog code for RISC Processor 
// Verilog testbench code to test the processor
module test_Risc_16_bit;

 // Inputs
 reg clk;

 // Instantiate the Unit Under Test (UUT)
 Risc_16_bit uut (
  .clk(clk)
 );

 initial 
  begin
   clk <=0;
   `simulation_time;
   //$finish;
  end

 always 
  begin
   #5 clk = ~clk;
  end

endmodule
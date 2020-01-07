`include "control.v"

`include "datapath.v"

module mips (clk,rst);
input clk,rst;
wire regdst,ALUsrc,memtoreg,regwr,memwr,branch,jump,extop,zero;
wire [5:0]op;
wire [31:0] busA,busB,npc,result,inscode;
wire [2:0] ALUop;
wire [31:0] extout,dmout,mux_result,mux2_data;
wire [15:0] imm16;
wire [4:0]rs,rt,rd;
wire [25:0] Target;
wire [31:0]pc;
wire [5:0]func;

datapath path(pc,npc,busA,busB,result,inscode,clk,rst,op,func,regdst,ALUsrc,memtoreg,regwr,memwr,branch,jump,ALUop,extop,extout,dmout,mux_result,mux2_data,imm16,rs,rt,rd,Target,zero);

control control_sign(op,func,regdst,ALUsrc,memtoreg,regwr,memwr,branch,jump,ALUop,extop);

endmodule 
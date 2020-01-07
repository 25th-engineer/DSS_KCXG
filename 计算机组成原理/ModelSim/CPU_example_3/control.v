
`include"ctrl.v"
module control(op,func,regdst,ALUsrc,memtoreg,regwr,memwr,branch,jump,ALUop,extop);
/*module ctrl(op,regdst,ALUsrc,memtoreg,regwr,memwr,branch,jump,ALUop);
input [5:0]op;
output regdst; 
output ALUsrc; //alu??????
output memtoreg;
output regwr;
output memwr;
output branch;
output jump;
output [2:0] ALUop;*/
input [5:0]op,func;
output wire regdst,ALUsrc,memtoreg,regwr,memwr,branch,jump,extop;
output wire [2:0]ALUop;
ctrl contrl(op,func,regdst,ALUsrc,memtoreg,regwr,memwr,branch,jump,ALUop,extop);

endmodule 
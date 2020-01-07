
module test_ctrl();
reg [5:0]op,func;
wire regdst,ALUsrc,memtoreg,regwr,memwr,branch,jump,extop;
wire [3:0]ALUop;
ctrl mm(op,func,regdst,ALUsrc,memtoreg,regwr,memwr,branch,jump,ALUop,extop);
initial 
 begin 
#0 op=6'b001001;
#0 func=6'b100000;

#10 op=6'b000000;
# 20 op=6'b000010;
# 30 op=6'b001101;
# 40 op=6'b001101;
# 50 op=6'b101011;
end 
endmodule 

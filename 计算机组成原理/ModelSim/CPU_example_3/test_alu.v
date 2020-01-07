
module ALUtest();
reg [2:0]op;
reg  [31:0] a,b;
wire zero;
wire [31:0] resule;
ALU mm(op,a,b,zero,resule);
initial 
begin 
#0 a=32'b00000000000000000000011000011101;
#0 b=32'b00000000000000000000011000100110;
#0   op=3'b000;
#10 op=3'b001;
#10 op=3'b010;

end
endmodule

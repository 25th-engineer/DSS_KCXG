
module signext #(parameter WIDTH=16)(
input [WIDTH-1:0]imm16,
input extop,
output reg[31:0]imm32);

always @(imm16 or extop)
 begin
    if (extop==0||imm16[15]==0)
	imm32={16'b0000000000000000,imm16};
	else
        imm32 ={16'b1111111111111111,imm16};
end
endmodule 
    
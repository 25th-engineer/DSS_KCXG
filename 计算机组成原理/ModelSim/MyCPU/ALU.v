module alu(outalu, a, b, select);
output [15:0] outalu;
input [15:0]a, b;
input [2:0]select;

reg [15:0] outalu;

always @(*)
case(select)
	3'b000: outalu = a + b;
	3'b001: outalu = a - b;
	3'b010: outalu = a&b;
	3'b011: outalu = a|b;
	3'b100: outalu = a<<b;
	3'b101: outalu = a>>b;
	3'b110: outalu = a*b;
	3'b111: outalu = a%b;
	default: outalu = 16'b1111111111111111;
endcase	

endmodule

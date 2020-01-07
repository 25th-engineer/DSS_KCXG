module acc(
	input ena, 
	input [1:0]accop, 
	input wire [15:0]accin, 
	output reg [15:0]acc
);
always @(*) begin
	if(ena ==1 )begin
	case(accop)
	2'b00: acc = 0;
	2'b01: acc = ~acc;
	2'b10: acc = acc>>1;
	2'b11: acc = {acc[14:0],acc[15]};
	default acc = accin;
	endcase
	end
end
endmodule

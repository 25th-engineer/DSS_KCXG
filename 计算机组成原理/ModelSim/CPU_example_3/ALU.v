
module ALU(
input [2:0] ALUctr,
input [31:0] rs, rt,
output zero,
output reg [31:0] ALUout
);   
assign zero = (ALUout== 32'd0) ? 1'b1 : 1'b0;
always @ (ALUctr or rs or rt)
	begin
		case(ALUctr)
			3'b000:  ALUout = rs+ rt;
			3'b001:  ALUout = rs+ rt;
			3'b010:  ALUout = rt|rs;
			3'b011:  ALUout <= rs|rt;
			3'b100:  ALUout = rs-rt;
			3'b101:  ALUout = rs-rt;
			3'b110:  ALUout = (rs<rt)?1:0;
			3'b111:  ALUout = (rs<rt)?1:0;
			default : ALUout =0;
		endcase
	end

endmodule

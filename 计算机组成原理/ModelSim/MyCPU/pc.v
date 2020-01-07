module pc(
	input stop,
	input clk,
	input rst,
	input pcJMP,
	input banEBL,
	input ban,
	input wire [7:0] data_in,
	output reg [7:0] pc
);
always @(posedge clk )begin
	if(rst==1) begin pc = 0;end
	if(stop!=1)begin 
	pc = pc +1;
	     		if(banEBL == 1) begin 
				if(ban == 1) begin 
				pc = pc + data_in;
				end
			end
			if(pcJMP == 1 ) begin pc = data_in;end
	end
end
endmodule


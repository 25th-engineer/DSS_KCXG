module Datastorage(
	input wire [5:0] address,
	input wire enable,
	input wire clk,
	output reg [15:0] dataout,
	input wire [15:0] datain
);

reg [15:0] storage [63:0];

always @(negedge clk)begin 
	if(enable)storage[address] = datain;
	else dataout = storage[address];
	end
endmodule

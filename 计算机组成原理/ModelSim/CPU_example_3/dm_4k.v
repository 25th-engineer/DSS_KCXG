
module dm_4k (
input [31:0]addr,
input [31:0] din,
input we,
input clk,
output [31:0] dout
);

reg [31:0] datamem[31:0];

assign dout = datamem[addr];
 

always @(posedge clk)
if(we)
      datamem[addr]<=din;

endmodule 
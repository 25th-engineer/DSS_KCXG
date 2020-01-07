
module im_4k(
input [31:0] addr,
output reg [31:0] dout);

reg [31:0]im [1024:0];
initial
begin 
$readmemh("code.txt",im);
end
always @(addr)
begin
   dout=im[(addr-32'o0000_3000)];
end


endmodule
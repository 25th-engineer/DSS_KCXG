

module dm_4k_test();
reg [31:0] addr,din;
reg we,clk;
wire [31:0]dout;
dm_4k mn(addr,din,we,clk,dout);

initial
 begin 
#0 clk =1'b0;
#0 we=1'b1;
# 0 din=32'b0000000000000000000000000000001;
#0 addr=32'b0000000000000000000000000000001;
# 10 clk=~clk;
end 
endmodule
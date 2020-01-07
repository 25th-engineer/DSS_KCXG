

`include "mips.v"
module test_cpu();
reg rst;
reg clk;

mips mycpu(clk,rst);

always #30 clk=!clk;
initial 
begin 
clk=0;
rst=1;
#30;
rst=0;
end
endmodule 
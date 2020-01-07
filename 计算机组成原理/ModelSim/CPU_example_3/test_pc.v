
module pc_test();
reg clk,rst;
reg [31:0]npc;
wire  [31:0] pc;
pc mm(npc,clk,rst,pc);
initial 
 begin 
#0 clk=1'b0;

npc=32'o0000_4512;
#0 rst=1'b1;
#10 rst=1'b0;
#10 clk=~clk;
end 
endmodule 
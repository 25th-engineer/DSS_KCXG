
module pc(npc,clk,rst,pc);
input clk;
input rst;
input [31:0] npc;
output [31:0] pc;
reg [31:0]pc;
initial 
begin
pc=32'o0000_3000;
end
always @(posedge clk or negedge rst)
   begin 
	if (rst==1)
             pc=32'o0000_3000;
         else 
              pc=npc;
   end
endmodule
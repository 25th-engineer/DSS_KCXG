

module npc_test();
reg zero,branch;
wire [31:0]npc;
reg  [31:0] pc;

reg [15:0] Imm16; //???
wire [25:0]Target;
wire jump;

npc mm(zero,branch,Imm16,pc,npc,Target,jump);
initial 
 begin 
#0 zero=1'b0;
#0 branch=1'b0;
#0 pc=32'b00000000000000000000000000000000;
#0 Imm16=16'b0000001010000000;
#10 zero=1'b1;
#10 branch=1'b1;
 end
endmodule 
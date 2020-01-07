

module npc(zero,branch,Imm16,pc,npc,Target,jump);
input zero ;
input branch ;  
input [31:0] pc ;
input [15:0] Imm16;
input [25:0]Target;
input jump;
output [31:0] npc;

wire [31:0] pc_plus_4,pc_br;
assign pc_plus_4=pc+1;
wire [15:0]mm;
assign mm= (Imm16[15])?16'b1111111111111111:
                       16'b0000000000000000;
assign pc_br= {mm,Imm16}+pc_plus_4;
 
  mux ming(pc_plus_4,pc_br,!zero&branch,npc);


  //assign  npc=jump?{6'b000000,Target}:pc_plus_4;


endmodule  
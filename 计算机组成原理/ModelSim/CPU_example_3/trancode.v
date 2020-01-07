module trancode (inscode,op,rs,rt,rd,Target,imm16,func);
input [31:0] inscode;
output [5:0] op,func;
output [4:0]rs,rt,rd;
output [25:0] Target;
output [15:0] imm16;
//output [5:0] func;
assign op=inscode[31:26];
assign rs=inscode[25:21];
assign rt=inscode[20:16];
assign rd=inscode[15:11];
assign imm16=inscode[15:0];
assign Target=inscode[25:0];
assign  func=inscode[5:0];
endmodule 
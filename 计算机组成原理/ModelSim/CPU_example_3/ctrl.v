
module ctrl(op,func,regdst,ALUsrc,memtoreg,regwr,memwr,branch,jump,ALUop,extop);
input [5:0]op,func;
output regdst; 
output ALUsrc; //alu??????
output memtoreg;
output regwr;
output memwr;
output branch;
output jump,extop;
output [2:0] ALUop;

assign regdst= (op==6'b000000)? 1:0;
assign branch= (op==6'b000100)? 1:0;
assign jump =  (op==6'b000010)? 1:0;
assign ALUsrc= (op==6'b001101 || op==6'b001001 || op==6'b100011 ||op==6'b101011)?1:0;
assign memtoreg =(op==6'b100011)?1:0;
assign regwr= (op==6'b000000 || op==6'b001101 ||op==6'b001001 || op==6'b100011) ?1:0;
assign memwr = (op==6'b101011) ?1:0;
assign extop= (op==6'b001001 || op==6'b100011 ||op==6'b101011)? 1:0;
assign ALUop[2]=(op!=6'b000000)?!op[5]&!op[4]&!op[3]&op[2]&!op[1]&!op[0]:(!func[2]&func[1]);
assign ALUop[1]=(op!=6'b000000)?!op[5]&!op[4]&op[3]&op[2]&!op[1]&op[0]:(func[3]&!func[2]&func[1]+!func[3]&func[2]&!func[1]&func[0]);
assign ALUop[0]=(op!=6'b000000)?!op[5]&!op[4]&!op[3]&!op[2]&!op[1]&!op[0]:(!func[3]&!func[2]&!func[0]+!func[2]&func[1]&!func[0]);
endmodule 
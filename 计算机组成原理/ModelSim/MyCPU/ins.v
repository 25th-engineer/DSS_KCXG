module ins(
	output reg[15:0] outins,
	input wire[7:0] adder,
	output reg[5:0] r1,
	output reg[7:0] r2,
	input wire clk
);
reg [15:0] storage [0:255];
reg [15:0] temp;
initial begin
 storage [8'h00] = 16'h0000; //start
 storage [8'h01] = 16'h0100; //cla
 storage [8'h02] = 16'h0200; //com
 storage [8'h03] = 16'h0300; //shr
 storage [8'h04] = 16'h0400; //csl
 storage [8'h05] = 16'h0500; //add x
 storage [8'h06] = 16'h0600; //sta x
 storage [8'h07] = 16'h0701; //lda x
 storage [8'h08] = 16'h0803; //jmp
 storage [8'h09] = 16'h0902; //ban
 storage [8'h0a] = 16'hffff; //stop
end

	always @(adder)begin 
	temp = storage[adder];
	r1 = {storage[adder][5:0]};
	r2 = {storage[adder][7:0]};
	outins  = temp;
	end
endmodule 
		
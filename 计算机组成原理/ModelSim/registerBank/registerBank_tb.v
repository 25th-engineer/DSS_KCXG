module registerBank_tb();
  parameter CYCLE = 2;
  
	reg [2:0] addrA, addrB, inAddr;
	wire [15:0] outA, outB;
	reg [15:0] inData;
	reg clk, WE;
 
	initial begin
    clk = 1'b0;
    //WE = 1'b0;
  end
  
  always #(CYCLE/2) clk = ~ clk;
  always #(CYCLE/2) WE = ~ WE;
  
	initial begin
    
    //assign WE = 1'b0;
    assign WE = 1'b1;
    assign inData = 16'h35A1;
    assign inAddr = 3'b010;
    assign addrA = 3'b010;
    assign addrB = 3'b100;
    
    #(CYCLE*1) 
    assign WE = 1'b1;
    assign inData = 16'h1234;
    assign inAddr = 3'b001;
    assign addrA = 3'b000;
    assign addrB = 3'b001;
    
    #(CYCLE*2)
    assign WE = 1'b0;
    assign inData = 16'h65df;
    assign inAddr = 3'b101;
    assign addrA = 3'b101;
    assign addrB = 3'b010;
    
    #(CYCLE*3)
    assign WE = 1'b1;
    assign inData = 16'h12ce;
    assign inAddr = 3'b100;
    assign addrA = 3'b100;
    assign addrB = 3'b011;
    
    #(CYCLE*2)
    assign WE = 1'b1;
    assign inData = 16'hefd3;
    assign inAddr = 3'b110;
    assign addrA = 3'b011;
    assign addrB = 3'b110;
    
    #(CYCLE*5)
    $stop;
    
	end
  
   
  registerBank reg_file(
	.clk(clk), .WE(WE),
    .addrA(addrA), .addrB(addrB), .inData(inData),
    .inAddr(inAddr),
    .outA(outA), .outB(outB)
  );
endmodule
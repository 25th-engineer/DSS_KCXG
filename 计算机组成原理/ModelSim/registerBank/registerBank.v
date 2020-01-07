module registerBank(
    input [2:0] addrA,addrB,inAddr,
    output wire[15:0] outA,outB,
    input wire[15:0] inData,
    input clk,WE
);

    reg[15:0] regs[0:7];
  
    assign outA = regs[addrA];
    assign outB = regs[addrB];
  
    always@(negedge clk)begin
        if( WE == 1 )
            regs[inAddr] = inData;
    end
endmodule
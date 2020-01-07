module MyPC(
    input clk, 
    input rst,
    //input wire [7:0] initAddr,
    output [7:0] pcAddr
);
    reg [7:0] tmpPC = 8'b0;
    
    always @(posedge clk or posedge rst) begin
        if(rst)
            tmpPC = 8'b0;//8'b01011010;
        //else if(tmpPC == 8'b0)
            //tmpPC = initAddr;
        else
            tmpPC = tmpPC + 1'b1;
    end
    assign pcAddr = tmpPC;
endmodule

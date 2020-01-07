module  MyRAM #( parameter DATA_WIDTH = 16,DEPTH = 1024)(
    //input;
    input    wire    clk,
    input    wire    WE, //write low enable;
    input    wire    [clogb2(DEPTH)-1:0] address,
    input    wire    [DATA_WIDTH-1:0] dataIn,
    //output;
    output   wire     [DATA_WIDTH-1:0] dataOut     
);

function integer clogb2 (input integer depth);
begin
    for (clogb2=0; depth>1; clogb2=clogb2+1) 
        depth = depth >>1;                          
end
endfunction 

(* ramstyle = "M9K" *) reg [DATA_WIDTH-1:0] memory[0:DEPTH-1];
reg [clogb2(DEPTH)-1:0] address_reg;
//read;

always @(posedge clk ) begin
    address_reg <= address;
end //always

assign dataOut = memory[address_reg];
//write;

always @(posedge clk ) begin
    if(~WE) begin
        memory[address] <= dataIn;
    end
end //always

endmodule
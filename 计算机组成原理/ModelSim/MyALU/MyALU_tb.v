module MyALU_tb();
    reg [15:0] inData1, inData2;
    reg [3:0] select [0:7];
    wire [15:0] ans;
    wire zero, carry, negative, overflow;
    
    initial begin
        inData1 = 16'b0101010101010101;
        inData2 = 16'b1010101010101010;
		
        #10 select[0] = 3'b000;
        #20 select[1] = 3'b001;
        #30 select[2] = 3'b010;
        #40 select[3] = 3'b011;
        #50 select[4] = 3'b100;
        #60 select[5] = 3'b101;
        #70 select[6] = 3'b110;
        #80 select[7] = 3'b111;
        
		    #100 $stop;
    end
	
    
    ALU test1(
        .x(inData1), .y(inData2),
        .select(select[0]),
        .ans(ans),
        .zero(zero),
        .carry(carry),
        .negative(negative),
        .overflow(overflow)
    );
	
	ALU test2(
        .x(inData1), .y(inData2),
        .select(select[1]),
        .ans(ans),
        .zero(zero),
        .carry(carry),
        .negative(negative),
        .overflow(overflow)
    );
	
	ALU test3(
        .x(inData1), .y(inData2),
        .select(select[2]),
        .ans(ans),
        .zero(zero),
        .carry(carry),
        .negative(negative),
        .overflow(overflow)
    );
	
	ALU test4(
        .x(inData1), .y(inData2),
        .select(select[3]),
        .ans(ans),
        .zero(zero),
        .carry(carry),
        .negative(negative),
        .overflow(overflow)
    );
	
	ALU test5(
        .x(inData1), .y(inData2),
        .select(select[4]),
        .ans(ans),
        .zero(zero),
        .carry(carry),
        .negative(negative),
        .overflow(overflow)
    );
	
	ALU test6(
        .x(inData1), .y(inData2),
        .select(select[5]),
        .ans(ans),
        .zero(zero),
        .carry(carry),
        .negative(negative),
        .overflow(overflow)
    );
	
	ALU test7(
        .x(inData1), .y(inData2),
        .select(select[6]),
        .ans(ans),
        .zero(zero),
        .carry(carry),
        .negative(negative),
        .overflow(overflow)
    );
	
	ALU test8(
        .x(inData1), .y(inData2),
        .select(select[7]),
        .ans(ans),
        .zero(zero),
        .carry(carry),
        .negative(negative),
        .overflow(overflow)
    );
endmodule
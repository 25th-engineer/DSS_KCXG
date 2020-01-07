module cu(
	input wire[7:0] accout,
	input wire [15:0] outins,
	output reg stop,
	output reg [1:0]accop,
	output reg ena,
	output reg [2:0] aluop,
	output reg enable,
	output reg pcJMP,banEBL,ban
);
always @* begin
	case(outins[15:8])
	8'h00:
	begin 
		stop = 0;
	end
	8'hff:
	begin 
		stop = 1;
	end
	8'h01:
	begin 
		ena  = 1; 
		accop = 2'b00;
		banEBL = 0;
		pcJMP=0;
	end
	8'h02:
	begin 
		ena  = 1; 
		accop = 2'b01;
		banEBL = 0;
		pcJMP=0;
	end
	8'h03:
	begin 
		ena  = 1; 
		accop = 2'b10;
		banEBL = 0;
		pcJMP=0;
	end
	8'h04:
	begin 
		ena  = 1; 
		accop = 2'b11;
		banEBL = 0;
		pcJMP=0;
	end
	8'h05:
	begin 
		ena  = 1; 
		enable = 0;
		aluop = 3'b000;
		banEBL = 0;
		pcJMP=0; 
	end
	8'h06:
	begin 
		ena  = 1; 
		enable = 1;
		banEBL = 0;
		pcJMP=0;
	end
	8'h07:
	begin 
		ena  = 1; 
		enable = 0;
		banEBL = 0;
		pcJMP = 0;
	end
	8'h08:
	begin 
		pcJMP = 1 ;
	end
	8'h09:
	begin 
		banEBL = 1 ;
	if(accout[7] == 1)
		begin 
			ban = 1 ;
		end	
	end

	endcase
end
endmodule 
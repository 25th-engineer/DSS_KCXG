
module regfile(
input clk,
input [4:0] rd,rs,rt,
input [31:0]busw,
output [31:0]busA,busB,
input regwr,regdst
);

reg [31:0] registerfile[31:0];
     integer i;
    initial 
      for (i=0;i<=0;i=i+1)
     begin
        registerfile[i] = 0;
     end

assign busA=registerfile [rs];
assign busB=registerfile[rt];

always @(posedge clk)
begin 
      if (regdst==1&& regwr==1)   
	 begin
		 registerfile[rd]<=busw;
          
	 end
       else if(regdst==0&&regwr==1)
          begin
                registerfile[rt]<=busw;
	  end
end 
endmodule 
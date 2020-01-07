
module mux2_test();
parameter width=32;
reg [width-1:0] d0,d1;
reg s;
wire [width-1:0]y;

mux2 an(d0,d1,s,y0);
initial 
begin
#0 d0=32'd1;
#0 d1=32'd0;
#0 s=1'b0;
#10 s=s;
#40 s=~s;
end
endmodule


module signnext_text();
parameter WIDTH= 16;
reg [WIDTH-1:0] a;
wire [31:0] y;
signext an(a,1'b1,y);
initial
begin
#0 a=16'd78;
#100 a=16'd45;
end
endmodule
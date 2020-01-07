library verilog;
use verilog.vl_types.all;
entity registerBank is
    port(
        addrA           : in     vl_logic_vector(2 downto 0);
        addrB           : in     vl_logic_vector(2 downto 0);
        inAddr          : in     vl_logic_vector(2 downto 0);
        outA            : out    vl_logic_vector(15 downto 0);
        outB            : out    vl_logic_vector(15 downto 0);
        inData          : in     vl_logic_vector(15 downto 0);
        clk             : in     vl_logic;
        WE              : in     vl_logic
    );
end registerBank;

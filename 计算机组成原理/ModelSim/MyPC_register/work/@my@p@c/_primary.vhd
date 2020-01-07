library verilog;
use verilog.vl_types.all;
entity MyPC is
    port(
        clk             : in     vl_logic;
        rst             : in     vl_logic;
        pcAddr          : out    vl_logic_vector(7 downto 0)
    );
end MyPC;

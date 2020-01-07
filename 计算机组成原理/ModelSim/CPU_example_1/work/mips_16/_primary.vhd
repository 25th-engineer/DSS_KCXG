library verilog;
use verilog.vl_types.all;
entity mips_16 is
    port(
        clk             : in     vl_logic;
        reset           : in     vl_logic;
        pc_out          : out    vl_logic_vector(15 downto 0);
        alu_result      : out    vl_logic_vector(15 downto 0)
    );
end mips_16;

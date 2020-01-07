library verilog;
use verilog.vl_types.all;
entity ALU is
    port(
        a               : in     vl_logic_vector(15 downto 0);
        b               : in     vl_logic_vector(15 downto 0);
        alu_control     : in     vl_logic_vector(2 downto 0);
        result          : out    vl_logic_vector(15 downto 0);
        zero            : out    vl_logic
    );
end ALU;

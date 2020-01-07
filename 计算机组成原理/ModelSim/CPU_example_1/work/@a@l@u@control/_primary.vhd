library verilog;
use verilog.vl_types.all;
entity ALUControl is
    port(
        ALU_Control     : out    vl_logic_vector(2 downto 0);
        ALUOp           : in     vl_logic_vector(1 downto 0);
        \Function\      : in     vl_logic_vector(3 downto 0)
    );
end ALUControl;

library verilog;
use verilog.vl_types.all;
entity alu_control is
    port(
        ALU_Cnt         : out    vl_logic_vector(2 downto 0);
        ALUOp           : in     vl_logic_vector(1 downto 0);
        Opcode          : in     vl_logic_vector(3 downto 0)
    );
end alu_control;

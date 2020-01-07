library verilog;
use verilog.vl_types.all;
entity Instruction_Memory is
    port(
        pc              : in     vl_logic_vector(15 downto 0);
        instruction     : out    vl_logic_vector(15 downto 0)
    );
end Instruction_Memory;

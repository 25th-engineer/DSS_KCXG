library verilog;
use verilog.vl_types.all;
entity instr_mem is
    port(
        pc              : in     vl_logic_vector(15 downto 0);
        instruction     : out    vl_logic_vector(15 downto 0)
    );
end instr_mem;

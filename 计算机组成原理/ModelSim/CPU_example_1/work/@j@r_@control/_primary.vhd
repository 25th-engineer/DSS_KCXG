library verilog;
use verilog.vl_types.all;
entity JR_Control is
    port(
        alu_op          : in     vl_logic_vector(1 downto 0);
        funct           : in     vl_logic_vector(3 downto 0);
        JRControl       : out    vl_logic
    );
end JR_Control;

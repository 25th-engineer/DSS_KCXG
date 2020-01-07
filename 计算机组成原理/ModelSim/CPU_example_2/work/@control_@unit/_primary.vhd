library verilog;
use verilog.vl_types.all;
entity Control_Unit is
    port(
        opcode          : in     vl_logic_vector(3 downto 0);
        alu_op          : out    vl_logic_vector(1 downto 0);
        jump            : out    vl_logic;
        beq             : out    vl_logic;
        bne             : out    vl_logic;
        mem_read        : out    vl_logic;
        mem_write       : out    vl_logic;
        alu_src         : out    vl_logic;
        reg_dst         : out    vl_logic;
        mem_to_reg      : out    vl_logic;
        reg_write       : out    vl_logic
    );
end Control_Unit;

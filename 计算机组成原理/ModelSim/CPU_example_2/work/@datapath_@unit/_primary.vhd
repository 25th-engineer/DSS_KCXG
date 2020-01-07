library verilog;
use verilog.vl_types.all;
entity Datapath_Unit is
    port(
        clk             : in     vl_logic;
        jump            : in     vl_logic;
        beq             : in     vl_logic;
        mem_read        : in     vl_logic;
        mem_write       : in     vl_logic;
        alu_src         : in     vl_logic;
        reg_dst         : in     vl_logic;
        mem_to_reg      : in     vl_logic;
        reg_write       : in     vl_logic;
        bne             : in     vl_logic;
        alu_op          : in     vl_logic_vector(1 downto 0);
        opcode          : out    vl_logic_vector(3 downto 0)
    );
end Datapath_Unit;

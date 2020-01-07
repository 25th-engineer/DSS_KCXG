library verilog;
use verilog.vl_types.all;
entity Data_Memory is
    port(
        clk             : in     vl_logic;
        mem_access_addr : in     vl_logic_vector(15 downto 0);
        mem_write_data  : in     vl_logic_vector(15 downto 0);
        mem_write_en    : in     vl_logic;
        mem_read        : in     vl_logic;
        mem_read_data   : out    vl_logic_vector(15 downto 0)
    );
end Data_Memory;

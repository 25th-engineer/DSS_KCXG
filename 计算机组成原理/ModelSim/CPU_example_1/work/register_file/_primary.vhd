library verilog;
use verilog.vl_types.all;
entity register_file is
    port(
        clk             : in     vl_logic;
        rst             : in     vl_logic;
        reg_write_en    : in     vl_logic;
        reg_write_dest  : in     vl_logic_vector(2 downto 0);
        reg_write_data  : in     vl_logic_vector(15 downto 0);
        reg_read_addr_1 : in     vl_logic_vector(2 downto 0);
        reg_read_data_1 : out    vl_logic_vector(15 downto 0);
        reg_read_addr_2 : in     vl_logic_vector(2 downto 0);
        reg_read_data_2 : out    vl_logic_vector(15 downto 0)
    );
end register_file;

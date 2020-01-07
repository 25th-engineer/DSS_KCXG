library verilog;
use verilog.vl_types.all;
entity MyRAM_tb is
    generic(
        CYCLE           : integer := 8;
        ADDR_WIDTH      : integer := 10;
        DATA_WIDTH      : integer := 16
    );
    attribute mti_svvh_generic_type : integer;
    attribute mti_svvh_generic_type of CYCLE : constant is 1;
    attribute mti_svvh_generic_type of ADDR_WIDTH : constant is 1;
    attribute mti_svvh_generic_type of DATA_WIDTH : constant is 1;
end MyRAM_tb;

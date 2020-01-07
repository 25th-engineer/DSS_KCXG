library verilog;
use verilog.vl_types.all;
entity MyRAM is
    generic(
        DATA_WIDTH      : integer := 16;
        DEPTH           : integer := 1024
    );
    port(
        clk             : in     vl_logic;
        WE              : in     vl_logic;
        address         : in     vl_logic_vector;
        dataIn          : in     vl_logic_vector;
        dataOut         : out    vl_logic_vector
    );
    attribute mti_svvh_generic_type : integer;
    attribute mti_svvh_generic_type of DATA_WIDTH : constant is 1;
    attribute mti_svvh_generic_type of DEPTH : constant is 1;
end MyRAM;

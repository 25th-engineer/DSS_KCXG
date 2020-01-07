library verilog;
use verilog.vl_types.all;
entity registerBank_tb is
    generic(
        CYCLE           : integer := 2
    );
    attribute mti_svvh_generic_type : integer;
    attribute mti_svvh_generic_type of CYCLE : constant is 1;
end registerBank_tb;

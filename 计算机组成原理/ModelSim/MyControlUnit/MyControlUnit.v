module CtrlUnit(

        input [31:0] instr,

        input [31:0] rdata1, rdata2,
        input [31:0] pc,ram_data,hi,lo,alu_r,cop_data,mul_out,

        input [31:0] exc_addr,
        input clk,

        output mtc0,eret,teq_exc,
        output reg [2:0] mdu,
        output reg reg_wena,
        output ram_wena,
        
        output reg [3:0] cause,
        output [4:0] rs,rt,rd,waddr,
        output reg [31:0] wdata,reg_data,
        output [31:0] ram_addr,
        output reg [31:0] pc_in,
        
        output reg [31:0] alu_a,alu_b,
        output reg [3:0] alu_op
        );
    
    
    //******MIPS-55*********//
    parameter   //SPECIAL OP LIST   5-0
                ADDU    =   6'b100001,
                SUBU    =   6'b100011,
                ADD     =   6'b100000,
                SUB     =   6'b100010,
                AND     =   6'b100100,
                OR      =   6'b100101,
                XOR     =   6'b100110,
                NOR     =   6'b100111,
                SLT     =   6'b101010,
                SLTU    =   6'b101011,
                SRL     =   6'b000010,
                SRA     =   6'b000011,
                SLL     =   6'b000000,
                SLLV    =   6'b000100,
                SRLV    =   6'b000110,
                SRAV    =   6'b000111,
                JR      =   6'b001000,
                JALR    =   6'b001001,
                MULT    =    6'b011000,
                MULTU   =   6'b011001,
                DIV     =   6'b011010,
                DIVU    =   6'b011011,
                MFHI    =   6'b010000,
                MFLO    =   6'b010010,
                MTHI    =   6'b010001,
                MTLO    =   6'b010011,
                BREAK   =   6'b001101,
                SYSCALL =   6'b001100,
                TEQ     =   6'b110100,
                //SPECIAL 2 func
                CLZ        =   6'b100000,
                MUL        =   6'b000010,
                //REGIMM OP LIST 20-16
                BLTZ    =   5'b00000,
                BGEZ    =   5'b00001,
                //COP0 OP LIST 
                ERET    =   6'b011000,  //5-0&&25TH=1
                MFC0    =   5'b00000,   //20-16
                MTC0    =   5'b00100,
                //OPCODE FIELD  31-26
                ADDI    =   6'b001000,
                ADDIU   =   6'b001001,
                ANDI    =   6'b001100,
                ORI     =   6'b001101,
                XORI    =   6'b001110,
                LW      =   6'b100011,
                SW      =   6'b101011,
                BEQ     =   6'b000100,
                BNE     =   6'b000101,
                BLEZ    =   6'b000110,
                BGTZ    =   6'b000111,
                SLTI    =   6'b001010,
                SLTIU   =   6'b001011,
                LUI     =   6'b001111,
                J       =   6'b000010,
                JAL     =   6'b000011,
                LB      =   6'b100000,//    Load Byte Function=6'h24    
                LBU     =   6'b100100,//    1Load Byte Unsigned 
                LH      =   6'b100001,//    Load high 
                LHU     =   6'b100101,//    Load High Unsigned
                SB      =   6'b101000,//    Send Byte
                SH      =   6'b101001,//    Send High
                
                SPECIAL =   6'b000000,
                SPECIAL2=    6'b011100,
                REGIMM  =   6'b000001,
                COP0    =   6'b010000;  

    //ALU OPCODE                
    parameter   _ADDU   =   4'b0000;    //r=a+b unsigned
    parameter   _ADD    =   4'b0010;    //r=a+b signed
    parameter   _SUBU   =   4'b0001;    //r=a-b unsigned
    parameter   _SUB    =   4'b0011;    //r=a-b signed
    parameter   _AND    =   4'b0100;    //r=a&b
    parameter   _OR     =   4'b0101;    //r=a|b
    parameter   _XOR    =   4'b0110;    //r=a^b
    parameter   _NOR    =   4'b0111;    //r=~(a|b)
    
    parameter   _LUI    =   4'b1000;    //r={b[15:0],16'b0}
    parameter   _SLT    =   4'b1011;    //r=(a-b<0)?1:0 signed
    parameter   _SLTU   =   4'b1010;    //r=(a-b<0)?1:0 unsigned
    parameter   _SRA    =   4'b1100;    //r=b>>>a 
    parameter   _SLL    =   4'b1110;    //r=b<<a
    parameter   _SRL    =   4'b1101;    //r=b>>a
    
    parameter    _SYSCALL=   4'b1000,
                _BREAK    =   4'b1001,
                _TEQ    =   4'b1101;
            
    wire [5:0]  op        =  instr[31:26];
    assign         rs        =  instr[25:21];
    assign         rt        =  instr[20:16];
    assign        rd        =  instr[15:11];
    wire [5:0]  func    =  instr[5:0];
    
    wire [4:0]  shamt   =  instr[10:6];
    wire [15:0] imm     =  instr[15:0];
    wire [25:0] addr    =  instr[25:0];
    
    parameter   SIGN    =   1'b1;
    parameter   UNSIGN  =   1'b0;
    wire     imm_sign    =   (op==ANDI||op==ORI||op==XORI)?UNSIGN:SIGN;
    
    wire [31:0] shamt_ext        =   {27'b0,shamt};
    wire [31:0] imm_ext            =   imm_sign?{{(16){imm[15]}},imm}:{16'b0,imm};

    reg  [31:0] load_data,clz_data;
    
    assign    waddr    =    (op==SPECIAL||op==SPECIAL2)?    rd:    (op==JAL)    ?5'b11111:rt;
    
    always@(*)begin
        case(op)
            SB:     reg_data    =   {24'b0,rdata2[7:0]};
            SH:     reg_data    =   {16'b0,rdata2[15:0]};
            SW:     reg_data    =   rdata2;
            default:reg_data    =    rdata2;
        endcase
        
        case(op)
            LB:     load_data   =   {{24{ram_data[7]}},ram_data[7:0]};
            LBU:    load_data   =   {24'b0,ram_data[7:0]};
            LH:     load_data   =   {{16{ram_data[15]}},ram_data[15:0]};
            LHU:    load_data   =   {16'b0,ram_data[15:0]};
            LW:     load_data   =   ram_data;
            default:load_data   =   ram_data;
        endcase
        
        if(op==SPECIAL)
            case(func)
                SYSCALL:cause    =    _SYSCALL;
                BREAK:    cause    =    _BREAK;
                TEQ:    cause    =    _TEQ;
                default:cause    =    5'b0;
            endcase
        else             cause    =    5'b0;
        
        /*
   mult
   multu
   div    
   divu
   mthi
   mtlo
   mul
        */
        
        if(op==SPECIAL)
            case(func)
                MULT:    mdu        =    3'h1;
                MULTU:    mdu        =    3'h2;
                DIV:    mdu        =    3'h3;
                DIVU:    mdu        =    3'h4;
                MTHI:    mdu        =    3'h5;
                MTLO:    mdu        =    3'h6;
                default:mdu        =    3'h0;
            endcase
        else             mdu        =    3'h0;
    end
    
    wire [31:0] npc                =   pc+4;
    
    wire [31:0] pc_branch        =   npc    +    {{(14){imm[15]}},imm,2'b00};
    wire [31:0] pc_jmp            =    {npc[31:28],addr,2'b00};

    assign    ram_addr    =   rdata1  +   imm_ext;
    assign    eret        =    op==COP0 && func==ERET;
    wire    mfc0        =    op==COP0 && rs==MFC0;
    assign    mtc0        =    op==COP0 && rs==MTC0;
    assign    teq_exc        =    rdata1==rdata2;
    
    parameter   ENA     =   1'b1;
    parameter   DIS     =   1'b0;
            
    wire mem_load   =   op==LB || op==LH || op==LBU || op==LHU || op==LW;
    assign ram_wena =   op==SW || op==SH || op==SB;
    
    integer i;    
    
    always@(*)begin

        
        case(op)
            SPECIAL: case(func)
                MULTU,
                DIV,
                DIVU,
                JR,
                MTHI,
                MTLO,
                BREAK,
                SYSCALL:reg_wena    =   DIS;
                default:reg_wena    =   ENA;    
            endcase
            COP0:       reg_wena    =   rs==MFC0?ENA:DIS;
            SPECIAL2,
            LB,
            LBU,
            LH,
            LHU,
            ADDI,
            ADDIU,
            ANDI,
            ORI,
            XORI,
            LW,
            SLTI,
            SLTIU,
            LUI,
            JAL:        reg_wena    =   ENA;            
            default:    reg_wena    =   DIS;
        endcase
        
        case(op)
            SPECIAL:case(func)
                JALR:    wdata   =   npc;
                MFHI:   wdata   =   hi;
                MFLO:   wdata   =   lo;
                default:wdata   =   alu_r;
            endcase 
            SPECIAL2:begin
                if(func==CLZ) casez(rdata1)
                    32'b1???????????????????????????????:   wdata   =       32'h0;
                    32'b01??????????????????????????????:   wdata   =       32'h1;
                    32'b001?????????????????????????????:   wdata   =       32'h2;
                    32'b0001????????????????????????????:   wdata   =       32'h3;
                    32'b00001???????????????????????????:   wdata   =       32'h4;
                    32'b000001??????????????????????????:   wdata   =       32'h5;
                    32'b0000001?????????????????????????:   wdata   =       32'h6;
                    32'b00000001????????????????????????:   wdata   =       32'h7;
                    32'b000000001???????????????????????:   wdata   =       32'h8;
                    32'b0000000001??????????????????????:   wdata   =       32'h9;
                    32'b00000000001?????????????????????:   wdata   =       32'ha;
                    32'b000000000001????????????????????:   wdata   =       32'hb;
                    32'b0000000000001???????????????????:   wdata   =       32'hc;
                    32'b00000000000001??????????????????:   wdata   =       32'hd;
                    32'b000000000000001?????????????????:   wdata   =       32'he;
                    32'b0000000000000001????????????????:   wdata   =       32'hf;
                    32'b00000000000000001???????????????:   wdata   =       32'h10;
                    32'b000000000000000001??????????????:   wdata   =       32'h11;
                    32'b0000000000000000001?????????????:   wdata   =       32'h12;
                    32'b00000000000000000001????????????:   wdata   =       32'h13;
                    32'b000000000000000000001???????????:   wdata   =       32'h14;
                    32'b0000000000000000000001??????????:   wdata   =       32'h15;
                    32'b00000000000000000000001?????????:   wdata   =       32'h16;
                    32'b000000000000000000000001????????:   wdata   =       32'h17;
                    32'b0000000000000000000000001???????:   wdata   =       32'h18;
                    32'b00000000000000000000000001??????:   wdata   =       32'h19;
                    32'b000000000000000000000000001?????:   wdata   =       32'h1a;
                    32'b0000000000000000000000000001????:   wdata   =       32'h1b;
                    32'b00000000000000000000000000001???:   wdata   =       32'h1c;
                    32'b000000000000000000000000000001??:   wdata   =       32'h1d;
                    32'b0000000000000000000000000000001?:   wdata   =       32'h1e;
                    32'b00000000000000000000000000000001:   wdata   =        32'h1f;
                    32'b00000000000000000000000000000000:    wdata   =       32'h20;
                endcase
                 else if(func==MUL)    wdata    =    mul_out;
                 else                wdata   =   alu_r;
            end
            JAL:                    wdata   =   npc;
            LW,LB,LH,LBU,LHU:        wdata   =   load_data;
            COP0:    if(rs==MFC0)    wdata   =   cop_data;
                    else            wdata   =   alu_r;
            default:                wdata   =   alu_r;
        endcase

        //Below is OK
        
        case(op)
            SPECIAL: case(func)
                SLL,
                SRL,
                SRA:begin
                    alu_a   =   shamt_ext;
                    alu_b   =   rdata2;
                end
                default:begin
                    alu_a   =   rdata1;
                    alu_b   =   rdata2;
                end
            endcase
            ADDI,
            ADDIU,
            ANDI,
            ORI,
            XORI,
            SLTI,
            SLTIU,
            LUI:begin
                    alu_a   =   rdata1;
                    alu_b   =   imm_ext;
            end
            default:begin
                    alu_a   =   rdata1;
                    alu_b   =   rdata2;
            end
        endcase
        
        //PC Source     
        case(op)
            SPECIAL: case(func)
                SYSCALL,
                TEQ,
                BREAK:  pc_in  =   exc_addr;
                JALR,
                JR:     pc_in  =   rdata1;   
                default:pc_in  =   npc; 
            endcase
            COP0: case(func)
                ERET:   pc_in  =   exc_addr;
                default:pc_in  =   npc;
            endcase
            REGIMM: case(rt)
                BLTZ:if(rdata1[31])
                        pc_in  =   pc_branch;
                    else
                        pc_in  =   npc;
                BGEZ:if(!rdata1[31])
                        pc_in  =   pc_branch;
                    else
                        pc_in  =   npc;
                default:pc_in  =   npc;
            endcase
            J,
            JAL:        pc_in  =   pc_jmp;
            BEQ:if(rdata1==rdata2)
                        pc_in  =   pc_branch;
                else    pc_in  =   npc;
            BNE:if(rdata1!=rdata2)
                        pc_in  =   pc_branch;
                else    pc_in  =   npc;
            BLEZ:if(rdata1[31] || rdata1==32'b0)
                        pc_in  =   pc_branch;
                else    pc_in  =   npc;
            BGTZ:if(!rdata1[31] && rdata1!=32'b0)
                        pc_in  =   pc_branch;
                else    pc_in  =   npc;
            default:    pc_in  =   npc; 
        endcase
            
        case(op)
            SPECIAL:case(func)
                ADDU:       alu_op  =   _ADDU;
                SUBU:       alu_op  =   _SUBU;
                ADD:        alu_op  =   _ADD;
                SUB:        alu_op  =   _SUB;
                AND:        alu_op  =   _AND;
                OR:         alu_op  =   _OR;
                XOR:        alu_op  =   _XOR;
                NOR:        alu_op  =   _NOR;
                SLT:        alu_op  =   _SLT;
                SRL:        alu_op  =   _SRL;
                SLL:        alu_op  =   _SLL;
                SRA:        alu_op  =   _SRA;
                SLTU:       alu_op  =   _SLTU;
                SRLV:       alu_op  =   _SRL;
                SLLV:       alu_op  =   _SLL;
                SRAV:       alu_op  =   _SRA;
                default:    alu_op  =   _ADDU;
            endcase 
            ORI:            alu_op  =   _OR;
            XORI:           alu_op  =   _XOR;
            BEQ:            alu_op  =   _SUBU;
            BNE:            alu_op  =   _SUBU;
            ANDI:           alu_op  =   _AND;
            ADDIU:          alu_op  =   _ADDU;
            ADDI:           alu_op  =   _ADD;
            SLTI:           alu_op  =   _SLT;
            SLTIU:          alu_op  =   _SLTU;
            LUI:            alu_op  =   _LUI;
            default:        alu_op  =   _ADDU;
        endcase
        
    end
endmodule


;/********************************************************************************************************/

;定义堆栈的大小，根据需要改变
FIQ_STACK_LEGTH     EQU         0
IRQ_STACK_LEGTH     EQU         64
ABT_STACK_LEGTH     EQU         0
UND_STACK_LEGTH     EQU         0
VICVectAddr         EQU         0xFFFFF030
NoInt       EQU 0x80


;引入的外部标号在这声明
        IMPORT  FIQ_Exception                   ;快速中断异常处理程序
        IMPORT  SoftwareInterrupt               ;软中断入口
        IMPORT  Reset                           ;复位向量入口
        IMPORT  OSIntCtxSw                      ;中断中任务切换函数
        IMPORT  OSIntExit                       ;中断退出函数
        IMPORT  OSTCBCur                        ;指向当前任务TCB的指针
        IMPORT  OSTCBHighRdy                    ;指向将要运行的任务TCB的指针
  
        IMPORT  OSIntNesting                    ;中断嵌套计数器


;给外部使用的标号在这声明，InitStack是必须的
        EXPORT  InitStack                       
        EXPORT  Vectors


    CODE32
    PRESERVE8
    AREA    StartUp,CODE,READONLY

    ENTRY
;中断向量表
Vectors
        LDR     PC, ResetAddr
        LDR     PC, UndefinedAddr
        LDR     PC, SWI_Addr
        LDR     PC, PrefetchAddr
        LDR     PC, DataAbortAddr
        DCD     0xb9205f80
        LDR     PC, [PC, #-0xff0]
        LDR     PC, FIQ_Addr

ResetAddr           DCD     Reset
UndefinedAddr       DCD     Undefined
SWI_Addr            DCD     SoftwareInterrupt
PrefetchAddr        DCD     PrefetchAbort
DataAbortAddr       DCD     DataAbort
nouse               DCD     0
IRQ_Addr            DCD     IRQ_Handler
FIQ_Addr            DCD     FIQ_Handler


    MACRO
$IRQ_Label HANDLER $IRQ_Exception

        EXPORT  $IRQ_Label                      ; 输出的标号
        IMPORT  $IRQ_Exception                  ; 引用的外部标号

$IRQ_Label
        SUB     LR, LR, #4                      ; 计算返回地址
        STMFD   SP!, {R0-R3, R12, LR}           ; 保存任务环境
        MRS     R3, SPSR                        ; 保存状态
        STMFD   SP!, {R3}

        LDR     R2,  =OSIntNesting              ; OSIntNesting++
        LDRB    R1, [R2]
        ADD     R1, R1, #1
        STRB    R1, [R2]
        
        BL      $IRQ_Exception                  ; 调用c语言的中断处理程序

        MSR     CPSR_c, #0x92
        BL      OSIntExit

        LDR     R0, =OSTCBHighRdy
        LDR     R0, [R0]
        LDR     R1, =OSTCBCur
        LDR     R1, [R1]
        CMP     R0, R1
        
        LDMFD   SP!, {R3}
        MSR     SPSR_cxsf, R3

        LDMEQFD SP!, {R0-R3, R12, PC}^          ; 不进行任务切换
        LDR     PC, =OSIntCtxSw                 ; 进行任务切换
    MEND

;未定义指令
Undefined
        b       Undefined

;取指令中止
PrefetchAbort
        b       PrefetchAbort

;取数据中止
DataAbort
        b       DataAbort

;中断
IRQ_Handler	HANDLER IRQ_Exception

;快速中断
FIQ_Handler
        STMFD   SP!, {R0-R3, LR}
        BL      FIQ_Exception
        LDMFD   SP!, {R0-R3, LR}
        SUBS    PC,  LR,  #4

;定时器0中断
Timer0_Handler  HANDLER Timer0

;/*********************************************************************************************************
;** 函数名称: InitStack
;** 功能描述: 初始化堆栈
;********************************************************************************************************/
InitStack    
        MOV     R0, LR
;设置中断模式堆栈
        MSR     CPSR_c, #0xd2
        LDR     SP, StackIrq
;设置快速中断模式堆栈
        MSR     CPSR_c, #0xd1
        LDR     SP, StackFiq
;设置中止模式堆栈
        MSR     CPSR_c, #0xd7
        LDR     SP, StackAbt
;设置未定义模式堆栈
        MSR     CPSR_c, #0xdb
        LDR     SP, StackUnd
;设置系统模式堆栈
        MSR     CPSR_c, #0xdf
        LDR     SP, StackIrq
        MOV     PC, R0

StackIrq           DCD     (IrqStackSpace + IRQ_STACK_LEGTH * 4 - 4)
StackFiq           DCD     (FiqStackSpace + FIQ_STACK_LEGTH * 4 - 4)
StackAbt           DCD     (AbtStackSpace + ABT_STACK_LEGTH * 4 - 4)
StackUnd           DCD     (UndtStackSpace + UND_STACK_LEGTH * 4 - 4)

;/* 分配堆栈空间 */
        AREA    MyStacks, DATA, NOINIT
IrqStackSpace      SPACE   IRQ_STACK_LEGTH * 4  ;中断模式堆栈空间
FiqStackSpace      SPACE   FIQ_STACK_LEGTH * 4  ;快速中断模式堆栈空间
AbtStackSpace      SPACE   ABT_STACK_LEGTH * 4  ;中止义模式堆栈空间
UndtStackSpace     SPACE   UND_STACK_LEGTH * 4  ;未定义模式堆栈




    END
;/*********************************************************************************************************
;**                            End Of File
;********************************************************************************************************/

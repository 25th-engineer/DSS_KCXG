;/****************************************Copyright (c)**************************************************
;**                               广州周立功单片机发展有限公司
;**                                     研    究    所
;**                                        产品一部 
;**
;**                                 http://www.zlgmcu.com
;**
;**--------------文件信息--------------------------------------------------------------------------------
;**文   件   名: vectors.s
;**创   建   人: 陈明计
;**最后修改日期: 2003年6月13日
;**描        述: lpc210x异常向量入口及异常向量与c语言代码的接口(for μCOS-II)，包括初始化堆栈的代码
;**              每个工程应当有独立的这个文件的拷贝，并进行相应的修改   
;**--------------历史版本信息----------------------------------------------------------------------------
;** 创建人: 陈明计
;** 版  本: v1.0
;** 日　期: 2003年6月5日
;** 描　述: 原始版本
;**
;**------------------------------------------------------------------------------------------------------
;** 修改人: 陈明计
;** 版  本: v1.1
;** 日　期: 2003年6月9日
;** 描　述: 使用宏使得中断程序更加容易编写
;**
;**------------------------------------------------------------------------------------------------------
;** 修改人: 陈明计
;** 版  本: v1.2
;** 日　期: 2003年6月11日
;** 描　述: 更改IRQ嵌套丢失CPSR的BUG
;**
;**------------------------------------------------------------------------------------------------------
;** 修改人: 陈明计
;** 版  本: v1.2
;** 日　期: 2003年6月13日
;** 描　述: 按照μCOS-II V2.52的要求修改（以前是基于μCOS-II V2.0）
;**
;**------------------------------------------------------------------------------------------------------
;** 修改人: 陈明计
;** 版  本: V1.3
;** 日　期: 2003年6月19日
;** 描　述: 不完全按照μCOS-II V2.52的要求以提高效率
;**
;**------------------------------------------------------------------------------------------------------
;** 修改人: 陈明计
;** 版  本: V1.4
;** 日　期: 2003年10月9日
;** 描　述: 修改中断嵌套时的一个隐蔽错误，极端情况下它会造成系统崩溃
;**
;**--------------当前版本修订------------------------------------------------------------------------------
;** 修改人: 
;** 日　期:
;** 描　述:
;**
;**------------------------------------------------------------------------------------------------------
;********************************************************************************************************/

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
;** 输　入:   无
;** 输　出 :  无
;** 全局变量: 无
;** 调用模块: 无
;** 
;** 作　者: 陈明计
;** 日　期: 2003年6月5日
;**-------------------------------------------------------------------------------------------------------
;** 修　改: 
;** 日　期: 
;**-------------------------------------------------------------------------------------------------------
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

;/****************************************Copyright (c)**************************************************
;
;**------------------------------------------------------------------------------------------------------
;********************************************************************************************************/
;定义系统模式堆栈的大小
SVC_STACK_LEGTH     EQU         32

NoInt       EQU 0x80

USR32Mode   EQU 0x10
SVC32Mode   EQU 0x13
SYS32Mode   EQU 0x1f
IRQ32Mode   EQU 0x12
FIQ32Mode   EQU 0x11

;T_bit用于检测进入异常前cpu是否处于THUMB状态
T_bit               EQU         0x20

    CODE32
    PRESERVE8
    AREA    |subr|, CODE, READONLY

            IMPORT  OSTCBCur                    ;指向当前任务TCB的指针
            IMPORT  OSTCBHighRdy                ;指向将要运行的任务TCB的指针
            IMPORT  OSPrioCur                   ;当前任务的优先级
            IMPORT  OSPrioHighRdy               ;将要运行的任务的优先级
            IMPORT  OSTaskSwHook                ;任务切换的钩子函数
            IMPORT  OSRunning                   ;uC/OS-II运行标志

            IMPORT  OsEnterSum                  ;关中断计数器（关中断信号量）
            IMPORT  SWI_Exception               ;软中断异常处理程序
            
            EXPORT  __OSStartHighRdy            
            EXPORT  OSIntCtxSw                  ;中断退出时的入口，参见startup.s中的IRQ_Handler
            EXPORT  SoftwareInterrupt           ;软中断入口


;/*********************************************************************************************************
;** 函数名称: SoftwareInterrupt
;** 功能描述: 软件中断，用于提供一些系统服务，功能参考os_cpu_c.c文件
;** 输　入:   依功能而定
;** 输　出 :  依功能而定
;** 全局变量: 无
;** 调用模块: SWI_Exception

;**-------------------------------------------------------------------------------------------------------
;********************************************************************************************************/

;软件中断
SoftwareInterrupt
        LDR     SP, StackSvc            ; 重新设置堆栈指针
        STMFD   SP!, {R0-R3, R12, LR}
        MOV     R1, SP                  ; R1指向参数存储位置

        MRS     R3, SPSR
        TST     R3, #T_bit              ; 中断前是否是Thumb状态
        LDRNEH  R0, [LR,#-2]            ; 是: 取得Thumb状态SWI号
        BICNE   R0, R0, #0xff00
        LDREQ   R0, [LR,#-4]            ; 否: 取得arm状态SWI号
        BICEQ   R0, R0, #0xFF000000
                                        ; r0 = SWI号，R1指向参数存储位置
        CMP     R0, #1
        LDRLO   PC, =OSIntCtxSw
        LDREQ   PC, =__OSStartHighRdy   ; SWI 0x01为第一次任务切换

        BL      SWI_Exception
        
        LDMFD   SP!, {R0-R3, R12, PC}^
        
StackSvc           DCD     (SvcStackSpace + SVC_STACK_LEGTH * 4 - 4)

;/*********************************************************************************************************
;** 函数名称: OSIntCtxSw
;** 功能描述: 中断退出时的入口
;** 输　入:   R3    :当前任务的状态寄存器CPSR（即SPSR的值）
;**           R4-R12:当前任务的R4-R11
;**           当前处理器模式的堆栈结构（出栈次序）：R0-R3、R12、PC（当前任务的）
;** 输　出 :  无
;** 全局变量: OSPrioCur,OSPrioHighRdy,OSPrioCur,OSPrioHighRdy
;** 调用模块: 无

;**-------------------------------------------------------------------------------------------------------
;********************************************************************************************************/
OSIntCtxSw
                                                    ;下面为保存任务环境
        LDR     R2, [SP, #20]                       ;获取PC
        LDR     R12, [SP, #16]                      ;获取R12
        MRS     R0, CPSR

        MSR     CPSR_c, #(NoInt | SYS32Mode)
        MOV     R1, LR
        STMFD   SP!, {R1-R2}                        ;保存LR,PC
        STMFD   SP!, {R4-R12}                       ;保存R4-R12

        MSR     CPSR_c, R0
        LDMFD   SP!, {R4-R7}                        ;获取R0-R3
        ADD     SP, SP, #8                          ;出栈R12,PC
        
        MSR     CPSR_c, #(NoInt | SYS32Mode)
        STMFD   SP!, {R4-R7}                        ;保存R0-R3
        
        LDR     R1, =OsEnterSum                     ;获取OsEnterSum
        LDR     R2, [R1]
        STMFD   SP!, {R2, R3}                       ;保存CPSR,OsEnterSum

                                                    ;保存当前任务堆栈指针到当前任务的TCB
        LDR     R1, =OSTCBCur
        LDR     R1, [R1]
        STR     SP, [R1]

        BL      OSTaskSwHook                        ;调用钩子函数
                                                    ;OSPrioCur <= OSPrioHighRdy
        LDR     R4, =OSPrioCur
        LDR     R5, =OSPrioHighRdy
        LDRB    R6, [R5]
        STRB    R6, [R4]
                                                    ;OSTCBCur <= OSTCBHighRdy
        LDR     R6, =OSTCBHighRdy
        LDR     R6, [R6]
        LDR     R4, =OSTCBCur
        STR     R6, [R4]
OSIntCtxSw_1
                                                    ;获取新任务堆栈指针
        LDR     R4, [R6]
        ADD     SP, R4, #68                         ;17寄存器CPSR,OsEnterSum,R0-R12,LR,SP
        LDR     LR, [SP, #-8]
        MSR     CPSR_c, #(NoInt | SVC32Mode)        ;进入管理模式
        MOV     SP, R4                              ;设置堆栈指针

        LDMFD   SP!, {R4, R5}                       ;CPSR,OsEnterSum
                                                    ;恢复新任务的OsEnterSum
        LDR     R3, =OsEnterSum
        STR     R4, [R3]
    
        MSR     SPSR_cxsf, R5                       ;恢复CPSR
        LDMFD   SP!, {R0-R12, LR, PC }^             ;运行新任务

;/*********************************************************************************************************
;** 函数名称: __OSStartHighRdy
;** 功能描述: uC/OS-II启动时使用OSStartHighRdy运行第一个任务,
;**           OSStartHighRdy会调用__OSStartHighRdy
;** 输　入:   无
;** 输　出 :  无
;** 全局变量: OSRunning,OSTCBCur,OSTCBHighRdy,OsEnterSum
;** 调用模块: OSTaskSwHook
;** 
;** 作　者: 陈明计
;** 日　期: 2003年6月5日
;**-------------------------------------------------------------------------------------------------------
;** 修　改: 陈明计
;** 日　期: 2003年6月13日
;**-------------------------------------------------------------------------------------------------------
;********************************************************************************************************/

__OSStartHighRdy
        MSR     CPSR_c, #(NoInt | SYS32Mode)
                                                ;告诉uC/OS-II自身已经运行
        LDR     R4, =OSRunning
        MOV     R5, #1
        STRB    R5, [R4]

        BL      OSTaskSwHook                    ;调用钩子函数

        LDR     R6, =OSTCBHighRdy
        LDR     R6, [R6]
        B       OSIntCtxSw_1

        AREA    SWIStacks, DATA, NOINIT
;,ALIGN=2
SvcStackSpace      SPACE   SVC_STACK_LEGTH * 4  ;管理模式堆栈空间

    END
;/*********************************************************************************************************
;**                            End Of File
;********************************************************************************************************/

/****************************************Copyright (c)**************************************************
**                               广州周立功单片机发展有限公司
**                                     研    究    所
**                                        产品一部 
**
**                                 http://www.zlgmcu.com
**
**--------------文件信息--------------------------------------------------------------------------------
**文   件   名: os_cpu_c.c
**创   建   人: 陈明计
**最后修改日期: 2003年7月8日
**描        述: μCOS-II在lpc210x上的移植代码C语言部分，包括任务堆栈初始化代码和钩子函数等
**              用ads1.2编译，必须使用ARM方式编译
**
**--------------历史版本信息----------------------------------------------------------------------------
** 创建人: 陈明计
** 版  本: 1.0
** 日　期: 2003年6月5日
** 描　述: 原始版本
**
**------------------------------------------------------------------------------------------------------
** 修改人: 陈明计
** 版  本: 1.1
** 日　期: 2003年6月13日
** 描　述: 增加函数IsrEnIRQ，使函数OSTaskStkInit代码更简洁
**
**------------------------------------------------------------------------------------------------------
** 修改人: 陈明计
** 版  本: 1.2
** 日　期: 2003年6月19日
** 描　述: 增加软中断服务函数
**
**------------------------------------------------------------------------------------------------------
** 修改人: 陈明计
** 版  本: 1.3
** 日　期: 2003年7月8日
** 描　述: 去除不必要的函数IsrEnIRQ
**
**--------------当前版本修订------------------------------------------------------------------------------
** 修改人: 
** 日　期: 
** 描　述: 
**
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
#define  OS_CPU_GLOBALS
#include "config.h"


/*********************************************************************************************************
** 函数名称: OSTaskStkInit
** 功能描述: 任务堆栈初始化代码，本函数调用失败会使系统崩溃
** 输　入: task  : 任务开始执行的地址
**         pdata ：传递给任务的参数
**         ptos  ：任务的堆栈开始位置
**         opt   ：附加参数，当前版本对于本函数无用，具体意义参见OSTaskCreateExt()的opt参数
** 输　出: 栈顶指针位置
** 全局变量:
** 调用模块: 
**
** 作　者: 陈明计
** 日　期: 2003年6月5日
**-------------------------------------------------------------------------------------------------------
** 修改人: 陈明计
** 日　期: 2003年6月13日
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/

        OS_STK *OSTaskStkInit (void (*task)(void *pd), void *pdata, OS_STK *ptos, INT16U opt)
{
    OS_STK *stk;

    opt    = opt;                           /* 'opt'  没有使用。作用是避免编译器警告    */
    stk    = ptos;                          /* 获取堆栈指针                                       */

                                            /* 建立任务环境，ADS1.2使用满递减堆栈       */
    *stk = (OS_STK) task;                   /*  pc  */
    *--stk = (OS_STK) task;                 /*  lr  */

    *--stk = 0;                             /*  r12  */
    *--stk = 0;                             /*  r11  */
    *--stk = 0;                             /*  r10  */
    *--stk = 0;                             /*  r9   */
    *--stk = 0;                             /*  r8   */
    *--stk = 0;                             /*  r7   */
    *--stk = 0;                             /*  r6   */
    *--stk = 0;                             /*  r5   */
    *--stk = 0;                             /*  r4   */
    *--stk = 0;                             /*  r3   */
    *--stk = 0;                             /*  r2   */
    *--stk = 0;                             /*  r1   */
    *--stk = (unsigned int) pdata;          /*  r0,第一个参数使用R0传递   */
    *--stk = (USER_USING_MODE|0x00);	    /*  spsr，允许 IRQ, FIQ 中断   */
    *--stk = 0;                             /*  关中断计数器OsEnterSum;    */

    return (stk);
}


/*********************************************************************************************************
** 函数名称: SWI_Exception
** 功能描述: 软中断异常处理程序，提供一些系统服务
**           
** 输　入:  SWI_Num:功能号
**          Regs[0] 为第一个参数，也是返回值
**          Regs[1] 为第二个参数
**          Regs[2] 为第三个参数
**          Regs[3] 为第四个参数
** 输　出: 根据功能而定
**         
** 全局变量: 无
** 调用模块: 无
**
** 作　者: 陈明计
** 日　期: 2003年6月5日
**-------------------------------------------------------------------------------------------------------
** 修改人: 陈明计
** 日　期: 2003年6月19日
**-------------------------------------------------------------------------------------------------------
** 修改人: 陈明计
** 日　期: 2003年6月24日
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
#if OS_SELF_EN > 0 
extern int const _OSFunctionAddr[];
extern int const _UsrFunctionAddr[];
#endif
        void SWI_Exception(int SWI_Num, int *Regs)
{
    OS_TCB   *ptcb;
    
    switch(SWI_Num)
    {
        //case 0x00:                    /* 任务切换函数OS_TASK_SW，参考os_cpu_s.s文件     */
        //    break;
        //case 0x01:                    /* 启动任务函数OSStartHighRdy，参考os_cpu_s.s文件 */
        //    break;
        case 0x02:                      /* 关中断函数OS_ENTER_CRITICAL()，参考os_cpu.h文件 */
            __asm
            {
                MRS     R0, SPSR
                ORR     R0, R0, #NoInt
                MSR     SPSR_c, R0
            }
            OsEnterSum++;
            break;
        case 0x03:                      /* 开中断函数OS_EXIT_CRITICAL()，参考os_cpu.h文件 */
            if (--OsEnterSum == 0)
            {
                __asm
                {
                    MRS     R0, SPSR
                    BIC     R0, R0, #NoInt
                    MSR     SPSR_c, R0
                }
            }
            break;
#if OS_SELF_EN > 0 
        case 0x40:
                                        /* 返回指定系统服务函数的地址       */
                                        /* 函数地址存于数组_OSFunctionAddr中*/
                                        /* 数组_OSFunctionAddr需要另外定义  */
                                        /* Regs[0] 为第一个参数，也是返回值 */
                                        /* Regs[1] 为第二个参数             */
                                        /* Regs[2] 为第三个参数             */
                                        /* Regs[3] 为第四个参数             */
                                        /* 仅有一个参数为系统服务函数的索引 */
            Regs[0] =  _OSFunctionAddr[Regs[0]];
            break;
        case 0x41:
                                        /* 返回指定用户的服务函数的地址     */
                                        /* 函数地址存于数组_UsrFunctionAddr中*/
                                        /* 数组_UsrFunctionAddr需要另外定义 */
                                        /* Regs[0] 为第一个参数，也是返回值 */
                                        /* Regs[1] 为第二个参数             */
                                        /* Regs[2] 为第三个参数             */
                                        /* Regs[3] 为第四个参数             */
                                        /* 仅有一个参数为用户服务函数的索引 */
            Regs[0] =  _UsrFunctionAddr[Regs[0]];
            break;
        case 0x42:                      /* 中断开始处理 */
            OSIntNesting++;
            break;
        case 0x43:                      /*  判断中断是否需要切换             */
            if (OSTCBHighRdy == OSTCBCur)
            {
                Regs[0] = 0;
            }
            else
            {
                Regs[0] = 1;
            }
            break;
#endif
        case 0x80:                      /* 任务切换到系统模式 */
            __asm
            {
                MRS     R0, SPSR
                BIC     R0, R0, #0x1f
                ORR     R0, R0, #SYS32Mode    
                MSR     SPSR_c, R0
            }
            break;
        case 0x81:                      /* 任务切换到用户模式 */
            __asm
            {
                MRS     R0, SPSR
                BIC     R0, R0, #0x1f
                ORR     R0, R0, #USR32Mode    
                MSR     SPSR_c, R0
            }
            break;
        case 0x82:                      /* 任务是ARM代码 */
            if (Regs[0] <= OS_LOWEST_PRIO)
            {
                ptcb = OSTCBPrioTbl[Regs[0]];
                if (ptcb != NULL)
                {
                    ptcb -> OSTCBStkPtr[1] &= ~(1 << 5);
                }
            }
            break;
        case 0x83:                      /* 任务是THUMB代码 */
            if (Regs[0] <= OS_LOWEST_PRIO)
            {
                ptcb = OSTCBPrioTbl[Regs[0]];
                if (ptcb != NULL)
                {
                    ptcb -> OSTCBStkPtr[1] |= (1 << 5);
                }
            }
            break;
        default:
            break;
    }
}

/*********************************************************************************************************
** 函数名称: OSStartHighRdy
** 功能描述: uC/OS-II启动时使用OSStartHighRdy运行第一个任务,
**           实质是产生swi 1指令
** 输　入:   无
** 输　出 :  无
** 全局变量: 无
** 调用模块: 无
** 
** 作　者: 陈明计
** 日　期: 2003年6月5日
**-------------------------------------------------------------------------------------------------------
** 修　改: 
** 日　期: 
**-------------------------------------------------------------------------------------------------------
********************************************************************************************************/

        void OSStartHighRdy(void)
{
    _OSStartHighRdy();
}


/* 以下为一些钩子函数，全部为空函数。具体说明请看相关资料 */

#if OS_CPU_HOOKS_EN
/*
*********************************************************************************************************
*                                       OS INITIALIZATION HOOK
*                                            (BEGINNING)
*
* Description: This function is called by OSInit() at the beginning of OSInit().
*
* Arguments  : none
*
* Note(s)    : 1) Interrupts should be disabled during this call.
*********************************************************************************************************
*/
#if OS_VERSION > 203
void OSInitHookBegin (void)
{
}
#endif

/*
*********************************************************************************************************
*                                       OS INITIALIZATION HOOK
*                                               (END)
*
* Description: This function is called by OSInit() at the end of OSInit().
*
* Arguments  : none
*
* Note(s)    : 1) Interrupts should be disabled during this call.
*********************************************************************************************************
*/
#if OS_VERSION > 203
void OSInitHookEnd (void)
{
}
#endif


/*
*********************************************************************************************************
*                                          TASK CREATION HOOK
*
* Description: This function is called when a task is created.
*
* Arguments  : ptcb   is a pointer to the task control block of the task being created.
*
* Note(s)    : 1) Interrupts are disabled during this call.
*********************************************************************************************************
*/
void OSTaskCreateHook (OS_TCB *ptcb)
{
    ptcb = ptcb;                       /* Prevent compiler warning                                     */
}


/*
*********************************************************************************************************
*                                           TASK DELETION HOOK
*
* Description: This function is called when a task is deleted.
*
* Arguments  : ptcb   is a pointer to the task control block of the task being deleted.
*
* Note(s)    : 1) Interrupts are disabled during this call.
*********************************************************************************************************
*/
void OSTaskDelHook (OS_TCB *ptcb)
{
    ptcb = ptcb;                       /* Prevent compiler warning                                     */
}

/*
*********************************************************************************************************
*                                           TASK SWITCH HOOK
*
* Description: This function is called when a task switch is performed.  This allows you to perform other
*              operations during a context switch.
*
* Arguments  : none
*
* Note(s)    : 1) Interrupts are disabled during this call.
*              2) It is assumed that the global pointer 'OSTCBHighRdy' points to the TCB of the task that
*                 will be 'switched in' (i.e. the highest priority task) and, 'OSTCBCur' points to the 
*                 task being switched out (i.e. the preempted task).
*********************************************************************************************************
*/
void OSTaskSwHook (void)
{
}

/*
*********************************************************************************************************
*                                           STATISTIC TASK HOOK
*
* Description: This function is called every second by uC/OS-II's statistics task.  This allows your 
*              application to add functionality to the statistics task.
*
* Arguments  : none
*********************************************************************************************************
*/
void OSTaskStatHook (void)
{
}

/*
*********************************************************************************************************
*                                           OSTCBInit() HOOK
*
* Description: This function is called by OSTCBInit() after setting up most of the TCB.
*
* Arguments  : ptcb    is a pointer to the TCB of the task being created.
*
* Note(s)    : 1) Interrupts may or may not be ENABLED during this call.
*********************************************************************************************************
*/
#if OS_VERSION > 203
void OSTCBInitHook (OS_TCB *ptcb)
{
    ptcb = ptcb;                                           /* Prevent Compiler warning                 */
}
#endif


/*
*********************************************************************************************************
*                                               TICK HOOK
*
* Description: This function is called every tick.
*
* Arguments  : none
*
* Note(s)    : 1) Interrupts may or may not be ENABLED during this call.
*********************************************************************************************************
*/
void OSTimeTickHook (void)
{
}


/*
*********************************************************************************************************
*                                             IDLE TASK HOOK
*
* Description: This function is called by the idle task.  This hook has been added to allow you to do  
*              such things as STOP the CPU to conserve power.
*
* Arguments  : none
*
* Note(s)    : 1) Interrupts are enabled during this call.
*********************************************************************************************************
*/
#if OS_VERSION >= 251
void OSTaskIdleHook (void)
{
}
#endif
#endif
/*********************************************************************************************************
**                            End Of File
********************************************************************************************************/


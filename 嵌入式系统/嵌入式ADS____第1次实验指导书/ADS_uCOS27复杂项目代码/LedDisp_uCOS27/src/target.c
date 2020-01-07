
/********************************************************************************************************/

#define IN_TARGET
#include "config.h"

/*********************************************************************************************************
** 函数名称: IRQ_Exception
** 功能描述: 中断异常处理程序，用户根据需要自己改变程序
********************************************************************************************************/
        void IRQ_Exception(void)
{
    while(1);                   // 这一句替换为自己的代码
}

/*********************************************************************************************************
** 函数名称: FIQ_Exception
** 功能描述: 快速中断异常处理程序，用户根据需要自己改变程序
********************************************************************************************************/
        void FIQ_Exception(void)
{
    while(1);
}

/*********************************************************************************************************
** 函数名称: Timer0
** 功能描述: 定时器0中断服务程序
********************************************************************************************************/
        void Timer0(void)
{
    T0IR = 0x01;
    T0MR0 += (Fpclk / OS_TICKS_PER_SEC);
    VICVectAddr = 0;            // 通知中断控制器中断结束
    OSTimeTick();
}


/*********************************************************************************************************
** 函数名称: TargetInit
** 功能描述: 目标板初始化代码，在需要的地方调用，根据需要改变
********************************************************************************************************/
        void TargetInit(void)
{
    extern void IRQ_Handler(void);
    extern void Timer0_Handler(void);
    
    OS_ENTER_CRITICAL();
  
 
    srand((uint32) TargetInit);

    VICIntEnClr = 0xffffffff;
    VICDefVectAddr = (uint32)IRQ_Handler;
    VICVectAddr0 = (uint32)Timer0_Handler;
    VICVectCntl0 = (0x20 | 0x04);
    VICIntEnable = 1 << 4;
    
    T0IR = 0xffffffff;
    T0TC = 0;
    T0TCR = 0x01;
    T0MCR = 0x01;
    T0MR0 = (Fpclk / OS_TICKS_PER_SEC);
    
    OS_EXIT_CRITICAL();
}

/*********************************************************************************************************
** 函数名称: InitialiseUART0
** 功能描述: 设置串口0 
********************************************************************************************************/

        void InitialiseUART0(uint32 bps)
{  	
    uint16 Fdiv;
    
    PINSEL0 = (PINSEL0 & 0xfffffff0) | 0x05;    /* 选择管脚为UART0 */

    U0LCR = 0x80;                               /* 允许访问分频因子寄存器 */
    Fdiv = (Fpclk / 16) / bps;                  /* 设置波特率 */
    U0DLM = Fdiv / 256;							
	U0DLL = Fdiv % 256;						
    U0LCR = 0x03;                               /* 禁止访问分频因子寄存器 */
                                                /* 且设置为8,1,n */
	U0IER = 0x00;                               /* 禁止中断 */
    U0FCR = 0x00;                               /* 初始化FIFO */
} 

/*********************************************************************************************************
** 函数名称: TargetResetInit
** 功能描述: 调用main函数前目标板初始化代码，根据需要改变，不能删除
********************************************************************************************************/
        void TargetResetInit(void)
{
    uint32 i;
    uint32 *cp1;
    uint32 *cp2;
    
    extern void Vectors(void);
    
/* 拷贝向量表，保证在flash和ram中程序均可正确运行 */    
    cp1 = (uint32 *)Vectors;
    cp2 = (uint32 *)0x40000000;
    for (i = 0; i < 2 * 8; i++)
    {
        *cp2++ = *cp1++;
    }
    
   
    MEMMAP = 0x2;                   //remap


    PINSEL0 = (PINSEL0 & 0xFFFF0000) | UART0_PCB_PINSEL_CFG | 0x50;

/* 设置系统各部分时钟 */
    PLLCON = 1;
#if (Fpclk / (Fcclk / 4)) == 1
    VPBDIV = 0;
#endif
#if (Fpclk / (Fcclk / 4)) == 2
    VPBDIV = 2;
#endif
#if (Fpclk / (Fcclk / 4)) == 4
    VPBDIV = 1;
#endif

#if (Fcco / Fcclk) == 2
    PLLCFG = ((Fcclk / Fosc) - 1) | (0 << 5);
#endif
#if (Fcco / Fcclk) == 4
    PLLCFG = ((Fcclk / Fosc) - 1) | (1 << 5);
#endif
#if (Fcco / Fcclk) == 8
    PLLCFG = ((Fcclk / Fosc) - 1) | (2 << 5);
#endif
#if (Fcco / Fcclk) == 16
    PLLCFG = ((Fcclk / Fosc) - 1) | (3 << 5);
#endif
    PLLFEED = 0xaa;
    PLLFEED = 0x55;
    while((PLLSTAT & (1 << 10)) == 0);
    PLLCON = 3;
    PLLFEED = 0xaa;
    PLLFEED = 0x55;

/* 设置存储器加速模块 */
    MAMCR = 2;
#if Fcclk < 20000000
    MAMTIM = 1;
#else
#if Fcclk < 40000000
    MAMTIM = 2;
#else
    MAMTIM = 3;
#endif
#endif

/* 设置串行口 */
    InitialiseUART0(115200);

/* 设置实时时钟 */
    CCR = 1;
    PREINT = Fpclk / 32768 - 1;
    PREFRAC = Fpclk - (Fpclk / 32768) * 32768;
    YEAR = 2003;
    MONTH = 6;
    DOM = 2;
    

    VICIntEnClr = 0xffffffff;
    VICVectAddr = 0;
    VICIntSelect = 0;
    T0IR = 0xffffffff;
    T0TCR = 0X02;
}
/*********************************************************************************************************
**                            End Of File
********************************************************************************************************/

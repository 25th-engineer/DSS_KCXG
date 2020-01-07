
/********************************************************************************************************/

#include "config.h"

#define  TASK_STK_SIZE                  64

OS_STK        Main_TaskStk[TASK_STK_SIZE];

void  LED_On (uint8 led)
{
    switch (led) {
        case 0:
             IO0CLR = 0x0000FFFF;
             break;

        case 1:
             IO0CLR = 1 << 0;
             break;

        case 2:
             IO0CLR = 1 << 1;
             break;

        case 3:
             IO0CLR = 1 << 2;
             break;

        case 4:
             IO0CLR = 1 << 3;
             break;

        case 5:
             IO0CLR = 1 << 4;
             break;

        case 6:
             IO0CLR = 1 << 5;
             break;

        case 7:
             IO0CLR = 1 << 6;
             break;

        case 8:
             IO0CLR = 1 << 7;
             break;

        case 9:
             IO0CLR = 1 << 8;
             break;

        case 10:
             IO0CLR = 1 << 9;
             break;

        case 11:
             IO0CLR = 1 << 10;
             break;

        case 12:
             IO0CLR = 1 << 11;
             break;

        case 13:
             IO0CLR = 1 << 12;
             break;

        case 14:
             IO0CLR = 1 << 13;
             break;

        case 15:
             IO0CLR = 1 << 14;
             break;

        case 16:
             IO0CLR = 1 << 15;
             break;
    }
}
void  LED_Off (uint8 led)
{
    switch (led) {
        case 0:
             IO0SET = 0x0000FFFF;
             break;

        case 1:
             IO0SET = 1 << 0;
             break;

        case 2:
             IO0SET = 1 << 1;
             break;

        case 3:
             IO0SET = 1 << 2;
             break;

        case 4:
             IO0SET = 1 << 3;
             break;

        case 5:
             IO0SET = 1 << 4;
             break;

        case 6:
             IO0SET = 1 << 5;
             break;

        case 7:
             IO0SET = 1 << 6;
             break;

        case 8:
             IO0SET = 1 << 7;
             break;

        case 9:
             IO0SET = 1 << 8;
             break;

        case 10:
             IO0SET = 1 << 9;
             break;

        case 11:
             IO0SET = 1 << 10;
             break;

        case 12:
             IO0SET = 1 << 11;
             break;

        case 13:
             IO0SET = 1 << 12;
             break;

        case 14:
             IO0SET = 1 << 13;
             break;

        case 15:
             IO0SET = 1 << 14;
             break;

        case 16:
             IO0SET = 1 << 15;
             break;
    }
}

void  Main_Task(void *data);

/*********************************************************************************************************
** 函数名称: main
** 功能描述: c语言的主函数，由它启动多任务环境
********************************************************************************************************/
        int main (void)
{
    OSInit();

    OSTaskCreate(Main_Task, (void *)0, &Main_TaskStk[TASK_STK_SIZE - 1], 0);
    OSStart();
    return 0;
}
void  LED_Init (void)
{
    PINSEL0= 0;                      /* Use GPIO pins P0.0 to P0.15 for LEDs              */
    IO0DIR|= 0x0000FFFF;             /* Set GPIO pins as outputs                          */

    LED_Off(0);                                   /* Turn OFF all the LEDs                             */

}
/*********************************************************************************************************
** 函数名称: Main_Task
** 功能描述: μCOS-II的第一个任务，通常由它初始化目标板和建立其它任务
********************************************************************************************************/
  void  Main_Task(void *p_arg)
{   int i;
    p_arg = p_arg;                                                  /* 避免编译警告 */
    TargetInit();
    LED_Init();                                                  /* 目标板初始化 */
                                 
     while (TRUE) {                         /* Task body, always written as an infinite loop.          */
        for (i = 1; i <= 16; i++) {
            LED_On(i);
            OSTimeDlyHMSM(0, 0, 0, 50);
            LED_Off(i);
        }
        for (i = 1; i <= 16; i++) {
            LED_On(17 - i);
            OSTimeDlyHMSM(0, 0, 0, 50);
            LED_Off(17 - i);
        }
    }  
    
} 

/*********************************************************************************************************
**                            End Of File
********************************************************************************************************/

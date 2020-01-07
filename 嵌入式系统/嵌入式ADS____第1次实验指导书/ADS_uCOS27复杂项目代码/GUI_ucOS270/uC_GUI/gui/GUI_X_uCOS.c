/*
*********************************************************************************************************
*                                                uC/GUI
*                        Universal graphic software for embedded applications
*
*                       (c) Copyright 2002, Micrium Inc., Weston, FL
*                       (c) Copyright 2002, SEGGER Microcontroller Systeme GmbH
*
*              礐/GUI is protected by international copyright laws. Knowledge of the
*              source code may not be used to write a similar product. This file may
*              only be used in accordance with a license and should not be redistributed
*              in any way. We appreciate your understanding and fairness.
*
----------------------------------------------------------------------
File        : GUI_X.C
Purpose     : COnfig / System dependent externals for GUI
---------------------------END-OF-HEADER------------------------------
*/
#include <stdio.h>
#include "ucos_ii.h"
#include "gui_x.h"
#include "includes.h"


/*********************************************************************
*
*      Timing:
*                 GUI_X_GetTime
*                 GUI_X_Delay

  Some timing dependent routines of emWin require a GetTime
  and delay funtion. Default time unit (tick), normally is
  1 ms.
*/

int GUI_X_GetTime(void) {
  return OSTimeGet();
}

void GUI_X_Delay(int Period) {
  OSTimeDly(Period);                                    // GUI 程序对延时时间要求并不高,所以直接使用参数.
}

/*********************************************************************
*
*       GUI_X_ExecIdle()
*/
void GUI_X_ExecIdle(void) {
  OSTimeDly(1);
}

/*********************************************************************
*
*      Multitasking:
*
*                 GUI_X_InitOS()
*                 GUI_X_GetTaskId()
*                 GUI_X_Lock()
*                 GUI_X_Unlock()

Note:
  The following routines are required only if emWin is used in a
  true multi task environment, which means you have more than one
  thread using the emWin API.
  In this case the
                      #define GUI_OS 1
  needs to be in GUIConf.h
*/

static OS_EVENT * DispSem;

int  GUI_X_GetTaskId (void)
{
    return ((int)(OSTCBCur->OSTCBPrio));
}

void GUI_X_InitOS (void)
{
    DispSem = OSSemCreate(1);
}

void GUI_X_Unlock (void)
{
    OSSemPost(DispSem);
}

void GUI_X_Lock (void) {
 INT8U err;

    OSSemPend(DispSem, 0, &err);
}

/*********************************************************************
*
*       GUI_X_Init()
*
* Note:
*     GUI_X_Init() is called from GUI_Init is a possibility to init
*     some hardware which needs to be up and running before the GUI.
*     If not required, leave this routine blank.
*/

void GUI_X_Init(void)
{
    GUI_Key_Init();                 // 初始化一个按键信号量.
    Init_Ts();				//此处接入触摸屏的中断设置
    GUI_TOUCH_SetDefaultCalibration();//用默认值校准触摸屏
   
}

//extern void Uart0_Print (const char *format, ...);

#define Uart0Print printf

void GUI_X_Log(const char *s)
{
 //static INT32U i;

   // Uart0Print (s);
   // Uart0Print ("\r");
   //Uart0Print ("------------------------ %i .\n\r",i++);
}

void GUI_X_Warn(const char *s)
{
}

void GUI_X_ErrorOut(const char *s)
{
}

/*
********************************************************************************************************
* End.
********************************************************************************************************
*/

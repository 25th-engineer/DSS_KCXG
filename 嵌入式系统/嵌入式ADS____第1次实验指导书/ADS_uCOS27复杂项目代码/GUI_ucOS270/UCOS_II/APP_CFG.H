/*
*********************************************************************************************************
*                                                uC/OS-II
*                                          The Real-Time Kernel
*
*                           (c) Copyright 1992-2006, Jean J. Labrosse, Weston, FL
*                                           All Rights Reserved
*
*                                  uC/OS-II Configuration File for V2.8x
*
* File    : APP_CFG.H
* By      : Jean J. Labrosse
* Version : V2.83
*********************************************************************************************************
*/

#ifndef APP_CFG_H
#define APP_CFG_H

/* 
********************************************************************************************************* 
* TASK PRIORITIES 
********************************************************************************************************* 
*/
#define OS_TASK_TMR_PRIO          63  /* This defines the priority of OS_TMR_Task() */ 
#define APP_TASK_START_PRIO       64  
#define APP_TASK_USER_IF_PRIO     65  
#define APP_TASK_KBD_PRIO         66  
#define CLK_TASK_PRIO             67  
#define OS_VIEW_TASK_PRIO         68  
#define OS_VIEW_TASK_ID           68  
/* 
********************************************************************************************************* 
* TASK STACK SIZES 
********************************************************************************************************* 
*/ 
#define APP_TASK_START_STK_SIZE           512 
#define APP_TASK_USER_IF_STK_SIZE         100 
#define APP_TASK_KBD_STK_SIZE             100 
#define CLK_TASK_STK_SIZE                512 /* Stack size in OS_STK for ClkTask_OS() */ 
#define OS_VIEW_TASK_STK_SIZE            100 
/* 
********************************************************************************************************* 
* uC/Clk CONFIGURATION 
********************************************************************************************************* 
*/ 
#define CLK_TS_BASE_YEAR       2005 /* Time stamps start year */ 
#define CLK_DLY_TICKS    OS_TICKS_PER_SEC /* # of clock ticks to obtain 1 second */ 
#define CLK_DATE_EN             1 /* Enable DATE (when 1) */ 
#define CLK_TS_EN               1 /* Enable TIME-STAMPS (when 1) */ 
#define CLK_USE_DLY             1 /* Task will use OSTimeDly() instead of pend on sem. */ 
/* 
********************************************************************************************************* 
* uC/LCD CONFIGURATION 
********************************************************************************************************* 
*/ 
#define DISP_BUS_WIDTH          4 /* LCD controller is accessed with a 4 bit bus */ 
/* 
********************************************************************************************************* 
* uC/OS-View CONFIGURATION 
********************************************************************************************************* 
*/ 
#define OS_VIEW_PARSE_TASK      1 /* Parsing of received packets will be done by a task */ 
#define OS_VIEW_TMR_32_BITS     1 /* uC/OS-View timer is 32 bits */ 
#define OS_VIEW_UART_0          0 
#define OS_VIEW_UART_1          1 
#define OS_VIEW_UART_2          2 
#define OS_VIEW_COMM_SEL    OS_VIEW_UART_1 

#endif
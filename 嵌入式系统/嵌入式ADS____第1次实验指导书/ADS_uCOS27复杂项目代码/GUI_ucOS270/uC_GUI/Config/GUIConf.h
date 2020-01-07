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
File        : GUICONF.H
Purpose     : Configures emWins abilities, fonts etc.
----------------------------------------------------------------------
*/
#ifndef __GUICONF_H
#define __GUICONF_H

/*
**********************************************************************
*         Configuration of desired functionality
**********************************************************************
*/

#define GUI_OS                    (1)   /* Compile with multitasking support */
#define GUI_WINSUPPORT            (1)   /* Use window manager if true (1)  */
#define GUI_SUPPORT_MEMDEV        (1)   /* Support memory devices */
#define GUI_SUPPORT_TOUCH         (1)   /* Support a touch screen (req. win-manager) */
#define GUI_SUPPORT_UNICODE       (1)   /* Support mixed ASCII/UNICODE strings */
#define GUI_SUPPORT_AA            1     /* Anti aliasing available */
#define GUI_DEBUG_LEVEL GUI_DEBUG_LEVEL_LOG_ALL

#define GUI_MAXTASK               (20)  // 多任务数 (每个任务都有单独的RAM需求,所以应根据需求设置).


/*********************************************************************
*         Configuration of dynamic memory
**********************************************************************
Dynamic memory is used for memory devices and window manager.
If you do not use these features, there is no need for dynamic memory
and it may be switched off completely. (This section can be erased)
*/

#define GUI_ALLOC_SIZE          92500   /* Size of dynamic memory */

/*
****************************************
* Configuration of available fonts     *
****************************************
*/

#define GUI_DEFAULT_FONT    &GUI_Font8x16   // 缺省字体.

#endif /* GUICONF_H */

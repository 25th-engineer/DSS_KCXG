/*
*********************************************************************************************************
*                                                uC/GUI
*                        Universal graphic software for embedded applications
*
*                       (c) Copyright 2002, Micrium Inc., Weston, FL
*                       (c) Copyright 2002, SEGGER Microcontroller Systeme GmbH
*
*              µC/GUI is protected by international copyright laws. Knowledge of the
*              source code may not be used to write a similar product. This file may
*              only be used in accordance with a license and should not be redistributed
*              in any way. We appreciate your understanding and fairness.
*
----------------------------------------------------------------------
File        : GUITouch.Conf.h
Purpose     : Configures emWin GSC touch screen module
----------------------------------------------------------------------
*/

#ifndef __GUITOUCH_CONF_H
#define __GUITOUCH_CONF_H

#define GUI_TOUCH_SWAP_XY       1
#define GUI_TOUCH_MIRROR_X      0
#define GUI_TOUCH_MIRROR_Y      0

#define GUI_TOUCH_AD_LEFT       100	//minX
#define GUI_TOUCH_AD_RIGHT      908	//maxX
#define GUI_TOUCH_AD_BOTTOM     115	//minY
#define GUI_TOUCH_AD_TOP        910	//maxY


#endif /* GUITOUCH_CONF_H */

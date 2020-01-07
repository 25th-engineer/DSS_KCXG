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
File        : GUI_HID.C
Purpose     : HID (Human interface device) management
----------------------------------------------------------------------
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "LCD_Private.H"      /* private modul definitions & config */
#include "GUI_Protected.h"

/* Generate code only if configuration says so ! */

#include "GUITouchconf.h"    /* Located in GUIx, will include GUITouch.conf.h */

/*********************************************************************
*
*       Static data
*
**********************************************************************
*/

static GUI_HID_STATE _State;

/*********************************************************************
*
*        Support for dynamic linkage
*
**********************************************************************
*/

void GUI_HID_Load(void) {
  #if (GUI_WINSUPPORT & (GUI_SUPPORT_TOUCH | GUI_SUPPORT_MOUSE))
    WM_pfHandleHID = WM_HandleHID;
  #endif
}


/*********************************************************************
*
*              GUI_HID_GetState
*/

int GUI_HID_GetState(GUI_HID_STATE *pState) {
  *pState = _State;
  return (pState->Pressed != 0) ? 1 : 0;
}

/*********************************************************************
*
*              GUI_HID_StoreState
*/
void GUI_HID_StoreState(const GUI_HID_STATE *pState) {
  GUI_HID_Load();
  _State = *pState;
}


/*********************************************************************
*
*          GUI_HID_Init
*/
void GUI_HID_Init(void) {}




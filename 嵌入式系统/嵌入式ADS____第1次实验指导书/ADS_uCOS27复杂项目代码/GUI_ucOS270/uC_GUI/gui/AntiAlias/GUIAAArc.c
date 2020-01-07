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
File        : GUIArcAA.C
Purpose     : Draw Arc routines with Antialiasing
----------------------------------------------------------------------
Version-Date---Author-Explanation
----------------------------------------------------------------------
1.00.00 000728 RS     First release of the new algorithm
----------------------------------------------------------------------
Known problems or limitations with current version
----------------------------------------------------------------------
None.
----------------------------------------------------------------------
Open issues
----------------------------------------------------------------------
None
---------------------------END-OF-HEADER------------------------------
*/

#include "GUI_Protected.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>


static void _DrawArcAA (int x0, int y0, int rx, int ry, int a0, int a1) {
  int PenSizePrev = GUI_GetPenSize();
  GUI_AA_Init(x0-rx-PenSizePrev, x0+rx+PenSizePrev);
  GUI_SetPenSize(PenSizePrev * GUI_Context.AA_Factor);
  GL_DrawArc(GUI_Context.AA_Factor*x0, GUI_Context.AA_Factor*y0, GUI_Context.AA_Factor*rx, GUI_Context.AA_Factor*ry,a0,a1);
  GUI_SetPenSize(PenSizePrev);
  /*
  // Cleanup
  */

  GUI_AA_Exit();
}


void GUI_AA_DrawArc(int x0, int y0, int rx, int ry, int a0, int a1) {
  #if (GUI_WINSUPPORT)
    WM_LOCK();
    WM_ADDORG(x0,y0);
    WM_ITERATE_START(NULL) {
  #endif
  _DrawArcAA( x0, y0, rx, ry, a0, a1);
  #if (GUI_WINSUPPORT)
    } WM_ITERATE_END();
    WM_UNLOCK();
  #endif
}









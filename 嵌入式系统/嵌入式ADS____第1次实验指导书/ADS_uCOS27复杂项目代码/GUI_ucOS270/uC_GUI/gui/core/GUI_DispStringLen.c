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
File        : GUI_DispStringLen.c
Purpose     : Implementation of optional routines
---------------------------END-OF-HEADER------------------------------
*/


#include "GUI_Protected.H"


void GUI_DispStringLen(const char GUI_FAR *s, int Len) {
  if (s) {
    U16 c;
    while (Len && ((c=(U16)(*s++))!=0)) {
      GUI_DispChar(c);
      Len--;
    }
  }
  while (Len--) {
    GUI_DispChar(' ');
  }
}





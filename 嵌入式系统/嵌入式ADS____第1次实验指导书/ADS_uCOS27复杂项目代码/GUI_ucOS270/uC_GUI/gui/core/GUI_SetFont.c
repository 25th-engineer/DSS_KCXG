/*********************************************************************
*                SEGGER MICROCONTROLLER SYSTEME GmbH                 *
*        Solutions for real time microcontroller applications        *
**********************************************************************
*                                                                    *
*        (c) 2002         SEGGER Microcontroller Systeme GmbH        *
*                                                                    *
*        Internet: www.segger.com    Support:  support@segger.com    *
*                                                                    *
**********************************************************************

**** emWin/GSC Grafical user interface for embedded applications ****
emWin is protected by international copyright laws. Knowledge of the
source code may not be used to write a similar product. This file may
only be used in accordance with a license and should not be re-
distributed in any way. We appreciate your understanding and fairness.
----------------------------------------------------------------------
File        : GUI_SetFont.C
Purpose     : Optional routines
----------------------------------------------------------------------
*/

#include "GUI_Protected.H"
#include "GUIDebug.h"

/*********************************************************************
*
*       Public routines
*
**********************************************************************
*/


/***********************************************************
*
*     GUI_SetFont

*/
const GUI_FONT* GUI_SetFont(const GUI_FONT* pNewFont) {
  const GUI_FONT * pOldFont = GUI_Context.pAFont;
  GUI_LOCK();
  if (pNewFont)
    GUI_Context.pAFont = pNewFont;
  GUI_UNLOCK();
  return pOldFont;
}





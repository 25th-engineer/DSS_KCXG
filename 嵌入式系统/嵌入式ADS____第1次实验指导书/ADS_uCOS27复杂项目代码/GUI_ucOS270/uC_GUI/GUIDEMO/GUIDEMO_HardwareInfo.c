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
File        : GUIDEMO_HardwareInfo.c
Purpose     : Displays hardware info
----------------------------------------------------------------------
*/

#include "GUI.H"
#include "GUIDEMO.H"
//#include "LCD_ConfDefaults.h"      /* valid LCD configuration */

/*********************************************************************
*
*       Static functions
*
**********************************************************************
*/

#if (LCD_CONTROLLER == 1375) | (LCD_CONTROLLER == 13705) | (LCD_CONTROLLER == 1386) | (LCD_CONTROLLER == 13806)
static void _Show(const char* s0, const char* s1) {
  int XSize = LCD_GetXSize();
  GUI_SetColor(GUI_WHITE);
  GUI_SetFont(&GUI_FontComic18B_ASCII);
  GUI_DispString("\n");
  GUI_DispString(s0);
  GUIDEMO_Delay(500);
  GUI_GotoX(120);
  GUI_SetColor(GUI_GREEN);
  if (LCD_GetXSize() < 320) {
    GUI_DispString("\n");
    GUI_GotoX(XSize / 4);
  }
  GUI_DispString(s1);
  GUIDEMO_Delay(1000);
}
#endif

/*********************************************************************
*
*       GUIDEMO_HardwareInfo
*
**********************************************************************
*/

void GUIDEMO_HardwareInfo(void) {
  #if (LCD_CONTROLLER == 1375) | (LCD_CONTROLLER == 13705) | (LCD_CONTROLLER == 1386) | (LCD_CONTROLLER == 13806)
    GUIDEMO_ShowIntro("Hardware info",   "" );
    GUI_Clear();
    GUI_SetFont(&GUI_FontComic24B_ASCII);
    GUI_SetColor(GUI_WHITE);
    #if (LCD_CONTROLLER == 1375) | (LCD_CONTROLLER == 13705)
      GUI_DispStringAt("S3C2410 LCD CON\n", 10, 65);
      _Show("Max. colors",  "64K (16 bit per color)");
      _Show("Display types", "TFT LCD");
      _Show("LUT",           "256 index colors");
    #elif (LCD_CONTROLLER == 1386) | (LCD_CONTROLLER == 13806)
      GUI_DispStringAt("S3C2410 LCD CON\n", 10, 65);
      _Show("Max. colors",  "64K (16 bit per color)");
      _Show("Display types", "TFT LCD");
      _Show("LUT",           "256 index colors");
    #endif
    GUIDEMO_Delay(1500);
  #endif
}

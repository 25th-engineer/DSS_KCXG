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
File        : GUIAALib.C
Purpose     : Antialiasing library
---------------------------END-OF-HEADER------------------------------
*/

#include "GUI_Private.H"
#include "LCD_ConfDefaults.h"            /* Required in order to know max. XSize so we do not waste memory */

#include <stdio.h>
#include <string.h>
#include <math.h>


#ifdef WIN32
  #pragma warning( disable : 4244 )  // Disable warning messages in simulation
  #pragma warning( disable : 4761 )  // Disable warning "integral size mismatch in argument; conversion supplied"
#endif                                      


/*
  *******************************************************************
  *
  *              Config defaults ...
  *
  *******************************************************************
*/
#ifndef GUI_AA_LINEBUFFER_SIZE
  #define GUI_AA_LINEBUFFER_SIZE LCD_XSIZE
#endif

/*
  *******************************************************************
  *
  *              Statics
  *
  *******************************************************************
*/

static U8   abAABuffer[GUI_AA_LINEBUFFER_SIZE];   /* This could be changed to dynamic memory ... */
static U8*  pabAABuffer;
static int  AA_x0, AA_x1, AA_y, AA_x0_InUse, AA_x1_InUse;
static GUI_RECT ClipRect_HL;

static tLCD_HL_APIList           DrawAPICopy;    /* Copy of device function ptr list */
static const tLCD_HL_APIList*    pLCD_HLPrev;    /* Copy of device function ptr list */


/*
          **********************************************
          *                                            *
          *              AA Clean Line Buffer          *
          *                                            *
          **********************************************

*/

static void AA_CleanLine(void) {
  memset(pabAABuffer,0, AA_x1 - AA_x0+1);
  AA_y = -16383;  /* Invalidate */  
  AA_x0_InUse =  16383;
  AA_x1_InUse = -16383;
}

/*
          **********************************************
          *                                            *
          *              AA Flush Line Buffer          *
          *                                            *
          **********************************************

*/

static void AA_FlushLine(void) {
  int i;
  int iEnd = AA_x1_InUse-AA_x0;
  int IMax = GUI_Context.AA_Factor*GUI_Context.AA_Factor;
  for (i =AA_x0_InUse-AA_x0; i<=iEnd; i++) {
    int Intens = *(pabAABuffer+i);
    if (Intens) {
      /* Check we can use line draw */
      if (Intens == IMax) {
        int j;
        for (j=i; j<iEnd; ) {
          if (*(pabAABuffer+j+1) != IMax) {
            break;
          }
          j++;
        }
        /* Draw the full pixel(s) */
        if (j!=i) {
          pLCD_HLPrev->pfDrawHLine(AA_x0+i, AA_y, AA_x0+j);
          i = j;  /*xxx*/
        } else {
          LCD_HL_DrawPixel (AA_x0+i,AA_y);
        }
      } else {
        LCD_SetPixelAA(AA_x0+i,AA_y, (15*Intens+IMax/2)/IMax);
      }
    }    
  }
  AA_CleanLine();
}


/*
          **********************************************
          *                                            *
          *              Local DrawHLine               *
          *                                            *
          **********************************************

  This is the redirected DrawHLine routine which is called
  instead of the default output routine. Its job is to do
  antialiasing and then perform the drawing operations.
*/

static void AA_DrawHLine  (int x0, int y,  int x1) {
  int x0Real, x1Real;
/* Make sure there is something to do */
  if (x1<x0)
    return;
/* Flush line if we are in an other pixel (real) line */
  if (y/GUI_Context.AA_Factor != AA_y) {
    AA_FlushLine();
    AA_y = y/GUI_Context.AA_Factor;
  }
  x0Real = x0/GUI_Context.AA_Factor;
  x1Real = x1/GUI_Context.AA_Factor;
/* Handle used area (speed optimization for drawing) */
  if (x0Real < AA_x0_InUse)
    AA_x0_InUse = x0Real;
  if (x1Real > AA_x1_InUse)
    AA_x1_InUse = x1Real;
/* Clip (should not be necessary ... Just to be on the safe side ! */
  if (x0Real < AA_x0) {
    x0 = AA_x0*GUI_Context.AA_Factor;
  }
  if (x1Real > AA_x1) {
    x1 = (AA_x1+1)*GUI_Context.AA_Factor-1;
  }
/* Make sure there is still something to do (after clipping) */
  if (x1<x0)
    return;
/* Inc. hit counters in buffer */
  {
    int x0_Off = x0/GUI_Context.AA_Factor-AA_x0;
    int x1_Off = x1/GUI_Context.AA_Factor-AA_x0;
    int iRem = x1_Off-x0_Off+1;
    U8 *pDest = pabAABuffer+x0_Off;
    if (iRem ==1) {
      *(pDest) += x1-x0+1;
    } else {
      /* First Pixel */
      *pDest++ += ((x0_Off+AA_x0+1)*GUI_Context.AA_Factor-x0);
      /* Middle Pixels */
      for (;--iRem>1; ) {
        *pDest++ +=GUI_Context.AA_Factor;
      }
      /* Last Pixel */
      *pDest += (1+x1- (x1_Off+AA_x0) *GUI_Context.AA_Factor);
    }
  }
}

/*
  *******************************************************************
  *
  *              CalcClipRectHL
  *
  *******************************************************************
*/
static void CalcClipRectHL(void) {
  ClipRect_HL.x0 =  GUI_Context.ClipRect.x0    * GUI_Context.AA_Factor;
  ClipRect_HL.y0 =  GUI_Context.ClipRect.y0    * GUI_Context.AA_Factor;
  ClipRect_HL.x1 = (GUI_Context.ClipRect.x1+1) * GUI_Context.AA_Factor -1;
  ClipRect_HL.y1 = (GUI_Context.ClipRect.y1+1) * GUI_Context.AA_Factor -1;
}

/******************************************************************
*
*              GUI_AA_Init
*
*******************************************************************
*/

int GUI_AA_Init(int x0, int x1) {
  int r =0;
  /* Bounds checking:
     Make sure x0, x1 are in legal range ...
     (The important point is that they span no more than configured as
      buffer size)
  */
  if (x0<0)
    x0 =0;
  if (x1-x0 > GUI_AA_LINEBUFFER_SIZE-1)
    x1 = x0+GUI_AA_LINEBUFFER_SIZE-1;
  /* Is there anything to do at all ??? */
  if (x1 < x0) {
    x1 = x0;   /* Not really ... */
    r =1;
  }
  DrawAPICopy = *GUI_Context.pLCD_HL; /* Copy API table */
  pLCD_HLPrev = GUI_Context.pLCD_HL; /* Remember list ptr (for restore) */
  DrawAPICopy.pfDrawHLine = AA_DrawHLine;  /* modify function ptr. for hline */
  GUI_Context.pLCD_HL = &DrawAPICopy;      /* Use copy of fp-list */
  pabAABuffer = abAABuffer;
  AA_x0 = x0;
  AA_x1 = x1;
  AA_CleanLine();
  CalcClipRectHL();
  GUI_Context.pClipRect_HL = &ClipRect_HL;
  return r;
}

/******************************************************************
*
*              GUI_AA_Init
*
*******************************************************************
*/

int GUI_AA_Init_HiRes(int x0, int x1) {
  x0 /= GUI_Context.AA_Factor;
  x1 /= GUI_Context.AA_Factor;
  return GUI_AA_Init(x0, x1);
}

/*********************************************************************
*
*              AA Selection of Factors for position and quality
*
**********************************************************************

*/


void GUI_AA_SetFactor(int Factor) {
  GUI_Context.AA_Factor = Factor;
  CalcClipRectHL();      /* High level clipping depends on quality factor */
}

int GUI_AA_GetFactor(void) {
  return GUI_Context.AA_Factor;
}

void GUI_AA_DisableHiRes(void) {
  GUI_Context.AA_HiResEnable = 0;
}

void GUI_AA_EnableHiRes(void) {
  GUI_Context.AA_HiResEnable =1;
}

I16 GUI_AA_HiRes2Pixel(int HiRes) {
  return GUI_Context.AA_Factor ? (HiRes / GUI_Context.AA_Factor) : HiRes;
}

/*
  *******************************************************************
  *
  *              GUI_AA_Exit
  *
  *******************************************************************
*/
void GUI_AA_Exit(void) {
  AA_FlushLine();
  /* restore previous settings */
  GUI_Context.pLCD_HL = pLCD_HLPrev;
  GUI_Context.pClipRect_HL = &GUI_Context.ClipRect;
}

/*********************** EOF FILE *******************************/







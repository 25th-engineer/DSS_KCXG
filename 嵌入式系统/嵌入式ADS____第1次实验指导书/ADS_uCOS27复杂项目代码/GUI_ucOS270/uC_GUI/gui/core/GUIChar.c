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
File        : GUIChar.C
Purpose     : Implementation of character and string services
---------------------------END-OF-HEADER------------------------------
*/


#include <stddef.h>           /* needed for definition of NULL */
#include <stdio.h>
#include <string.h>
#include "GUI_Private.H"
 

/**********************************************************
*
*        Public code
*
***********************************************************
*/


/***********************************************************
*
*       GUI_DispChar
*/

void GL_DispChar(U16 c) {
  /* check for control characters */
  if (c == '\n') {
    GUI_DispNextLine();
  } else {
    if (c != '\r') {
      GUI_Context.pAFont->pfDispChar(c);
    }
  }
}


/*************** GUI_GetStringDistX ****************************
This routine is used to calculate the length of a string in pixels.
*/

int GUI_GetLineDistX(const char GUI_FAR *s, int Len) {
  int Dist =0;
  if (s) {
    if (GUI_Context.pAFont->pafEncode) {
      return GUI_Context.pAFont->pafEncode->pfGetLineDistX(s, Len);
    }
#if (GUI_SUPPORT_UNICODE)
    {
      U8 c0;
      char UCActive=0;
      while (((c0=*(U8*)s) !=0) && Len >0) {
        s++; Len--;
        if (UCActive) {
          if (c0 == GUI_UC_ENDCHAR)
            UCActive = 0;
          else {
            U8  c1 = *(U8*)s++;
            Len--;
            Dist += GUI_GetCharDistX(GUI_DB2UC(c0, c1));
          }
        } else { /* Unicode not active */
          if (c0 == GUI_UC_STARTCHAR)
            UCActive = 1;
          else
            Dist += GUI_GetCharDistX(c0);
        }
      }
    }
#else
    while (--Len>=0) {
      Dist += GUI_GetCharDistX(*(U8*)s++);
    }
#endif
  }
  return Dist;
}

/*************** GUI_GetLineLen ****************************
Returns the number of characters in a string
Note: The return value can be used as offset into the
string, which means that double characters count double
*/

int GUI__GetLineLen(const char GUI_FAR *s, int MaxLen) {
  int Len =0;
  if (!s)
    return 0;
  if (GUI_Context.pAFont->pafEncode) {
    return GUI_Context.pAFont->pafEncode->pfGetLineLen(s, MaxLen);
  }
#if (GUI_SUPPORT_UNICODE)
  {
    U8 c0;
    char UCActive=0;
    while (((c0=*(U8*)s) !=0) && Len < MaxLen) {
      s++;
      if (UCActive) {
        switch (c0) {
        case GUI_UC_ENDCHAR: UCActive = 0; break;
        default: Len++; s++;
        }
      } else { /* Unicode not active */
        switch (c0) {
        case GUI_UC_STARTCHAR: UCActive = 1; break;
        case '\n': return Len;
        case '\r': return Len;
        }
      }
      Len++;
    }
  }
#else
  for (;Len<MaxLen; Len++) {
    U8 Data = *(U8*)s++;
    if ((Data == 0) || (Data == '\n'))
      break;
  }
#endif
  return Len;
}


/**********************************************************************
*
*       "GET"   routines    (information retrieval)
*
***********************************************************************

These routines all return a value like selected font, current display
position in x/y direction, window size in x/y direction,
font size and matrix in x/y.
The routines prefixed with GUI_ can be called from the application
program or emWin internally, while the routines without that prefix
are not supposed to be called from outside emWin.
The main reason is that GUI_LOCK has to be called before these
values can be reliably retrieved in a multitasking environment.

*/

/*------------------------------------------------------------------
  GUI_GetYAdjust  --- returns adjustment in vertical (Y) direction
                  The return value needs to be subtracted from
                  the y-position of the character
*/

int GUI_GetYAdjust(void) {
  switch (GUI_Context.TextAlign&GUI_TA_VERTICAL) {
	case GUI_TA_BOTTOM:
		return GUI_Context.pAFont->YSize-1;
	case GUI_TA_VCENTER:
		return GUI_Context.pAFont->YSize/2;
	case GUI_TA_BASELINE:
		return GUI_Context.pAFont->YSize/2;
	}
  return 0;
}



/*
        *******************************************
        *                                         *
        *        Get Font Spacing routines        *
        *                                         *
        *******************************************
*/

int GUI_GetFontDistY(void) {
  int r;
  GUI_LOCK();
  r = GUI_Context.pAFont->YDist * GUI_Context.pAFont->YMag;
  GUI_UNLOCK();
  return r;
}


/*
        *******************************************
        *                                         *
        *        Get Char spacing routines        *
        *                                         *
        *******************************************
*/

int GUI_GetCharDistX(U16 c) {
  int r;
  GUI_LOCK();
  r = GUI_Context.pAFont->pfGetCharDistX(c);
  GUI_UNLOCK();
  return r;
}








/*
      *********************************
      *                               *
      *       Linefeed                *
      *                               *
      *********************************

*/
void GUI_DispNextLine(void) {
  GUI_Context.DispPosY +=GUI_GetFontDistY();
  GUI_Context.DispPosX = GUI_Context.LBorder;
}



/*
        ************************************************************
        *                                                          *
        *             Set the write position                       *
        *                                                          *
        ************************************************************

Sets the write position. The routines routine 1 if it is clear that
the current write position is in an area outside the current window
and will therefor not be written.

*/

static char _GotoY(int y) {
  GUI_Context.DispPosY = y;
  return 0;
}

static char _GotoX(int x) {
  GUI_Context.DispPosX = x;
  return 0;
}


char GUI_GotoY(int y) {
  char r;
  GUI_LOCK();
  r = _GotoY(y);
  GUI_UNLOCK();
  return r;
}


char GUI_GotoX(int x) {
  char r;
  GUI_LOCK();
  r = _GotoX(x);
  GUI_UNLOCK();
  return r;
}

char GUI_GotoXY(int x, int y) {
  char r;
  GUI_LOCK();
  r  = GUI_GotoX(x);
  r |= GUI_GotoY(y);
  GUI_UNLOCK();
  return r;
}






/*
********************************************************
*
*                Display line
*
********************************************************
*/

void GL_DispLine(const char GUI_FAR *s, int Len, const GUI_RECT *pRect) {
  /*
    Check if we have anything to do at all ...
    If the window manager has already set the clipping rect, it does not
    make sense to due this. So it makes sense only if
    a) The window manager is not used (-> Configuration)
      or
    b) The window manager is inactive (-> Memory device active)
  */
  if (GUI_Context.pClipRect_HL) {
    if (GUI_RectsIntersect(GUI_Context.pClipRect_HL, pRect) == 0)
      return;
  }
  if (GUI_Context.pAFont->pafEncode) {
    GUI_Context.pAFont->pafEncode->pfDispLine(s, Len);
    return;
  }
#if (GUI_SUPPORT_UNICODE)
  {
    U8 c0;
    char UCActive=0;
    while (--Len >=0) {
      c0=*(U8*)s++;
      if (UCActive) {
        if (c0 == GUI_UC_ENDCHAR)
          UCActive = 0;
        else {
          U8  c1 = *(U8*)s++;
          Len--;
          GL_DispChar (GUI_DB2UC(c0, c1));
        }
      } else { /* Unicode not active */
        if (c0 == GUI_UC_STARTCHAR)
          UCActive = 1;
        else
          GL_DispChar(c0);
      }
    }
  }
#else
  {
    while (--Len >=0) {
      GL_DispChar(*(U8*)s++);
    }
  }
#endif
}

void GUI__DispLine(const char GUI_FAR *s, int Len, const GUI_RECT* pr) {
  GUI_RECT r;
  r = *pr;
  #if GUI_WINSUPPORT
  WM_ADDORG(r.x0,r.y0);
  WM_ADDORG(r.x1,r.y1);
  WM_ITERATE_START(&r) {
  #endif
     GUI_Context.DispPosX = r.x0;
     GUI_Context.DispPosY = r.y0;
  /* Do the actual drawing via routine call. */
     GL_DispLine(s, Len, &r);
  #if GUI_WINSUPPORT
  } WM_ITERATE_END();
  WM_SUBORG(GUI_Context.DispPosX, GUI_Context.DispPosY);
  #endif
}









/********************************************************************
*
*                Display String
*
*********************************************************************
*/

void GUI_DispString(const char GUI_FAR *s) {
  int xAdjust, yAdjust, xOrg;
  int FontSizeY;
  if (!s)
    return;
  GUI_LOCK();
  FontSizeY = GUI_Context.pAFont->YDist;
  xOrg = GUI_Context.DispPosX;
 /* Adjust vertical position */
  yAdjust = GUI_GetYAdjust();
  GUI_Context.DispPosY -= yAdjust;
  for (; *s; s++) {
    GUI_RECT r;
    int LineLen= GUI__GetLineLen(s,0x7fff);
    int xLineSize = GUI_GetLineDistX(s, LineLen);
  /* Check if x-position needs to be changed due to h-alignment */
    switch (GUI_Context.TextAlign & GUI_TA_HORIZONTAL) { 
    case GUI_TA_CENTER: xAdjust= xLineSize/2; break;
    case GUI_TA_RIGHT:  xAdjust= xLineSize; break;
    default:            xAdjust= 0;
    }
    r.x0 = GUI_Context.DispPosX -= xAdjust;
    r.x1 = r.x0 + xLineSize-1;    
    r.y0 = GUI_Context.DispPosY;
    r.y1 = r.y0 + FontSizeY-1;    
    GUI__DispLine(s, LineLen, &r);
    GUI_Context.DispPosY = r.y0;
    s += LineLen;
    if ((*s=='\n') || (*s=='\r')) {
      switch (GUI_Context.TextAlign & GUI_TA_HORIZONTAL) { 
      case GUI_TA_CENTER:
      case GUI_TA_RIGHT:
        GUI_Context.DispPosX = xOrg;
        break;
      default:
        GUI_Context.DispPosX = GUI_Context.LBorder;
        break;
      }
      if (*s=='\n')
        GUI_Context.DispPosY += GUI_GetFontDistY();
    } else {
      GUI_Context.DispPosX = r.x0+xLineSize;
    }
    if (*s==0)    /* end of string (last line) reached ? */
      break;
  }
  GUI_Context.DispPosY += yAdjust;
  GUI_Context.TextAlign &= ~GUI_TA_HORIZONTAL;
  GUI_UNLOCK();
}




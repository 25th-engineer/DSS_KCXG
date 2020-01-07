/*
*********************************************************************************************************
*                                                
*                                          Include File For 44B0
* use as  #include "Includes.h"
*                                       Copyright 2008, LISUWEIZHAI
*                                           All Rights Reserved
*
*                                          
*********************************************************************************************************
*/

#ifndef __INCLUDES_H__
#define __INCLUDES_H__

#include "option.h"
#include "2440addr.h"
#include "2440lib.h"
#include "2440slib.h"
#include "def.h"
#include "lcdd.h"
#include "others.h"
#include "LCD_MAP.h"
#include "WAVE_MAP.h"
#include "gui.h"
//#include "string.h"
//#include  "math.h"
//#include "stdlib.h"

/*************************************DEFINE****************************************************/ 
#define 	printf 			Uart_Printf
#define		STOP 			while(1)


/*************************************CONFIG****************************************************/
#define 	 UCOS
//#define		GPRS_MODEM
//#define		LCD_Driver_12864
//#define  	KeyBoard4_4
//#define		Menu
//#define 	 	Ds18b20
//#define 	 	ZLGGUI
//#define		RTC_44B0
//#define		ADC_44B0
//#define		UART_44B0






/*************************************USING UCOS****************************************************/
#ifdef   UCOS
//#define   show_busy              //显示cpu忙闲状态开关标志 
//#include    "uhal.h"
#include    "os_cpu.h"
#include    "os_cfg.h"
#include    "ucos_ii.h"
#endif


/*************************************USING ZLGGUI****************************************************/
#ifdef   ZLGGUI
#include    "GUI_CONFIG.H"
#include    "LCMDRV.H"
#include    "GUI_BASIC.H"
#include    "GUI_STOCKC.H"
#include    "FONT_MACRO.H"
#include    "FONT5_7.H"
#include    "FONT8_8.H"
#include    "FONT24_32.H"
#include    "LOADBIT.H"
#include    "WINDOWS.H"
#include    "MENU.H"
#include    "GUI_BASIC.H"
#include    "SPLINE.H"
#endif

/*************************************USING KeyBoard4*4*************************************************/
#ifdef  KeyBoard4_4
#include  "KeyBoard 4x4.h"
#include  "KeyPro.h"
#endif



/*************************************USING 12864_LCD_Driver*************************************************/
#ifdef  LCD_Driver_12864
#include  "LCD_Driver.h"
#endif



/*************************************USING MUNE*************************************************/
#ifdef  Menu
#include  "Menu.h"
#endif



/*************************************USING GPRS_MODEM****************************************************/
#ifdef	GPRS_MODEM
#include  "GPRS_Modem.h"
#endif



/*************************************USING GPRS_MODEM****************************************************/
#ifdef	Ds18b20
#include  "Ds1820.h"
#endif


/*************************************USING 44B0  RTC****************************************************/
#ifdef	RTC_44B0
#include  "44B0_RTC.h"
#endif


/*************************************USING 44B0  ADC****************************************************/

#ifdef	ADC_44B0
#include  "44B0_ADC.h"
#endif


/*************************************USING 44B0  UART****************************************************/

#ifdef	UART_44B0
#include  "44B0_UART.h"
#endif


/*****************************************ENDIF****************************************************/
#endif


